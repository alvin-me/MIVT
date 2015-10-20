#pragma once

#include "singleton.h"
#include "config.h"

#include <vector>
#include <sstream>

#pragma warning(disable:4127)

namespace tgt {

  class LogManager;
  template class TGT_API Singleton<LogManager>;

  /**
  * Specifies the severity of the log event.
  * Debug messages are not logged in release builds!
  */
  enum LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
  };

  /**
  * Holds the information for filtering messages.
  */
  struct LogFilter {
    std::string cat_;
    bool children_;
    LogLevel level_;
  };

  /**
  * Abstract basis class for logging messages.
  */
  class Log {
  public:
    TGT_API virtual ~Log() {}

    /// Log a message in this Log (message is filtered based on cat and level)
    TGT_API virtual void log(const std::string &cat, LogLevel level, const std::string &msg, const std::string &extendedInfo = "");

    /**
    * Add a category that is accepted to this log.
    * @param cat All messages with category = cat are accepted and logged in this log.
    * @param Children If true all messages of subcategories of cat are also accepted.
    * example:
    * Texture.Reader is a subcategory of Texture
    * @param level All messages below this LogLevel are discarded, even if the category matches.
    */
    TGT_API virtual void addCat(const std::string &cat, bool Children = true, LogLevel level = Debug);
    TGT_API virtual bool isOpen() = 0;

    /// Sets the log level of all filters registered at this Log.
    TGT_API virtual void setLogLevel(LogLevel level);

    /// Returns if the messages are time-stamped.
    TGT_API inline bool getTimeStamping() const { return timeStamping_; }
    TGT_API inline void setTimeStamping(const bool timeStamping) { timeStamping_ = timeStamping; }

    /// Returns if the messages are date-stamped.
    TGT_API inline bool getDateStamping() const { return dateStamping_; }
    TGT_API inline void setDateStamping(const bool dateStamping) { dateStamping_ = dateStamping; }

    /// Returns if the category is printed along with the messages
    TGT_API inline bool getShowCat() const { return showCat_; }
    TGT_API inline void setShowCat(const bool showCat) { showCat_ = showCat; }

    /// Returns if the LogLevel is printed along with the messages.
    TGT_API inline bool getShowLevel() const { return showLevel_; }
    TGT_API inline void setShowLevel(const bool showLevel) { showLevel_ = showLevel; }

  protected:
    virtual bool testFilter(const std::string &cat, LogLevel level);
    virtual void logFiltered(const std::string &cat, LogLevel level, const std::string &msg,
      const std::string &extendedInfo = "") = 0;
    virtual std::string getTimeString();
    virtual std::string getDateString();
    virtual std::string getLevelString(LogLevel level);

    bool timeStamping_, dateStamping_, showCat_, showLevel_;
    std::vector<LogFilter> filters_;
  };

  //------------------------------------------------------------------------------

  /// Implements logging to a plain Textfile
  class TextLog : public Log {
  public:
    TGT_API TextLog(const std::string &filename, bool dateStamping = true, bool timeStamping = true,
      bool showCat = true, bool showLevel = true);
    TGT_API virtual ~TextLog();
    TGT_API bool isOpen();

  protected:
    FILE* file_;
    void logFiltered(const std::string &cat, LogLevel level, const std::string &msg, const std::string &extendedInfo = "");
    std::string getLevelColor(LogLevel level);
  };

  //------------------------------------------------------------------------------

  ///Implements a colored html log.
  class HtmlLog : public Log {
  public:
    TGT_API HtmlLog(const std::string &filename, bool dateStamping = false, bool timeStamping = true,
      bool showCat = true, bool showLevel = true, bool append = true);
    TGT_API ~HtmlLog();
    TGT_API bool isOpen();

    TGT_API std::string getAbsFilename() const;

  protected:
    void logFiltered(const std::string &cat, LogLevel level, const std::string &msg, const std::string &extendedInfo = "");
    std::string getLevelString(LogLevel level);
    std::string getLevelColor(LogLevel level);

    FILE* file_;
    std::string absFilename_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  /**
  * The Logmanager distributes logmessages to all Logs registered to the manager.
  * Logmessages consist of a message, a logging category and a loglevel.
  * See Log and LogLevel classes for further information.
  *
  * Usage:
  * Define "static const std::string loggerCat_;" in every class that logs messages and use the macros defined below:
  * LWARNING("Warning!");
  * Alternatively, LWARNINGC("Cat", "Warning!") may be used, which does not require the definition of loggerCat_.
  *
  * LDEBUG statements are removed if _DEBUG is not defined!
  * @author Stefan Diepenbrock
  */
  class LogManager : public Singleton<LogManager> {
  public:
    TGT_API LogManager(const std::string& logDir = "");
    TGT_API ~LogManager();

    /// Initialize logmanager, put all logfiles in logDir
    TGT_API void setLogDir(const std::string& logDir);
    TGT_API std::string getLogDir() const { return logDir_; }

    /// Log message
    TGT_API void log(const std::string& cat, LogLevel level, const std::string& msg, const std::string& extendedInfo = "");

    /// Add a log to the manager, from now all messages received by the manager are also distributed to this log.
    /// All logs are deleted upon destruction of the manager.
    /// If a ConsoleLog is added it will replace an existing one, the old one will be deleted.
    TGT_API void addLog(Log* log);

    // Remove a log from the manager.
    TGT_API void removeLog(Log* log);

    /// Removes all registered logs.
    TGT_API void clear();

    /// Sets the log level of all registered Logs.
    TGT_API void setLogLevel(LogLevel level);

    /// Returns all Logs currently registered at the LogManager.
    TGT_API std::vector<Log*> getLogs() const;

  protected:
    std::string logDir_;
    std::vector<Log*> logs_;
  };

} // end namespace tgt

//------------------------------------------------------------------------------

#define LogMgr tgt::Singleton<tgt::LogManager>::getRef()

#ifdef _DEBUG

  #define LDEBUG(msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Debug, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LINFO(msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Info, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LWARNING(msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Warning, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LERROR(msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Error, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LFATAL(msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Fatal, _tmp.str(), _tmp2.str()); \
  } while (0)

  //with category parameter:
  #define LDEBUGC(cat, msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Debug, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LINFOC(cat, msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Info, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LWARNINGC(cat, msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Warning, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LERRORC(cat, msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Error, _tmp.str(), _tmp2.str()); \
  } while (0)

  #define LFATALC(cat, msg) \
  do { \
    std::ostringstream _tmp, _tmp2; \
    _tmp2 << __FUNCTION__ << " File: " << __FILE__ << "@" << __LINE__; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Fatal, _tmp.str(), _tmp2.str()); \
  } while (0)

#else // _DEBUG

  #define LDEBUG(msg)
  //#define LDEBUG(msg) \
  //do { \
  //  std::ostringstream _tmp; \
  //  _tmp << msg; \
  //  LogMgr.log(loggerCat_, tgt::Debug, _tmp.str()); \
  //} while (0)

  #define LINFO(msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Info, _tmp.str()); \
  } while (0)

  #define LWARNING(msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Warning, _tmp.str()); \
  } while (0)

  #define LERROR(msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Error, _tmp.str()); \
  } while (0)

  #define LFATAL(msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(loggerCat_, tgt::Fatal, _tmp.str()); \
  } while (0)

  //
  // with category parameter
  //

  //#define LDEBUGC(cat, msg)
  #define LDEBUGC(cat, msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Debug, _tmp.str()); \
  } while (0)

  #define LINFOC(cat, msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Info, _tmp.str()); \
  } while (0)

  #define LWARNINGC(cat, msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Warning, _tmp.str()); \
  } while (0)

  #define LERRORC(cat, msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Error, _tmp.str()); \
  } while (0)

  #define LFATALC(cat, msg) \
  do { \
    std::ostringstream _tmp; \
    _tmp << msg; \
    LogMgr.log(cat, tgt::Fatal, _tmp.str()); \
  } while (0)

#endif // _DEBUG