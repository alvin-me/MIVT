#pragma once

#include "singleton.h"
#include "exception.h"

#include <map>
#include <vector>
#include <fstream>
#include <stdint.h>

namespace tgt {

  /**
  * Base class for input files.
  */
  class File {
  public:
    /**
    * Offset identifiers for seek(). Used exactly like
    * seekdir parameter in std::istream::seekg().
    */
    enum SeekDir {
      BEGIN,      ///< Offset is specified relatively to beginning of file
      CURRENT,    ///< Offset is specified relatively to current position in file
      END         ///< Offset is specified relatively to end of file
    };

    TGT_API File();
    TGT_API File(const std::string& name);
    TGT_API virtual ~File();

    /// Closes the file.
    TGT_API virtual void close() = 0;

    /// Returns the name of the file
    TGT_API const std::string& getName(void) { return name_; }

    /// Returns the size of the file in bytes (0 if unknown)
    TGT_API size_t size(void) const { return size_; }

    /// Read up to count bytes into buf. Returns the number of bytes read.
    TGT_API virtual size_t read(void* buf, size_t count) = 0;
    /// Reads up to maxCount characters into buf. Stops at first occurence of delim.
    TGT_API virtual size_t readLine(char* buf, size_t maxCount, char delim = '\n');
    /// Reads until first occurence of delim and returns a string
    TGT_API virtual std::string getLine(char delim = '\n');
    /// Reads the whole file and puts it into a string.
    TGT_API virtual std::string getAsString();
    /// Skips until after the first occurence of delim.
    TGT_API virtual size_t skipLine(char delim = '\n');

    /// Skips count bytes (or to end of file, whatever comes first)
    TGT_API virtual void skip(size_t count) = 0;
    /// Seeks to pos (relative to the begin of file)
    TGT_API virtual void seek(std::streamoff pos) = 0;
    /// Seeks to offset (relative to seekDir)
    TGT_API virtual void seek(std::streamoff offset, SeekDir seekDir) = 0;
    /// Returns the current reading position
    TGT_API virtual size_t tell() = 0;

    /// Returns true if the End Of File is reached
    TGT_API virtual bool eof() = 0;
    /// Returns true if the file is open.
    TGT_API virtual bool isOpen() = 0;
    /// Check if the state of the file is good for i/o operations.
    TGT_API virtual bool good() = 0;

  protected:
    std::string name_;
    size_t size_;
  };

  //------------------------------------------------------------------------------

  /**
  * A file from the regular filesystem.
  */
  class RegularFile : public File {
  public:
    /// Open filename
    TGT_API RegularFile(const std::string& filename);
    TGT_API virtual ~RegularFile();

    TGT_API virtual void close();

    TGT_API virtual size_t read(void* buf, size_t count);

    TGT_API virtual void skip(size_t count);
    TGT_API virtual void seek(std::streamoff pos);
    TGT_API virtual void seek(std::streamoff offset, File::SeekDir seekDir);
    TGT_API virtual size_t tell();

    TGT_API virtual bool eof();
    TGT_API virtual bool isOpen();
    TGT_API virtual bool good();

  protected:
    std::ifstream file_;
    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  /**
  * A virtual file, read from a chunk of memory
  */
  class MemoryFile : public File {
  public:
    /// Create memoryfile from data with given size and call it filename.
    /// If deleteData is true the memory file will delete the data upon destruction.
    TGT_API MemoryFile(const char* data, size_t size, const std::string& filename, bool deleteData = false);
    TGT_API ~MemoryFile();

    TGT_API virtual void close();

    TGT_API virtual size_t read(void* buf, size_t count);

    TGT_API virtual void skip(size_t count);
    TGT_API virtual void seek(std::streamoff pos);
    TGT_API virtual void seek(std::streamoff offset, File::SeekDir seekDir);
    TGT_API virtual size_t tell();

    TGT_API virtual bool eof();
    TGT_API virtual bool isOpen();
    TGT_API virtual bool good();

  protected:
    const char* data_;
    size_t pos_;
    bool deleteData_;
  };

  //------------------------------------------------------------------------------

  /**
  * A FileFactory plugs into the virtual FS and creates File objects
  */
  class FileFactory {
  public:
    TGT_API virtual ~FileFactory() {}
    /// Open the file filename from this factory
    TGT_API virtual File* open(const std::string& filename) = 0;
    /// Returns the filename the factory can open.
    TGT_API virtual std::string getFilename() = 0;
  };

  //------------------------------------------------------------------------------

  /**
  * Creates instances of MemoryFile.
  * This factory always provides only one file.
  */
  class MemoryFileFactory : public FileFactory {
  public:
    /// Create factory which provides a file with name filename from the data in memory at data with size size
    TGT_API MemoryFileFactory(const std::string& filename, char* data, size_t size);
    /// Create factory which provides a file with name filename from the data in the string data
    TGT_API MemoryFileFactory(const std::string& filename, const std::string& data);
    TGT_API virtual File* open(const std::string& filename);
    TGT_API virtual std::string getFilename();

  protected:
    char* data_;
    size_t size_;
    std::string filename_;
  };

  //------------------------------------------------------------------------------

  /**
  * Provides transparent access to the filesystem.
  */
  class FileSystem : public Singleton<FileSystem> {
  public:
    TGT_API FileSystem();
    TGT_API ~FileSystem();

    /// Open a file, first checks for availability in virtual FS, then in regular FS.
    TGT_API File* open(const std::string& filename);
    /// Checks wether a file with the specified filename exists.
    TGT_API bool exists(const std::string& filename);
    /// Adds a FileFactory and inserts all files provided by the factory to the virtual FS.
    /// All Factories are deleted upon destruction.
    TGT_API void addFactory(FileFactory* ff);

    /// Creates a MemoryFileFactory and adds it (just for convenience)
    TGT_API void addMemoryFile(const std::string& filename, char* data, size_t size);
    /// Creates a MemoryFileFactory and adds it (just for convenience)
    TGT_API void addMemoryFile(const std::string& filename, const std::string& data);

    //
    // Static methods for file system information
    // NOTE: These do not work with the virtual filesystem!
    //

    //
    // path information/manipulation
    //

    /**
    * Returns whether the passed path is an absolute one.
    */
    TGT_API static bool isAbsolutePath(const std::string& path);

    /**
    * Returns the canonicalized absolute pathname.
    */
    TGT_API static std::string absolutePath(const std::string& path);

    /**
    * Returns a path relative to dir.
    */
    TGT_API static std::string relativePath(const std::string& path, const std::string& dir);

    /**
    * Return the file name without the path component.
    */
    TGT_API static std::string fileName(const std::string& filepath);

    /**
    * Return the file name without the path component and without suffix.
    */
    TGT_API static std::string baseName(const std::string& filepath);

    /**
    * Return the file name with the path component and without suffix.
    */
    TGT_API static std::string fullBaseName(const std::string& filepath);

    /**
    * Return the full directory path without the file name component.
    */
    TGT_API static std::string dirName(const std::string& filepath);

    /**
    * Return the parent directory of the directory.
    */
    TGT_API static std::string parentDir(const std::string& dir);

    /**
    * Return the file extension (suffix) from the path.
    * @param lowercase convert result to lower case
    */
    TGT_API static std::string fileExtension(const std::string& path, bool lowercase = false);

    /**
    * Returns a string containing the current working directory.
    */
    TGT_API static std::string currentDirectory();

    /**
    * Returns whether the passed paths refer to the same file/directory.
    */
    TGT_API static bool comparePaths(const std::string& path1, const std::string& path2);

    /**
    * Removes double and trailing path separators.
    * @param native if true, native path separators will be used (\ on Windows, / on Unix).
    *  Otherwise, Unix path separators will be used.
    */
    TGT_API static std::string cleanupPath(std::string path, bool native = true);

    /**
    * Splits a path into its directory/file components.
    */
    TGT_API static std::vector<std::string> splitPath(std::string path);

    //
    // file system manipulation/information
    //

    /**
    * Changes the current working directory to the given one.
    */
    TGT_API static bool changeDirectory(const std::string& directory);

    /**
    * Creates the directory of the give name if it does not already exists.
    *
    * @param   directory   name for the new directory to be created
    * @return  true if the creation was succesful or false otherwise
    */
    TGT_API static bool createDirectory(const std::string& directory);

    TGT_API static bool createDirectoryRecursive(const std::string& directory);

    TGT_API static bool deleteDirectory(const std::string& directory);

    /**
    * Deletes the directory after clearing its contents.
    */
    TGT_API static bool deleteDirectoryRecursive(const std::string& directory);

    /**
    * Deletes all files and subdirectories of the passed directory.
    */
    TGT_API static bool clearDirectory(const std::string& directory);

    /**
    * Deletes the file with the given filename.
    *
    * @param   filename    name of the file to be deleted
    * @return  true if the file has been deleted successfully, false otherwise.
    */
    TGT_API static bool deleteFile(const std::string& filename);

    /**
    * Returns whether the given directory exists.
    */
    TGT_API static bool dirExists(const std::string& dirpath);

    /**
    * Returns the existed sub dir in a specified folder path.
    */
    TGT_API static std::string findWithSubDir(const std::string& path, const std::string& subdir, int iterations = 0);

    /**
    * Determines whether a file of the given name exists or not.
    *
    * @param   filename   name of the file which is suspected to exist
    * @return  true if the file exists, false otherwise
    */
    TGT_API static bool fileExists(const std::string& filename);

    /// Returns the file size in bytes or 0 if the file doesn't exist.
    TGT_API static uint64_t fileSize(const std::string& filename);

    /// Returns the size (in bytes) of all files in the directory.
    TGT_API static uint64_t dirSize(const std::string& directory, const bool recursive = true);

    /// Returns the last time the specified file has been modified.
    TGT_API static time_t fileTime(const std::string& filename);

    /// Returns the last time the specified file has been accessed.
    TGT_API static time_t fileAccessTime(const std::string& filename);

    /**
    * Reads the content of the the directory, ignoring "." and ".."
    *
    * @param   directory   the directory to be read
    * @param   sort    determines whether the returned vector should be sorted in
    *                  alphabetical order
    * @param   recursiveSearch determines whether to search the directory recursively.
    * @return  names of all files contained in the given directory. Files from sub-
    *          directories are inserted like "/subdirectory/file".
    *          If the directory does not exist, the returned vector is empty.
    */
    TGT_API static std::vector<std::string> readDirectory(const std::string& directory,
      const bool sort = false,
      const bool recursiveSearch = false);

    /**
    * Returns all files in a given directory (non-recursive).
    *
    * @param   directory   the directory to be read
    * @param   sort    determines whether the returned vector should be sorted in
    *                  alphabetical order
    * @return  names of all files contained in the given directory.
    *          If the directory does not exist, the returned vector is empty.
    */
    TGT_API static std::vector<std::string> listFiles(const std::string& directory, const bool sort = false);

    /**
    * Returns all files in a given directory (recursive).
    *
    * @param   directory   the directory to be read
    * @param   sort    determines whether the returned vector should be sorted in
    *                  alphabetical order
    * @return  names of all files contained in the given directory. Files from sub-
    *          directories are inserted like "subdirectory/file".
    *          If the directory does not exist, the returned vector is empty.
    */
    TGT_API static std::vector<std::string> listFilesRecursive(const std::string& directory, const bool sort = false);

    /**
    * Returns all subdirectories of a given directory (non-recursive).
    *
    * @param   directory   the directory to be read
    * @param   sort    determines whether the returned vector should be sorted in
    *                  alphabetical order
    * @return  names of all directories contained in the given directory.
    *          If the directory does not exist, the returned vector is empty.
    */
    TGT_API static std::vector<std::string> listSubDirectories(const std::string& directory, const bool sort = false);

    /**
    * Returns all subdirectories of a given directory (recursive).
    *
    * @param   directory   the directory to be read
    * @param   sort    determines whether the returned vector should be sorted in
    *                  alphabetical order
    * @return  names of all directories contained in the given directory. Files from sub-
    *          directories are inserted like "subdirectory/subdirectory".
    *          If the directory does not exist, the returned vector is empty.
    */
    TGT_API static std::vector<std::string> listSubDirectoriesRecursive(const std::string& directory, const bool sort = false);

    /**
    * Renames the file given by filename, if it exists to the name given in
    * newName. If ignorePath is true (default), the file will only be renamed,
    * but not moved. Otherwise, if newName contains a path and the path exists,
    * file is moved to that directory.
    *
    * @param   filename    name of the file to be renamed
    * @param   newName new name of the file
    * @param   ignorePath  Determines whether to rename the file in its current
    *                      directory only or whether to move to a different
    *                      directory which might be containing in newName
    * @return  true if the files was renamed/moved successfully, false otherwise.
    */
    TGT_API static bool renameFile(const std::string& filename, const std::string& newName,
      bool ignorePath = true);

    /**
    * Copies a file by using file streams.
    *
    * @param   srcFile     Path to the file to be copied.
    * @param   destFile    Path to the destination file. Its directory must exist.
    *
    * @throw tgt::Exception if file copying failed.
    */
    TGT_API static void copyFile(const std::string& srcFile, const std::string& destFile)
      throw (tgt::Exception);

    /**
    * Sets the access and modification times of the specified file
    * to the current timestamp.
    */
    TGT_API static void updateFileTime(const std::string& filename)
      throw (tgt::Exception);


  protected:
    std::map<std::string, FileFactory*> virtualFS_;
    std::vector<FileFactory*> factories_;
    static const std::string loggerCat_;
    static const char goodSlash_;   /** '/' on non-Windows OS, '\' on Windows OS */
    static const char badSlash_;    /** '\' on non-Windows OS, '/' on Windows OS */
  };

} // end namespace tgt

#define FileSys tgt::Singleton<tgt::FileSystem>::getRef()