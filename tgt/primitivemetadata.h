#pragma once

#include "metadatabase.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"
#include "tgt_string.h"

namespace tgt {

  /**
  * Generic abstract base class for meta data of primitive types.
  */
  template<typename T>
  class PrimitiveMetaDataBase : public MetaDataBase {
  public:
    PrimitiveMetaDataBase() {}
    PrimitiveMetaDataBase(T value);

    virtual std::string toString() const = 0;
    virtual std::string toString(const std::string& /*component*/) const;

    void setValue(T value);
    T getValue() const;

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

  private:
    T value_;
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single boolean value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class BoolMetaData : public PrimitiveMetaDataBase<bool> {
  public:
    TGT_API BoolMetaData() : PrimitiveMetaDataBase<bool>() {}
    TGT_API BoolMetaData(bool value) : PrimitiveMetaDataBase<bool>(value) {}

    TGT_API virtual std::string getClassName() const { return "BoolMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new BoolMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new BoolMetaData(getValue()); }

    TGT_API virtual std::string toString() const {
      if (getValue())
        return "true";
      else
        return "false";
    }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single string value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class StringMetaData : public PrimitiveMetaDataBase<std::string> {
  public:
    TGT_API StringMetaData() : PrimitiveMetaDataBase<std::string>() {}
    TGT_API StringMetaData(std::string value) : PrimitiveMetaDataBase<std::string>(value) {}

    TGT_API virtual std::string getClassName() const { return "StringMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new StringMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new StringMetaData(getValue()); }
    TGT_API virtual std::string toString() const { return getValue(); }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single int value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class IntMetaData : public PrimitiveMetaDataBase<int> {
  public:
    TGT_API IntMetaData() : PrimitiveMetaDataBase<int>() {}
    TGT_API IntMetaData(int value) : PrimitiveMetaDataBase<int>(value) {}

    TGT_API virtual std::string getClassName() const { return "IntMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new IntMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new IntMetaData(getValue()); }
    TGT_API virtual std::string toString() const { return itos(getValue()); }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single size_t value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class SizeTMetaData : public PrimitiveMetaDataBase<size_t> {
  public:
    TGT_API SizeTMetaData() : PrimitiveMetaDataBase<size_t>() {}
    TGT_API SizeTMetaData(size_t value) : PrimitiveMetaDataBase<size_t>(value) {}

    TGT_API virtual std::string getClassName() const { return "SizeTMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new SizeTMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new SizeTMetaData(getValue()); }
    TGT_API virtual std::string toString() const { return itos(getValue()); }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single float value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class FloatMetaData : public PrimitiveMetaDataBase<float> {
  public:
    TGT_API FloatMetaData() : PrimitiveMetaDataBase<float>() {}
    TGT_API FloatMetaData(float value) : PrimitiveMetaDataBase<float>(value) {}

    TGT_API virtual std::string getClassName() const { return "FloatMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new FloatMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new FloatMetaData(getValue()); }
    TGT_API virtual std::string toString() const { return ftos(getValue()); }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a single double value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class DoubleMetaData : public PrimitiveMetaDataBase<double> {
  public:
    TGT_API DoubleMetaData() : PrimitiveMetaDataBase<double>() {}
    TGT_API DoubleMetaData(double value) : PrimitiveMetaDataBase<double>(value) {}

    TGT_API virtual std::string getClassName() const { return "DoubleMetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new DoubleMetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new DoubleMetaData(getValue()); }
    TGT_API virtual std::string toString() const { return dtos(getValue()); }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a vec2 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Vec2MetaData : public PrimitiveMetaDataBase<glm::vec2> {
  public:
    TGT_API Vec2MetaData() : PrimitiveMetaDataBase<glm::vec2>() {}
    TGT_API Vec2MetaData(glm::vec2 value) : PrimitiveMetaDataBase<glm::vec2>(value) {}

    TGT_API virtual std::string getClassName() const { return "Vec2MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Vec2MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Vec2MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return ftos(getValue().x);
      else if (component == "y")
        return ftos(getValue().y);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::ivec2 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class IVec2MetaData : public PrimitiveMetaDataBase<glm::ivec2> {
  public:
    TGT_API IVec2MetaData() : PrimitiveMetaDataBase<glm::ivec2>() {}
    TGT_API IVec2MetaData(glm::ivec2 value) : PrimitiveMetaDataBase<glm::ivec2>(value) {}

    TGT_API virtual std::string getClassName() const { return "IVec2MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new IVec2MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new IVec2MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return itos(getValue().x);
      else if (component == "y")
        return itos(getValue().y);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::vec3 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Vec3MetaData : public PrimitiveMetaDataBase<glm::vec3> {
  public:
    TGT_API Vec3MetaData() : PrimitiveMetaDataBase<glm::vec3>() {}
    TGT_API Vec3MetaData(glm::vec3 value) : PrimitiveMetaDataBase<glm::vec3>(value) {}

    TGT_API virtual std::string getClassName() const { return "Vec3MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Vec3MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Vec3MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return ftos(getValue().x);
      else if (component == "y")
        return ftos(getValue().y);
      else if (component == "z")
        return ftos(getValue().z);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::dvec2 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class DVec2MetaData : public PrimitiveMetaDataBase<glm::dvec2> {
  public:
    TGT_API DVec2MetaData() : PrimitiveMetaDataBase<glm::dvec2>() {}
    TGT_API DVec2MetaData(glm::dvec2 value) : PrimitiveMetaDataBase<glm::dvec2>(value) {}

    TGT_API virtual std::string getClassName() const { return "DVec2MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new DVec2MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new DVec2MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return dtos(getValue().x);
      else if (component == "y")
        return dtos(getValue().y);
      else
        return toString();
    }


  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::dvec3 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class DVec3MetaData : public PrimitiveMetaDataBase<glm::dvec3> {
  public:
    TGT_API DVec3MetaData() : PrimitiveMetaDataBase<glm::dvec3>() {}
    TGT_API DVec3MetaData(glm::dvec3 value) : PrimitiveMetaDataBase<glm::dvec3>(value) {}

    TGT_API virtual std::string getClassName() const { return "DVec3MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new DVec3MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new DVec3MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return dtos(getValue().x);
      else if (component == "y")
        return dtos(getValue().y);
      else if (component == "z")
        return dtos(getValue().z);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::ivec3 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class IVec3MetaData : public PrimitiveMetaDataBase<glm::ivec3> {
  public:
    TGT_API IVec3MetaData() : PrimitiveMetaDataBase<glm::ivec3>() {}
    TGT_API IVec3MetaData(glm::ivec3 value) : PrimitiveMetaDataBase<glm::ivec3>(value) {}

    TGT_API virtual std::string getClassName() const { return "IVec3MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new IVec3MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new IVec3MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return itos(getValue().x);
      else if (component == "y")
        return itos(getValue().y);
      else if (component == "z")
        return itos(getValue().z);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::vec4 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Vec4MetaData : public PrimitiveMetaDataBase<glm::vec4> {
  public:
    TGT_API Vec4MetaData() : PrimitiveMetaDataBase<glm::vec4>() {}
    TGT_API Vec4MetaData(glm::vec4 value) : PrimitiveMetaDataBase<glm::vec4>(value) {}

    TGT_API virtual std::string getClassName() const { return "Vec4MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Vec4MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Vec4MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return ftos(getValue().x);
      else if (component == "y")
        return ftos(getValue().y);
      else if (component == "z")
        return ftos(getValue().z);
      else if (component == "w")
        return ftos(getValue().w);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::ivec4 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class IVec4MetaData : public PrimitiveMetaDataBase<glm::ivec4> {
  public:
    TGT_API IVec4MetaData() : PrimitiveMetaDataBase<glm::ivec4>() {}
    TGT_API IVec4MetaData(glm::ivec4 value) : PrimitiveMetaDataBase<glm::ivec4>(value) {}

    TGT_API virtual std::string getClassName() const { return "IVec4MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new IVec4MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new IVec4MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return itos(getValue().x);
      else if (component == "y")
        return itos(getValue().y);
      else if (component == "z")
        return itos(getValue().z);
      else if (component == "w")
        return itos(getValue().w);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::dvec4 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class DVec4MetaData : public PrimitiveMetaDataBase<glm::dvec4> {
  public:
    TGT_API DVec4MetaData() : PrimitiveMetaDataBase<glm::dvec4>() {}
    TGT_API DVec4MetaData(glm::dvec4 value) : PrimitiveMetaDataBase<glm::dvec4>(value) {}

    TGT_API virtual std::string getClassName() const { return "DVec4MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new DVec4MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new DVec4MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

    TGT_API virtual std::string toString(const std::string& component) const {
      if (component == "x")
        return dtos(getValue().x);
      else if (component == "y")
        return dtos(getValue().y);
      else if (component == "z")
        return dtos(getValue().z);
      else if (component == "w")
        return dtos(getValue().w);
      else
        return toString();
    }
  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::mat2 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Mat2MetaData : public PrimitiveMetaDataBase<glm::mat2> {
  public:
    TGT_API Mat2MetaData() : PrimitiveMetaDataBase<glm::mat2>() {}
    TGT_API Mat2MetaData(glm::mat2 value) : PrimitiveMetaDataBase<glm::mat2>(value) {}

    TGT_API virtual std::string getClassName() const { return "Mat2MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Mat2MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Mat2MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::mat3 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Mat3MetaData : public PrimitiveMetaDataBase<glm::mat3> {
  public:
    TGT_API Mat3MetaData() : PrimitiveMetaDataBase<glm::mat3>() {}
    TGT_API Mat3MetaData(glm::mat3 value) : PrimitiveMetaDataBase<glm::mat3>(value) {}

    TGT_API virtual std::string getClassName() const { return "Mat3MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Mat3MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Mat3MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }

  };

  //---------------------------------------------------------------------------------------

  /**
  * Holds a glm::mat4 value to be stored as serialization meta data.
  *
  * @see MetaDataBase
  */
  class Mat4MetaData : public PrimitiveMetaDataBase<glm::mat4> {
  public:
    TGT_API Mat4MetaData() : PrimitiveMetaDataBase<glm::mat4>() {}
    TGT_API Mat4MetaData(glm::mat4 value) : PrimitiveMetaDataBase<glm::mat4>(value) {}

    TGT_API virtual std::string getClassName() const { return "Mat4MetaData"; }
    TGT_API virtual MetaDataBase* create() const     { return new Mat4MetaData(); }
    TGT_API virtual MetaDataBase* clone() const      { return new Mat4MetaData(getValue()); }
    TGT_API virtual std::string toString() const {
      std::stringstream s;
      s << glm::to_string(getValue());
      return s.str();
    }
  };

  //----------------------------------------------------------------------------------------

  template <typename T>
  PrimitiveMetaDataBase<T>::PrimitiveMetaDataBase(T value) : value_(value) {}

  template <typename T>
  void PrimitiveMetaDataBase<T>::setValue(T value) {
    value_ = value;
  }

  template <typename T>
  T PrimitiveMetaDataBase<T>::getValue() const {
    return value_;
  }

  template<typename T>
  std::string PrimitiveMetaDataBase<T>::toString(const std::string& /*component*/) const {
    return toString();
  }

  template <typename T>
  void PrimitiveMetaDataBase<T>::serialize(XmlSerializer& s) const {
    s.serialize("value", value_);
  }

  template <typename T>
  void PrimitiveMetaDataBase<T>::deserialize(XmlDeserializer& s) {
    s.deserialize("value", value_);
  }

} // end namespace tgt