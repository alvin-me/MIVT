#include "filesystem.h"
#include "logmanager.h"

#include <algorithm>
#include <sstream>
#include <time.h>
#include <stack>
#include <sys/stat.h>

#include <windows.h>
#include <direct.h>
#include <sys/utime.h>

#pragma warning(disable:4127)

using std::string;

// anonymous namespace
namespace {

  enum {
    TEMP_BUFFERSIZE = 128,
    LINE_BUFFERSIZE = 512,
  };

  size_t findDelim(char* buf, size_t size, char delim) {
    size_t pos = 0;
    while (pos < (size - 1)) {
      if (buf[pos] == delim)
        return pos;
      ++pos;
    }
    return size + 1;
  }

  const string PATH_SEPARATORS = "/\\";

  string replaceAllCharacters(const std::string& name, const char oldChar,
    const char newChar)
  {
    // the simplest algorithm but it should be fast enough
    //
    string conv(name);
    for (size_t i = 0; i < conv.size(); ++i) {
      if (conv[i] == oldChar)
        conv[i] = newChar;
    }
    return conv;
  }

  size_t removeTrailingCharacters(std::string& str, const char trailer) {
    if (str.empty())
      return 0;

    size_t pos = str.find_last_not_of(trailer);
    if (pos != std::string::npos) {
      size_t count = str.size() - (pos + 1);
      str.resize(pos + 1);
      return count;
    }

    return 0;
  }

} // end anonymous namespace

namespace tgt {

  File::File()
    : size_(0)
  {}

  File::File(const std::string& name)
    : name_(name),
    size_(0)
  {}

  File::~File()
  {}

  size_t File::readLine(char *buf, size_t maxCount, char delim) {
    size_t bytesread = 0;
    size_t start = tell();
    char* p = buf;

    if (tell() + maxCount > size())
      maxCount = size() - tell();
    while (bytesread < maxCount) {
      size_t bufsize = TEMP_BUFFERSIZE;
      if ((bytesread + TEMP_BUFFERSIZE) > maxCount)
        bufsize = (maxCount - bytesread);
      read(p, bufsize);
      size_t d = findDelim(p, bufsize, delim);
      if (d < bufsize) {
        bytesread += d;
        p[d] = 0;
        seek(start + bytesread + 1);
        return bytesread;
      }
      p += bufsize;
      bytesread += bufsize;
    }
    return bytesread;
  }

  std::string File::getLine(char delim) {
    char* buf = new char[LINE_BUFFERSIZE];
    size_t br = readLine(buf, LINE_BUFFERSIZE, delim);
    std::string ret(buf, br);
    while (br == LINE_BUFFERSIZE) {
      br = readLine(buf, LINE_BUFFERSIZE, delim);
      ret.append(buf, br);
    }
    delete[] buf;
    return ret;
  }

  std::string File::getAsString(void) {
    char* buf = new char[size()];
    read(buf, size());
    std::string ret(buf, size());
    delete[] buf;
    return ret;
  }

  size_t File::skipLine(char delim) {
    size_t bytesread = 0;
    size_t start = tell();
    char* p = new char[TEMP_BUFFERSIZE];
    while (true) {
      read(p, TEMP_BUFFERSIZE);
      size_t d = findDelim(p, TEMP_BUFFERSIZE, delim);
      if (d < TEMP_BUFFERSIZE) {
        bytesread += d - 1;
        delete[] p;
        seek(start + bytesread + 2);
        return bytesread;
      }
      bytesread += TEMP_BUFFERSIZE;
    }
  }

  //------------------------------------------------------------------------------

  const std::string RegularFile::loggerCat_("common.RegularFile");

  RegularFile::RegularFile(const std::string& filename)
    : File(filename)
  {
    file_.open(filename.c_str(), std::ios::binary);

    if (!file_) {
      LDEBUG("Cannot open file: " << filename);
      return;
    }

    // get file size
    file_.seekg(0, std::ios::end);
    size_ = tell();
    file_.seekg(std::ios::beg);
  }

  RegularFile::~RegularFile() {
    close();
  }

  void RegularFile::close() {
    if (isOpen())
      file_.close();
  }

  size_t RegularFile::read(void* buf, size_t count) {
    if (!isOpen() || eof())
      return 0;

    file_.read(static_cast<char*>(buf), count);
    return static_cast<size_t>(file_.gcount());
  }

  void RegularFile::skip(size_t count) {
    if (isOpen())
      file_.seekg(count, std::ios::cur);
  }

  void RegularFile::seek(std::streamoff pos) {
    if (isOpen())
      file_.seekg(pos);
  }

  void RegularFile::seek(std::streamoff offset, File::SeekDir seekDir) {
    if (isOpen()) {
      switch (seekDir)  {
      case File::BEGIN:
        file_.seekg(offset, std::ios_base::beg);
        break;
      case File::CURRENT:
        file_.seekg(offset, std::ios_base::cur);
        break;
      case File::END:
        file_.seekg(offset, std::ios_base::end);
        break;
      }
    }
  }

  size_t RegularFile::tell() {
    if (isOpen()) {
      std::streampos p = file_.tellg();
      if (p >= 0)
        return static_cast<size_t>(p);
    }
    return 0;
  }

  bool RegularFile::eof() {
    if (isOpen()) {
      if (tell() == size())
        return true;
      else
        return file_.eof();
    }
    else {
      return true;
    }
  }

  bool RegularFile::isOpen() {
    return file_.is_open();
  }

  bool RegularFile::good() {
    if (isOpen())
      return file_.good();
    else
      return false;
  }

  //------------------------------------------------------------------------------

  MemoryFile::MemoryFile(const char* data, size_t size, const std::string& filename, bool deleteData)
    : File(filename),
    data_(data),
    pos_(0),
    deleteData_(deleteData)
  {
    size_ = size;
  }

  MemoryFile::~MemoryFile(){
    if (data_ && deleteData_)
      delete[] data_;
  }

  void MemoryFile::close() {
    pos_ = 0;
  }

  size_t MemoryFile::read(void* buf, size_t count) {
    if ((pos_ + count) < size_) {
      memcpy(buf, (const void*)(data_ + pos_), count);
      pos_ += count;
      return count;
    }
    else {
      if (eof())
        return 0;
      size_t r = size_ - pos_;
      memcpy(buf, (const void*)(data_ + pos_), r);
      pos_ = size_ - 1;
      return r;
    }
  }

  void MemoryFile::skip(size_t count) {
    pos_ += count;
    if (pos_ >= size_)
      pos_ = size_ - 1;
  }

  void MemoryFile::seek(std::streamoff pos) {
    pos_ = static_cast<size_t>(pos);
    if (pos_ >= size_)
      pos_ = size_ - 1;
  }

  void MemoryFile::seek(std::streamoff offset, File::SeekDir seekDir) {
    switch (seekDir) {
    case File::BEGIN:
      seek(offset);
      break;
    case File::CURRENT:
      seek(pos_ + offset);
      break;
    case File::END:
      pos_ = size_ - 1;
      break;
    }
  }

  size_t MemoryFile::tell() {
    return pos_;
  }

  bool MemoryFile::eof() {
    return (pos_ >= size_ - 1);
  }

  bool MemoryFile::isOpen() {
    return true;
  }

  bool MemoryFile::good() {
    // only possibility not to be good is on EOF
    return !eof();
  }

  //------------------------------------------------------------------------------

  MemoryFileFactory::MemoryFileFactory(const std::string& filename, char* data, size_t size) {
    data_ = data;
    size_ = size;
    filename_ = filename;
  }

  MemoryFileFactory::MemoryFileFactory(const std::string& filename, const std::string& data) {
    size_ = data.length();
    data_ = new char[size_];
    memcpy(data_, data.c_str(), size_);
    filename_ = filename;
  }

  File* MemoryFileFactory::open(const std::string& filename) {
    if (filename == filename_)
      return new MemoryFile(data_, size_, filename_);
    else
      return 0;
  }

  std::string MemoryFileFactory::getFilename() {
    return filename_;
  }

  //------------------------------------------------------------------------------

  const std::string FileSystem::loggerCat_("common.FileSystem");
  const char FileSystem::goodSlash_ = '\\';
  const char FileSystem::badSlash_ = '/';

  FileSystem::FileSystem() {
  }

  FileSystem::~FileSystem() {
    while (!factories_.empty()) {
      delete (factories_.back());
      factories_.pop_back();
    }
  }

  File* FileSystem::open(const std::string& filename) {
    if (virtualFS_.find(filename) != virtualFS_.end()) {
      LDEBUG("Opening file " << filename << " from virtualFS");
      return virtualFS_[filename]->open(filename);
    }
    else {
      LDEBUG("Opening file " << filename << " from real FS");

      RegularFile* f = new RegularFile(filename);
      if (f->isOpen()) {
        return f;
      }
      else {
        delete f;
        return 0;
      }
    }
  }

  bool FileSystem::exists(const std::string& filename) {
    if (virtualFS_.find(filename) != virtualFS_.end()) {
      LDEBUG("Not checking if file " << filename << " exists in virtualFS, unimplemented.");
      return true;
    }
    else {
      std::ifstream file(filename.c_str(), std::ios::binary);
      return file.good();
    }
  }

  void FileSystem::addFactory(FileFactory* ff) {
    std::string file = ff->getFilename();
    virtualFS_[file] = ff;
    factories_.push_back(ff);
  }

  void FileSystem::addMemoryFile(const std::string& filename, char* data, size_t size) {
    FileFactory* ff = new MemoryFileFactory(filename, data, size);
    addFactory(ff);
  }

  void FileSystem::addMemoryFile(const std::string& filename, const std::string& data) {
    FileFactory* ff = new MemoryFileFactory(filename, data);
    addFactory(ff);
  }

  //
  // static methods for the regular filesystem
  //

  bool FileSystem::isAbsolutePath(const std::string& path) {
    return ((path.size() > 0 && path.at(0) == '/') || (path.size() > 1 && path.at(1) == ':'));
  }

  string FileSystem::absolutePath(const string& path) {
    char* buffer;
    buffer = static_cast<char*>(malloc(4096));
    buffer[0] = 0;
    if (GetFullPathName(path.c_str(), 4096, buffer, 0) == 0) {
      free(buffer);
      buffer = 0;
    }

    if (buffer) {
      string result(buffer);
      free(buffer);
      return cleanupPath(result);
    }
    return path;
  }

  std::string FileSystem::relativePath(const std::string& path, const std::string& dir) {
    // when there is no dir we just return the path
    if (dir.empty())
      return path;

    // make paths absolute and add trailing separator
    string abspath = cleanupPath(absolutePath(path)) + "/";
    string absdir = cleanupPath(absolutePath(dir)) + "/";

    // if both directories are the same we return an empty relative path
    if (abspath.compare(absdir) == 0)
      return "";

    // do not generate a relative patch across different Windows drives
    if (!abspath.empty() && !absdir.empty() && (abspath[0] != absdir[0])) {
      return path;
    }

    // find common part in path and dir string
    string::size_type pospath = abspath.find_first_of(PATH_SEPARATORS);
    string::size_type posdir = absdir.find_first_of(PATH_SEPARATORS);
    size_t i = 0;
    while (abspath.compare(0, pospath, absdir, 0, posdir) == 0) {
      i = pospath;
      pospath = abspath.find_first_of(PATH_SEPARATORS, pospath + 1);
      posdir = absdir.find_first_of(PATH_SEPARATORS, posdir + 1);
    }

    // now we have remaining then non-common parts of both paths
    string restpath = abspath.substr(i + 1);
    string restdir = absdir.substr(i + 1);

    // the remaining path is our initial relative path
    string relative = restpath;

    // add ".." for each path separator in the remaining part of dir
    string::size_type pos = restdir.find_first_of(PATH_SEPARATORS);
    while (pos != string::npos) {
      relative = "../" + relative;
      pos = restdir.find_first_of(PATH_SEPARATORS, pos + 1);
    }

    // cleanup and return result
    return cleanupPath(relative);
  }

  string FileSystem::fileName(const string& filepath) {
    string::size_type separator = filepath.find_last_of("/\\");
    if (separator != string::npos)
      return filepath.substr(separator + 1);
    else
      return filepath;
  }

  string FileSystem::baseName(const string& filepath) {
    string filename = fileName(filepath);
    string::size_type dot = filename.rfind(".");

    if (dot != string::npos)
      return filename.substr(0, dot);
    else
      return filename;
  }

  string FileSystem::fullBaseName(const string& filepath) {
    string::size_type dot = filepath.rfind(".");

    if (dot != string::npos)
      return filepath.substr(0, dot);
    else
      return filepath;
  }

  string FileSystem::dirName(const std::string& filepath) {
    if (dirExists(filepath)) //< directory passed
      return filepath;

    string::size_type separator = filepath.find_last_of("/\\");
    if (separator != string::npos)
      return filepath.substr(0, separator);
    else
      return "";
  }

  string FileSystem::parentDir(const std::string& dir) {
    std::string curDir = cleanupPath(dir);

    string::size_type separator = curDir.find_last_of("/\\");
    if (separator != string::npos)
      return curDir.substr(0, separator);
    else
      return "";
  }

  string FileSystem::fileExtension(const string& path, bool lowercase) {
    string filename = fileName(path);

    string::size_type dot = filename.rfind(".");
    string extension;
    if (dot != string::npos)
      extension = filename.substr(dot + 1);

    if (lowercase)
      std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

    return extension;
  }

  bool FileSystem::comparePaths(const std::string& path1, const std::string& path2) {

    std::string pathAbs1 = cleanupPath(absolutePath(path1));
    std::string pathAbs2 = cleanupPath(absolutePath(path2));

    return (pathAbs1 == pathAbs2);
  }

  string FileSystem::cleanupPath(std::string path, bool native /*= true*/) {

    if (path.empty())
      return "";

    // remove double path separators
    string::size_type p_start = 0;
#ifdef WIN32
    // do not convert windows network path prefix "\\", if on windows
    if (path.length() >= 2 && path.substr(0, 2) == "\\\\")
      p_start = 2;
#endif
    string::size_type p = p_start;
    while (p != string::npos) {
      string::size_type p_next;
      p_next = path.find("//", p);
      if (p_next == string::npos)
        p_next = path.find("\\\\", p);
      if (p_next == string::npos)
        p_next = path.find("\\/", p);
      if (p_next == string::npos)
        p_next = path.find("/\\", p);

      if (p_next != string::npos)
        path = path.substr(0, p_next) + path.substr(p_next + 1);

      p = p_next;
    }

    // remove trailing separator
    if (path.find_last_of(PATH_SEPARATORS) == path.size() - 1)
      path = path.substr(0, path.size() - 1);

    // convert to win or unix separators
#ifdef WIN32
    bool convertToWin = native;
#else
    bool convertToWin = false;
#endif
    if (convertToWin) {
      // convert to native windows separators
      path = replaceAllCharacters(path, '/', '\\');

      // convert drive letter to uppercase
      if ((path.size()>1) && isalpha(path[0]) && (path[1] == ':'))
        std::transform(path.begin(), path.begin() + 1, path.begin(), toupper);
    }
    else {
      // convert to unix separators
      assert(p_start < path.length()); // see above
      path = path.substr(0, p_start) + replaceAllCharacters(path.substr(p_start), '\\', '/');
    }

    return path;
  }

  std::vector<std::string> FileSystem::splitPath(std::string path) {
    path = cleanupPath(path);

    // split by goodSlash_
    std::vector<std::string> components;
    std::stringstream stream(path);
    std::string item;
    while (std::getline(stream, item, goodSlash_)) {
      components.push_back(item);
    }
    return components;
  }

  std::string FileSystem::currentDirectory() {
    char* buffer = new char[MAX_PATH + 1];
    memset(buffer, 0, MAX_PATH + 1);

    DWORD size = GetCurrentDirectory(MAX_PATH, buffer);
    if (size >= MAX_PATH) {
      delete[] buffer;
      buffer = new char[size + 1];
      memset(buffer, 0, size + 1);
      size = GetCurrentDirectory(size, buffer);
    }
    std::string dir(buffer);
    dir.resize(size);
    delete[] buffer;

    return dir;
  }

  bool FileSystem::changeDirectory(const std::string& directory) {
    if (directory.empty())
      return false;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);
    return (_chdir(converted.c_str()) == 0);
  }

  bool FileSystem::createDirectory(const std::string& directory) {
    if (directory.empty())
      return false;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);
    return (CreateDirectory(converted.c_str(), 0) != 0);
  }

  bool FileSystem::createDirectoryRecursive(const std::string& directory) {
    if (dirExists(directory))
      return true;
    else {
      if (createDirectoryRecursive(parentDir(directory))) {
        return createDirectory(directory);
      }
      else
        return false;
    }
  }

  bool FileSystem::deleteDirectory(const std::string& directory) {
    if (directory.empty())
      return false;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);
    return (_rmdir(converted.c_str()) == 0);
  }

  bool FileSystem::deleteDirectoryRecursive(const std::string& directory) {
    if (directory.empty())
      return false;

    bool success = clearDirectory(directory);
    success &= deleteDirectory(directory);

    return success;
  }

  bool FileSystem::clearDirectory(const std::string& directory) {
    if (directory.empty())
      return false;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    bool success = true;

    //recursively delete all subdirectories
    std::vector<std::string> subDirs = listSubDirectories(converted);
    for (size_t i = 0; i<subDirs.size(); i++) {
      success &= deleteDirectoryRecursive(converted + goodSlash_ + subDirs[i]);
    }
    //delete all files
    std::vector<std::string> files = listFiles(converted);
    for (size_t i = 0; i<files.size(); i++) {
      success &= deleteFile(converted + goodSlash_ + files[i]);
    }

    return success;
  }

  bool FileSystem::deleteFile(const std::string& filename) {
    if (filename.empty())
      return false;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);
    return (DeleteFile(converted.c_str()) != 0);
  }

  bool FileSystem::renameFile(const std::string& filename, const std::string& newName,
    bool ignorePath)
  {
    if (FileSystem::fileExists(filename) == false)
      return false;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    std::string convertedNew = replaceAllCharacters(newName, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);
    removeTrailingCharacters(convertedNew, goodSlash_);

    bool res = false;
    if (ignorePath == true) {
      std::string name = (FileSystem::dirName(filename) + std::string(&goodSlash_, 1)
        + FileSystem::fileName(newName));
      res = (rename(converted.c_str(), name.c_str()) == 0);
    }
    else {
      res = (rename(converted.c_str(), convertedNew.c_str()) == 0);
    }
    return res;
  }

  void FileSystem::copyFile(const std::string& srcFile, const std::string& destFile) throw (Exception) {
    // check if input file/output dir exists
    if (!fileExists(srcFile))
      throw FileNotFoundException("srcFile not found", srcFile);
    std::string destDir = dirName(destFile);
    if (destDir != "" && !dirExists(destDir))
      throw FileNotFoundException("destination directory not found", destDir);

    // open input and output files
    std::ifstream src(srcFile.c_str(), std::ios::in | std::ios::binary);
    if (!src.good())
      throw FileAccessException("failed to open srcFile fir reading", srcFile);
    std::ofstream dest(destFile.c_str(), std::ios::trunc | std::ios::binary);
    if (!dest.good())
      throw FileAccessException("failed to open dest file for writing", destFile);

    // copy data
    dest << src.rdbuf();

    // check if copying succeeded
    if (!src.good())
      throw FileException("reading from source file failed", srcFile);
    if (!dest.good())
      throw FileException("writing to dest file failed", destFile);
  }

  void FileSystem::updateFileTime(const std::string& filename) throw (Exception) {
    if (!fileExists(filename))
      throw Exception("File does not exist: " + filename);

    struct utimbuf newTime;
    newTime.actime = time(NULL);       /* set atime to current time */
    newTime.modtime = time(NULL);      /* set mtime to current time */
    utime(filename.c_str(), &newTime);
  }

  bool FileSystem::fileExists(const std::string& filename) {
    if (filename.empty())
      return false;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    struct stat st;
    return (stat(converted.c_str(), &st) == 0);
  }

  uint64_t FileSystem::dirSize(const std::string& directory, const bool recursive) {
    std::vector<std::string> files;
    if (recursive)
      files = listFilesRecursive(directory, false);
    else
      files = listFiles(directory, false);

    uint64_t size = 0;
    for (size_t i = 0; i<files.size(); i++) {
      size += fileSize(directory + goodSlash_ + files[i]);
    }
    return size;
  }

  uint64_t FileSystem::fileSize(const std::string& filename) {
    if (filename.empty())
      return 0;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    struct stat st;
    if (stat(converted.c_str(), &st) == 0) {
      return st.st_size;
    }
    else {
      // error
      return 0;
    }
  }

  time_t FileSystem::fileTime(const std::string& filename) {
    if (filename.empty())
      return 0;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    struct stat st;
    if (stat(converted.c_str(), &st) == 0) {
      return st.st_mtime;
    }
    else {
      // error
      return 0;
    }
  }

  time_t FileSystem::fileAccessTime(const std::string& filename) {
    if (filename.empty())
      return 0;

    std::string converted = replaceAllCharacters(filename, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    struct stat st;
    if (stat(converted.c_str(), &st) == 0) {
      return st.st_atime;
    }
    else {
      // error
      return 0;
    }
  }

  bool FileSystem::dirExists(const string& dirpath) {
    DWORD result = GetFileAttributes(dirpath.c_str());
    return (result != INVALID_FILE_ATTRIBUTES) && (result & FILE_ATTRIBUTE_DIRECTORY);
  }

  string FileSystem::findWithSubDir(const string& path, const string& subdir, int iterations) {
    string p = path;

    // try in start directory
    if (dirExists(p + "/" + subdir))
      return p;

    // now try parent dirs
    for (int i = 0; i < iterations; i++) {
      p += "/..";
      if (dirExists(p + "/" + subdir))
        return p;
    }

    return "";
  }

  std::vector<std::string> FileSystem::readDirectory(const std::string& directory, const bool sort,
    const bool recursiveSearch) {
    if (recursiveSearch)
      return listFilesRecursive(directory, sort);
    else
      return listFiles(directory, sort);
  }

  std::vector<std::string> FileSystem::listFiles(const std::string& directory, const bool sort) {
    std::vector<std::string> result;
    if (directory.empty())
      return result;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    WIN32_FIND_DATA findFileData = { 0 };
    HANDLE hFind = 0;

    std::stack<std::string> stackDirs;
    std::string dir(converted + "\\*");

    hFind = FindFirstFile(dir.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
      do {
        std::string file(findFileData.cFileName);
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
          result.push_back(file);
        }
      } while (FindNextFile(hFind, &findFileData) != 0);
    }
    FindClose(hFind);

    if (sort)
      std::sort(result.begin(), result.end());

    return result;
  }

  std::vector<std::string> FileSystem::listFilesRecursive(const std::string& directory, const bool sort) {
    std::vector<std::string> result;
    if (directory.empty())
      return result;

    result = listFiles(directory, false);

    std::vector<std::string> subDirs = listSubDirectories(directory, false);
    for (size_t i = 0; i<subDirs.size(); i++) {
      std::vector<std::string> files = listFilesRecursive(directory + "/" + subDirs[i], false);
      for (size_t j = 0; j<files.size(); j++) {
        result.push_back(subDirs[i] + "/" + files[j]);
      }
    }

    if (sort)
      std::sort(result.begin(), result.end());

    return result;
  }

  std::vector<std::string> FileSystem::listSubDirectories(const std::string& directory, const bool sort) {
    std::vector<std::string> result;
    if (directory.empty())
      return result;

    std::string converted = replaceAllCharacters(directory, badSlash_, goodSlash_);
    removeTrailingCharacters(converted, goodSlash_);

    WIN32_FIND_DATA findFileData = { 0 };
    HANDLE hFind = 0;

    std::stack<std::string> stackDirs;
    std::string dir(converted + "\\*");

    hFind = FindFirstFile(dir.c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
      do {
        std::string file(findFileData.cFileName);
        if ((file != ".") && (file != "..")) {
          if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            result.push_back(file);
          }
        }
      } while (FindNextFile(hFind, &findFileData) != 0);
    }
    FindClose(hFind);

    if (sort)
      std::sort(result.begin(), result.end());

    return result;
  }

  std::vector<std::string> FileSystem::listSubDirectoriesRecursive(const std::string& directory, const bool sort) {
    std::vector<std::string> result;
    if (directory.empty())
      return result;

    result = listSubDirectories(directory, false);

    std::vector<std::string> subDirs = result;
    for (size_t i = 0; i<subDirs.size(); i++) {
      std::vector<std::string> dirs = listSubDirectoriesRecursive(directory + "/" + subDirs[i], false);
      for (size_t j = 0; j<dirs.size(); j++) {
        result.push_back(subDirs[i] + "/" + dirs[j]);
      }
    }

    if (sort)
      std::sort(result.begin(), result.end());

    return result;
  }

} // end namespace tgt