#include "logmanager.h"
#include "filesystem.h"

#include <ctime>

#pragma warning(disable:4996)

namespace tgt {

  bool Log::testFilter(const std::string &cat, LogLevel level) {
    for (size_t i = 0; i < filters_.size(); i++)     {
      if (filters_[i].children_) {
        if (cat.find(filters_[i].cat_, 0) == 0) {
          if (filters_[i].level_ <= level)
            return true;
        }
      }
      else {
        if (filters_[i].cat_ == cat) {
          if (filters_[i].level_ <= level)
            return true;
        }
      }
    }
    return false;
  }

  void Log::log(const std::string &cat, LogLevel level, const std::string &msg, const std::string &extendedInfo) {
    if (testFilter(cat, level))
      logFiltered(cat, level, msg, extendedInfo);
  }

  void Log::addCat(const std::string &cat, bool children, LogLevel level) {
    LogFilter newFilter;
    newFilter.cat_ = cat;
    newFilter.children_ = children;
    newFilter.level_ = level;
    filters_.push_back(newFilter);
  }

  void Log::setLogLevel(LogLevel level) {
    for (size_t i = 0; i<filters_.size(); i++)
      filters_.at(i).level_ = level;
  }

  std::string Log::getTimeString() {
    time_t long_time = 0;
    tm *now = 0;
    time(&long_time);
    now = localtime(&long_time);
    char SzBuffer[256];
    sprintf(SzBuffer, "%.2i:%.2i:%.2i", now->tm_hour, now->tm_min, now->tm_sec);
    std::string temp(SzBuffer);
    return temp;
  }

  std::string Log::getDateString() {
    time_t long_time = 0;
    tm *now = 0;
    time(&long_time);
    now = localtime(&long_time);
    char SzBuffer[256];
    sprintf(SzBuffer, "%.2i.%.2i.%.4i", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    std::string temp(SzBuffer);
    return temp;
  }

  std::string Log::getLevelString(LogLevel level) {
    switch (level) {
    case Debug: return "Debug";
    case Info: return "Info";
    case Warning: return "Warning";
    case Error: return "Error";
    case Fatal: return "Fatal";
    default: return "UNKNOWN";
    }
  }

  //------------------------------------------------------------------------------

  void TextLog::logFiltered(const std::string& cat, LogLevel level, const std::string& msg,
    const std::string& /*extendedInfo*/)
  {
    if (!file_)
      return;

    std::string output = "";
    if (dateStamping_)
      output += "[" + getDateString() + "] ";
    if (timeStamping_)
      output += "[" + getTimeString() + "] ";
    if (showCat_)
      output += cat + " ";
    if (showLevel_)
      output += "(" + getLevelString(level) + ") ";
    if (output != "")
      output += '\t';
    fputs((output + msg + "\n").c_str(), file_);
    fflush(file_);
  }

  TextLog::TextLog(const std::string &filename, bool dateStamping, bool timeStamping, bool showCat, bool showLevel) {
    std::string absFilename;
    if (FileSystem::isAbsolutePath(filename))
      absFilename = filename;
    else
      absFilename = FileSystem::absolutePath(LogMgr.getLogDir().empty() ? "" : LogMgr.getLogDir() + "/") + filename;

    file_ = fopen(absFilename.c_str(), "w");
    timeStamping_ = timeStamping;
    dateStamping_ = dateStamping;
    showCat_ = showCat;
    showLevel_ = showLevel;
  }

  TextLog::~TextLog() {
    if (!file_)
      return;
    fputs("---\n", file_);
    fclose(file_);
  }

  bool TextLog::isOpen() {
    return (file_ != 0);
  }

  //------------------------------------------------------------------------------

  std::string HtmlLog::getLevelString(LogLevel level) {
    switch (level) {
    case Debug:
      return "<td>Debug</TD>";
    case Info:
      return "<td>Info</TD>";
    case Warning:
      return "<td>Warning</TD>";
    case Error:
      return "<td>Error</TD>";
    case Fatal:
      return "<td>Fatal</TD>";
    default:
      return "<td>Unknown</TD>";
    }
  }

  std::string HtmlLog::getLevelColor(LogLevel level) {
    switch (level) {
    case Debug:
      return "#00CC00";
    case Info:
      return "#FFFFFF";
    case Warning:
      return "#FFFF00";
    case Error:
      return "#FF0000";
    case Fatal:
      return "#0000FF";
    default:
      return "#FFFFFF";
    }
  }

  void HtmlLog::logFiltered(const std::string &cat, LogLevel level, const std::string &msg,
    const std::string &extendedInfo)
  {
    if (!file_)
      return;

    std::string output = "\t\t\t<tr bgcolor=\"" + getLevelColor(level) + "\">\n";
    if (dateStamping_)
      output += "\t\t\t\t<td>" + getDateString() + "</td>\n";
    if (timeStamping_)
      output += "\t\t\t\t<td>" + getTimeString() + "</td>\n";
    if (showCat_)
      output += "\t\t\t\t<td>" + cat + "</td>\n";
    if (showLevel_)
      output += "\t\t\t\t" + getLevelString(level) + "\n";
    fputs((output + "\t\t\t\t<td title=\"" + extendedInfo + "\">" + msg + "</td>\n\t\t\t</tr>\n").c_str(), file_);
    fflush(file_);
  }

  HtmlLog::HtmlLog(const std::string &filename, bool dateStamping, bool timeStamping, bool showCat, bool showLevel, bool append) {
    if (FileSystem::isAbsolutePath(filename))
      absFilename_ = filename;
    else
      absFilename_ = FileSystem::absolutePath((LogMgr.getLogDir().empty() ? "" : LogMgr.getLogDir() + "/") + filename);
    file_ = fopen(absFilename_.c_str(), append ? "a" : "w");

    timeStamping_ = timeStamping;
    dateStamping_ = dateStamping;
    showCat_ = showCat;
    showLevel_ = showLevel;

    if (!file_)
      return;

    std::string output = "<html>\n\t<head>\n\t\t<title>MIVT Logfile</title>\n\t</head>\n\t"
      "<body>\n\n\t<table cellpadding=3 cellspacing=0 border=1>\n\t\t"
      "<CAPTION>MIVT Logfile</CAPTION>\n\n\t\t<THEAD>\n\t\t\t<TR>\n";
    if (dateStamping_)
      output += "\t\t\t\t<th>Date</th>\n";
    if (timeStamping_)
      output += "\t\t\t\t<th>Time</th>\n";
    if (showCat_)
      output += "\t\t\t\t<th>Category</th>\n";
    if (showLevel_)
      output += "\t\t\t\t<th>Type</th>\n";
    output += "\t\t\t\t<th>Message</th>\n\t\t\t</tr>\n\t\t<tbody>\n";
    fputs(output.c_str(), file_);
    fflush(file_);
  }

  HtmlLog::~HtmlLog() {
    if (!file_)
      return;
    fputs("\t\t</tbody>\n\t</table>\n\t</body>\n</html>", file_);
    fclose(file_);
  }

  bool HtmlLog::isOpen() {
    return (file_ != 0);
  }

  std::string HtmlLog::getAbsFilename() const {
    return absFilename_;
  }

  //------------------------------------------------------------------------------

  LogManager::LogManager(const std::string& logDir)
    : logDir_(logDir)
  {}


  LogManager::~LogManager() {
    clear();
  }

  void LogManager::reinit(const std::string& logDir) {
    logDir_ = logDir;
  }

  void LogManager::log(const std::string &cat, LogLevel level, const std::string &msg,
    const std::string &extendedInfo)
  {
    std::vector<Log*>::iterator it;
    for (it = logs_.begin(); it != logs_.end(); it++) {
      if (*it != 0)
        (*it)->log(cat, level, msg, extendedInfo);
    }
  }

  void LogManager::addLog(Log* log) {
    logs_.push_back(log);
  }

  void LogManager::removeLog(Log* log) {
    std::vector<Log*>::iterator iter = logs_.begin();
    while (iter != logs_.end()) {
      if (*iter == log)
        iter = logs_.erase(iter);
      else
        ++iter;
    }
  }

  void LogManager::setLogLevel(LogLevel level) {
    for (size_t i = 0; i<logs_.size(); i++)
      logs_.at(i)->setLogLevel(level);
  }

  void LogManager::clear() {
    std::vector<Log*>::iterator it;
    for (it = logs_.begin(); it != logs_.end(); it++)
      delete (*it);
    logs_.clear();
  }

  std::vector<Log*> LogManager::getLogs() const {
    return logs_;
  }

} // end namespace tgt