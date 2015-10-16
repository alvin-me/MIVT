#include "exception.h"

namespace tgt {

  const char* Exception::what() const throw() {
    return what_.c_str();
  };

  //------------------------------------------------------------------------------

  std::string FileException::getErrorMessage(const std::string& exception, const std::string& what,
    const std::string& filename) const throw()
  {
    std::string s = exception;

    if (!what.empty())
      s += ": " + what;

    if (!filename.empty())
      s += " (" + filename + ")";

    return s;
  }

  //------------------------------------------------------------------------------

  FileNotFoundException::FileNotFoundException(const std::string& what, const std::string& filename)
    : FileException(getErrorMessage("FileNotFoundException", what, filename), filename)
  {}

  //------------------------------------------------------------------------------

  FileAccessException::FileAccessException(const std::string& what, const std::string& filename)
    : FileException(getErrorMessage("FileAccessException", what, filename), filename)
  {}

  //------------------------------------------------------------------------------

  CorruptedFileException::CorruptedFileException(const std::string& what, const std::string& filename)
    : FileException(getErrorMessage("CorruptedFileException", what, filename), filename)
  {}

  //------------------------------------------------------------------------------

  UnsupportedFormatException::UnsupportedFormatException(const std::string& extension, const std::string& filename)
    : FileException(getErrorMessage("UnsupportedFormatException", "unsupported extension: " + extension, filename),
    filename)
  {}

  //------------------------------------------------------------------------------

  IOException::IOException(const std::string& what, const std::string& filename)
    : FileException(getErrorMessage("IOException", what, filename), filename)
  {}


} // end namespace tgt