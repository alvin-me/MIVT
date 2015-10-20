#pragma once

#include <limits>
#include <stdint.h>
#include <string>

namespace tgt {

  /**
  * Used internally by VolumeElement.
  */
  template<class T>
  T getTypeUpperLimit() {
    return std::numeric_limits<T>::max();
  }

  template<class T>
  T getTypeLowerLimit() {
    return std::numeric_limits<T>::min();
  }

  template<class T>
  bool isTypeSigned() {
    return std::numeric_limits<T>::is_signed;
  }

  template<class T>
  bool isTypeInteger() {
    return std::numeric_limits<T>::is_integer;
  }

  inline uint8_t swapEndian(uint8_t value) {
    return value;
  }
  inline int8_t swapEndian(int8_t value) {
    return value;
  }

  inline uint16_t swapEndian(uint16_t value) {
    return (value >> 8) | (value << 8);
  }
  inline int16_t swapEndian(int16_t value) {
    return (value >> 8) | (value << 8);
  }

  inline uint32_t swapEndian(uint32_t value) {
    return (value >> 24) |
      ((value << 8) & 0x00FF0000) |
      ((value >> 8) & 0x0000FF00) |
      (value << 24);
  }

  inline int32_t swapEndian(int32_t value) {
    return (value >> 24) |
      ((value << 8) & 0x00FF0000) |
      ((value >> 8) & 0x0000FF00) |
      (value << 24);
  }

  inline uint64_t swapEndian(uint64_t value) {
    return (value >> 56) |
      ((value << 40) & 0x00FF000000000000) |
      ((value << 24) & 0x0000FF0000000000) |
      ((value << 8) & 0x000000FF00000000) |
      ((value >> 8) & 0x00000000FF000000) |
      ((value >> 24) & 0x0000000000FF0000) |
      ((value >> 40) & 0x000000000000FF00) |
      (value << 56);
  }

  inline int64_t swapEndian(int64_t value) {
    return (value >> 56) |
      ((value << 40) & 0x00FF000000000000) |
      ((value << 24) & 0x0000FF0000000000) |
      ((value << 8) & 0x000000FF00000000) |
      ((value >> 8) & 0x00000000FF000000) |
      ((value >> 24) & 0x0000000000FF0000) |
      ((value >> 40) & 0x000000000000FF00) |
      (value << 56);
  }

  inline float swapEndian(float value) {
    float result;
    char* src = (char*)&value;
    char* dst = (char*)&result;
    dst[0] = src[3];
    dst[1] = src[2];
    dst[2] = src[1];
    dst[3] = src[0];
    return result;
  }

  inline double swapEndian(double value) {
    double result;
    char* src = (char*)&value;
    char* dst = (char*)&result;
    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];
    return result;
  }

  inline std::string getTypeFormat(uint8_t) {
    return "uint8";
  }

  inline std::string getTypeFormat(uint16_t) {
    return "uint16";
  }

  inline std::string getTypeFormat(uint32_t) {
    return "uint32";
  }

  inline std::string getTypeFormat(uint64_t) {
    return "uint64";
  }

  inline std::string getTypeFormat(int8_t) {
    return "int8";
  }

  inline std::string getTypeFormat(int16_t) {
    return "int16";
  }

  inline std::string getTypeFormat(int32_t) {
    return "int32";
  }

  inline std::string getTypeFormat(int64_t) {
    return "int64";
  }

  inline std::string getTypeFormat(float) {
    return "float";
  }

  inline std::string getTypeFormat(double) {
    return "double";
  }

  //------------------------------------------------------------------------------

  /**
  * Helper class for getting sth like the maximum value
  */
  template<class T>
  class VolumeElement {
  public:
    inline static T getZero();

    /**
    * Returns data format as string.
    */
    inline static std::string getFormat();

    /**
    * Returns whether the data type is signed.
    */
    inline bool static isSigned();

    /**
    * Returns whether the data type is bounded,
    * which is true for integer types and false for
    * floating point types.
    */
    inline bool static isInteger();

    /**
    * Returns the maximal possible value of T
    */
    inline static T rangeMax();

    /**
    * Returns the minimal possible value of T
    */
    inline static T rangeMin();

    /**
    * Returns the maximum value an element can have. In contrast to max() no
    * vector is returned but the maximum value an element in the vector can have.
    * Since we cannot make any assumption about the element's data type,
    * we have to use float as fall back.
    */
    inline static float rangeMaxElement();

    /**
    * Returns the minimum value an element can have.
    */
    inline static float rangeMinElement();

    inline static T swapEndianness(T v);

    /**
    * type -> float
    */

    inline static float getTypeAsFloat(float value) {
      return value;
    }

    inline static float getTypeAsFloat(double value) {
      return static_cast<float>(value);
    }

    inline static float getTypeAsFloat(uint8_t value) {
      return static_cast<float>(value) / VolumeElement<uint8_t>::rangeMaxElement();
    }

    inline static float getTypeAsFloat(int8_t value) {
      if (value >= 0)
        return static_cast<float>(value) / VolumeElement<int8_t>::rangeMaxElement();
      else
        return static_cast<float>(value) / -VolumeElement<int8_t>::rangeMinElement();
    }

    inline static float getTypeAsFloat(uint16_t value) {
      return static_cast<float>(value) / VolumeElement<uint16_t>::rangeMaxElement();
    }

    inline static float getTypeAsFloat(int16_t value) {
      if (value >= 0)
        return static_cast<float>(value) / VolumeElement<int16_t>::rangeMaxElement();
      else
        return static_cast<float>(value) / -VolumeElement<int16_t>::rangeMinElement();
    }

    inline static float getTypeAsFloat(uint32_t value) {
      return static_cast<float>(value) / VolumeElement<uint32_t>::rangeMaxElement();
    }

    inline static float getTypeAsFloat(int32_t value) {
      if (value >= 0)
        return static_cast<float>(value) / VolumeElement<int32_t>::rangeMaxElement();
      else
        return static_cast<float>(value) / -VolumeElement<int32_t>::rangeMinElement();
    }

    inline static float getTypeAsFloat(uint64_t value) {
      return static_cast<float>(value) / VolumeElement<uint64_t>::rangeMaxElement();
    }

    inline static float getTypeAsFloat(int64_t value) {
      if (value >= 0)
        return static_cast<float>(value) / VolumeElement<int64_t>::rangeMaxElement();
      else
        return static_cast<float>(value) / -VolumeElement<int64_t>::rangeMinElement();
    }


  };

  //------------------------------------------------------------------------------

  template<class T>
  std::string VolumeElement<T>::getFormat() {
    T tmp = T(1);
    return getTypeFormat(tmp);
  }

  template<class T>
  inline T VolumeElement<T>::getZero() {
    return T(0);
  }

  template<class T>
  inline bool VolumeElement<T>::isSigned() {
    return isTypeSigned<T>();
  }

  template<class T>
  inline bool VolumeElement<T>::isInteger() {
    return isTypeInteger<T>();
  }

  template<class T>
  inline T VolumeElement<T>::rangeMax() {
    return static_cast<T>(getTypeUpperLimit<T>());
  }

  template<class T>
  inline T VolumeElement<T>::rangeMin() {
    return static_cast<T>(getTypeLowerLimit<T>());
  }

  template<class T>
  inline float VolumeElement<T>::rangeMaxElement() {
    return static_cast<float>(getTypeUpperLimit<T>());
  }

  template<class T>
  inline float VolumeElement<T>::rangeMinElement() {
    return static_cast<float>(getTypeLowerLimit<T>());
  }

  template<class T>
  inline T VolumeElement<T>::swapEndianness(T v) {
    return swapEndian(v);
  }

} // end namespace tgt