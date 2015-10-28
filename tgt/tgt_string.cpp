#include "tgt_string.h"
#include "tgt_math.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#pragma warning(disable:4996)

namespace tgt {

  std::string itos(int i, int stringLength /*= -1*/, char fillChar /*= '0'*/) {
    std::stringstream s;
    if (stringLength > 0)
      s << std::setw(stringLength) << std::setfill(fillChar) << i;
    else
      s << i;
    return s.str();
  }

  std::string itos(uint32_t i, int stringLength /*= -1*/, char fillChar /*= '0'*/) {
    std::stringstream s;
    if (stringLength > 0)
      s << std::setw(stringLength) << std::setfill(fillChar) << i;
    else
      s << i;
    return s.str();
  }

  std::string itos(uint64_t i, int stringLength /*= -1*/, char fillChar /*= '0'*/) {
    std::stringstream s;
    if (stringLength > 0)
      s << std::setw(stringLength) << std::setfill(fillChar) << i;
    else
      s << i;
    return s.str();
  }

  std::string ftos(float f, int precision /* =-1*/) {
    char buffer[50];
    if (precision > -1)
      sprintf(buffer, "%.*f", precision, f);
    else
      sprintf(buffer, "%f", f);
    return std::string(buffer);
  }

  std::string dtos(double d, int precision /* = -1*/) {
    char buffer[50];
    if (precision > -1)
      sprintf(buffer, "%.*f", precision, d);
    else
      sprintf(buffer, "%f", d);
    return std::string(buffer);
  }

  char* strToChr(const std::string& s){
    char* result = new char[s.size() + 1];
    s.copy(result, s.size());
    result[s.size()] = '\0';
    return result;
  }

  std::string strReplaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string strConv(str);
    size_t lookHere = 0;
    size_t foundHere;
    while ((foundHere = strConv.find(from, lookHere)) != std::string::npos) {
      strConv.replace(foundHere, from.size(), to);
      lookHere = foundHere + to.size();
    }
    return strConv;
  }

  std::string strReplaceFirst(const std::string& str, const std::string& from, const std::string& to) {
    std::string strConv(str);
    size_t foundHere;
    if ((foundHere = strConv.find(from)) != std::string::npos) {
      strConv.replace(foundHere, from.size(), to);
    }
    return strConv;
  }

  std::string strReplaceLast(const std::string& str, const std::string& from, const std::string& to) {
    std::string strConv(str);
    size_t foundHere;
    if ((foundHere = strConv.rfind(from)) != std::string::npos) {
      strConv.replace(foundHere, from.size(), to);
    }
    return strConv;
  }

  std::string trim(std::string str, const std::string& charlist) {
    //trim from beginning:
    while (!str.empty()) {
      char curChar = str[0];

      if (charlist.find(curChar) != std::string::npos)
        str.erase(0, 1); //delete first character
      else
        break;
    }

    //trim from end:
    while (!str.empty()) {
      char curChar = str[str.size() - 1];

      if (charlist.find(curChar) != std::string::npos)
        str.erase(str.size() - 1, 1); //delete last character
      else
        break;
    }

    return str;
  }

  std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

  std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
  }

  std::vector<std::string> strSplit(const std::string& str, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  }

  std::vector<std::string> strSplit(const std::string& str, const std::string& delim) {
    std::vector<std::string> result;
    if (delim.length() == 0 || str.length() == 0) {
      result.push_back(str);
      return result;
    }

    size_t pos = 0;
    while (pos != std::string::npos) {
      size_t last = pos;
      pos = str.find(delim, last);
      result.push_back(str.substr(last, pos - last));
      if (pos != std::string::npos)
        pos += delim.size();
    }
    return result;
  }

  bool endsWith(const std::string& input, const std::string& ending) {
    size_t li = input.length();
    size_t le = ending.length();
    if (li >= le) {
      for (size_t i = 0; i<le; i++) {
        if (ending[i] != input[li - le + i])
          return false;
      }
      return true;
    }
    else
      return false;
  }

  bool startsWith(const std::string& input, const std::string& start) {
    size_t li = input.length();
    size_t ls = start.length();
    if (li >= ls) {
      for (size_t i = 0; i<ls; i++) {
        if (start[i] != input[i])
          return false;
      }
      return true;
    }
    else
      return false;
  }

  bool contains(const std::string& input, const std::string& substr) {
    return (input.find(substr) != std::string::npos);
  }

  std::string formatMemorySize(uint64_t bytes) {
    // calculate GB/MB/kB with 0.1f precision
    float gb = glm::round(static_cast<float>(bytes) / 107374182.4f) / 10.f;
    float mb = glm::round(static_cast<float>(bytes) / 104857.6f) / 10.f;
    float kb = glm::round(static_cast<float>(bytes) / 102.4f) / 10.f;

    // select appropriate unit
    std::stringstream out;
    if (gb >= 1.0f)
      out << gb << " GB";
    else if (mb >= 1.0f)
      out << mb << " MB";
    else if (kb >= 1.0f)
      out << kb << " kB";
    else
      out << bytes << " bytes";

    return out.str();
  }

  std::string formatTime(const size_t time) {
    // calculate min/sec/ms
    size_t min = time / 60000; // 1000ms*60sec
    size_t sec = (time - min * 60000) / 1000;
    size_t ms = time % 1000;

    // select appropriate unit
    std::stringstream out;
    if (min >= 1) {
      out << min << ":" << itos(sec, 2) << "." << itos(ms, 3) << " min";
    }
    else if (sec >= 1)
      out << sec << "." << itos(ms, 3) << " sec";
    else
      out << ms << " ms";

    return out.str();
  }

} // end namespace tgt
