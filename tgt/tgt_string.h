#pragma once

#include "exception.h"

#include <string>
#include <stdint.h>
#include <vector>

namespace tgt {

  /// Converts an int to a string.
  TGT_API std::string itos(int i, int stringLength = -1, char fillChar = '0');

  /// Converts an uint32_t to a string.
  TGT_API std::string itos(uint32_t i, int stringLength = -1, char fillChar = '0');

  /// Converts an uint64_t to a string.
  TGT_API std::string itos(uint64_t i, int stringLength = -1, char fillChar = '0');

  /**
  * Converts a float to a string.
  *
  * @param precision number of decimals to print.
  *  For precision=-1, sprintf's standard floating point formatting is used (%f).
  */
  TGT_API std::string ftos(float f, int precision = -1);

  /**
  * Converts a double to a string.
  *
  * @param precision number of decimals to print.
  *  For precision=-1, sprintf's standard floating point formatting is used (%f).
  */
  TGT_API std::string dtos(double d, int precision = -1);

  /**
  * Converts the string to a null-terminated char array with length s.size()+1.
  * Deleting the allocated memory is up to the caller.
  */
  TGT_API char* strToChr(const std::string& s);

  /// Generic to-string conversion using a stringstream.
  template<class T>
  std::string genericToString(const T& value);

  /**
  * Generic from-string conversion using a stringstream.
  *
  * @throw VoreenException if the conversion failed
  */
  template<class T>
  T genericFromString(const std::string& str)
    throw (Exception);

  /// Returns a copy of \p str where all occurrences of \p from have been replaced by \p to.
  TGT_API std::string strReplaceAll(const std::string& str, const std::string& from, const std::string& to);

  /// Returns a copy of \p str where the first occurrence of \p from has been replaced by \p to.
  TGT_API std::string strReplaceFirst(const std::string& str, const std::string& from, const std::string& to);

  /// Returns a copy of \p str where the last occurrence of \p from has been replaced by \p to.
  TGT_API std::string strReplaceLast(const std::string& str, const std::string& from, const std::string& to);

  /**
  * Removes whitespaces from beginning and end of a string.
  *
  * @param str Input string.
  * @param charlist Characters to remove, defaults to space, tab, newline, carriage return, 0, vertical tab.
  */
  TGT_API std::string trim(std::string str, const std::string& charlist = " \t\n\r\0\x0B");

  /// Converts a string to lower case.
  TGT_API std::string toLower(const std::string& str);

  /// Converts a string to upper case.
  TGT_API std::string toUpper(const std::string& str);

  /**
  * Splits a string by the specified delimiter and returns the items in a vector.
  */
  TGT_API std::vector<std::string> strSplit(const std::string& str, char delim);
  /// @overload
  TGT_API std::vector<std::string> strSplit(const std::string& str, const std::string& delim);

  /**
  * Joins a sequence of tokens to a string. The converted tokens
  * are separated by the specified delimiter.
  */
  template<typename T>
  std::string strJoin(const std::vector<T>& tokens, const std::string& delim);

  /// Returns true if \p input ends with \p ending
  TGT_API bool endsWith(const std::string& input, const std::string& ending);

  /// Returns true if \p input starts with \p start
  TGT_API bool startsWith(const std::string& input, const std::string& start);

  /// Returns true if \p input contains \p substr
  TGT_API bool contains(const std::string& input, const std::string& substr);

  /// Formats the passed byte size as string using the appropriate unit (bytes/kB/MB/GB).
  TGT_API std::string formatMemorySize(uint64_t bytes);

  /// Formats the passed time in ms as string using the appropriate unit (min/sec/ms).
  TGT_API std::string formatTime(const size_t time);

  //------------------------------------------------------------------------------

  template<class T>
  std::string genericToString(const T& value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
  }

  template<class T>
  T genericFromString(const std::string& str) throw (Exception) {
    T result;
    std::istringstream stream;
    stream.str(str);
    if ((stream >> result) == 0)
      throw Exception("failed to convert string '" + str + "'");
    return result;
  }

  template<typename T>
  std::string strJoin(const std::vector<T>& tokens, const std::string& delim) {
    if (tokens.empty())
      return "";
    std::stringstream s;
    s << tokens.at(0);
    for (size_t i = 1; i<tokens.size(); i++)
      s << delim << tokens.at(i);
    return s.str();
  }

} // end namespace tgt
