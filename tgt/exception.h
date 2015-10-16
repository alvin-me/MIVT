#pragma once

#include <exception>
#include <string>
#include "config.h"

/**
* C++ exception specification ignored
* https://msdn.microsoft.com/en-us/library/sa28fef8.aspx
*/
#pragma warning( disable : 4290 )

namespace tgt {

  /*
  Note: Use std::bad_alloc for out of memory error
  */

  //------------------------------------------------------------------------------

  /**
  * Base class for all exceptions.
  */
  class Exception : public std::exception {
  public:
    TGT_API Exception(const std::string& what = "") : what_(what) {}
    TGT_API virtual ~Exception() throw() {}

    TGT_API virtual const char* what() const throw();
  protected:
    std::string what_;
  };

  //------------------------------------------------------------------------------

  /**
  * Base class for all file based exceptions.
  */
  class FileException : public Exception {
  public:
    /// @param filename The name of the affected file.
    TGT_API FileException(const std::string& what = "", const std::string filename = "")
      : Exception(what), filename_(filename) {}
    TGT_API virtual ~FileException() throw() {}

    /// Returns the name of the affected file.
    TGT_API const std::string& getFileName() const throw() { return filename_; }

  protected:
    std::string getErrorMessage(const std::string& exception, const std::string& what,
      const std::string& filename) const throw();

    std::string filename_;
  };

  //------------------------------------------------------------------------------

  /**
  * Thrown when a file was not found.
  */
  class FileNotFoundException : public FileException {
  public:
    /// @param filename The name of the file which was not found.
    TGT_API FileNotFoundException(const std::string& what = "", const std::string& filename = "");
    TGT_API virtual ~FileNotFoundException() throw() {}
  };

  //------------------------------------------------------------------------------

  /**
  * Thrown when a file couldn't be opened. No proper permessions may be the cause
  * for example.
  */
  class FileAccessException : public FileException {
  public:
    /// @param filename The name of the file which couldn't be opened.
    TGT_API FileAccessException(const std::string& what = "", const std::string& filename = "");
    TGT_API virtual ~FileAccessException() throw() {}
  };

  //------------------------------------------------------------------------------

  /**
  * Thrown when a file was tried to load but file/format of the file is
  * corrupted.
  */
  class CorruptedFileException : public FileException {
  public:
    TGT_API  CorruptedFileException(const std::string& what = "", const std::string& filename = "");
    TGT_API virtual ~CorruptedFileException() throw() {}
  };

  //------------------------------------------------------------------------------

  /**
  * Thrown when a file was tried to load or save in an unsupported format.
  */
  class UnsupportedFormatException : public FileException {
  public:
    /// @param extension The extension which is not supported.
    TGT_API UnsupportedFormatException(const std::string& extension, const std::string& filename = "");
    TGT_API virtual ~UnsupportedFormatException() throw() {}
  };

  //------------------------------------------------------------------------------

  /**
  * Thrown when an i/o operation could not be completed successfull. The cause
  * may be a full or damaged disk for instance. Furthermore this Exception is
  * thrown when no special error information is available when dealing with i/o
  * operations. This is the case when using fstream for instance.
  */
  class IOException : public FileException {
  public:
    TGT_API IOException(const std::string& what = "", const std::string& filename = "");
    TGT_API virtual ~IOException() throw() {}
  };

} // end namespace tgt