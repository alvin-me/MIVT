#pragma once

#include "transfunc.h"

#include <vector>

namespace tgt {

  class TransFuncAlphaKey;
  class TransFuncColorKey;
  class Volume;

  /**
  * One dimensional, piece-wise linear transfer function based on key values.
  *
  * Internally, it is represented by a one-dimensional RGBA texture of type GL_UNSIGNED_BYTE.
  */
  class TransFunc1D : public TransFunc {
  public:
    /**
    * Constructor
    *
    * @param width desired width of the transfer function
    */
    TGT_API TransFunc1D(int width = 256);

    /**
    * Destructor - deletes the keys of the transfer function
    */
    TGT_API virtual ~TransFunc1D();

    /**
    * Re-generates the texture from the already existing keys.
    * If the texture was empty before the call, a new texture is created automatically.
    */
    TGT_API void updateTexture();

    /**
    * Creates a default function.
    * Generates two keys:
    * One at intensity 0 with the color (0,0,0,0) ;
    * another one at intensity 1 with the color (255,255,255,255)
    */
    TGT_API void setToStandardFunc();

    /**
    * Createa a function cover all domain of the volume.
    * Generates two keys:
    * One at intensity lower of volume with the color (0,0,0,0) ;
    * another one at intensity upper of volume with the color (255,255,255,255)
    * @param
    * @return
    */
    TGT_API void fitVolume(Volume *handle);

    /**
    * Returns the value to which the input value is being mapped.
    * The procedures handles missing keys and out-of-range values gracefully.
    *
    * @param value the intensity value for which the mapping is requested
    * @return the value the input value is mapped to.
    */
    TGT_API glm::col3 getMappingForColorValue(float value) const;

    TGT_API float getMappingForAlphaValue(float value) const;

    /**
    * Returns the number of keys in this transfer function.
    *
    * @return the number of keys
    */
    TGT_API int getNumColorKeys() const;

    TGT_API int getNumAlphaKeys() const;

    /**
    * Returns the key at i-th position. Keys are sorted by their intensities in ascending order.
    * If a value outside of [0, getNumKeys()] is passed, it will be clamped to the appropriate values.
    *
    * @param i the i-th key will be returned
    * @return the pointer to the appropriate key
    */
    TGT_API const TransFuncAlphaKey* getAlphaKey(int i) const;

    TGT_API TransFuncAlphaKey* getAlphaKey(int i);

    TGT_API const TransFuncColorKey* getColorKey(int i) const;

    TGT_API TransFuncColorKey* getColorKey(int i);

    /**
    * Returns all keys. Keys are sorted by their intensities in ascending order.
    *
    * @return a vector containing all the keys
    */
    TGT_API const std::vector<TransFuncAlphaKey*> getAlphaKeys() const;

    TGT_API const std::vector<TransFuncColorKey*> getColorKeys() const;

    /**
    * Replaces the current keys by the passed ones.
    *
    * @param keys the new keys
    */
    TGT_API void setAlphaKeys(std::vector<TransFuncAlphaKey*> keys);

    TGT_API void setColorKeys(std::vector<TransFuncColorKey*> keys);

    /**
    * Adds a key to the property mapping function.
    * It will be automatically inserted into the correct position.
    *
    * @param key the key to be added
    */
    TGT_API void addKey(TransFuncAlphaKey* key);

    TGT_API void addKey(TransFuncColorKey* key);

    /**
    * Updates a key within the property mapping function.
    * Call this method when intensity of a key is changed.
    *
    * @param key the key to be updated
    */
    TGT_API void updateKey();

    /**
    * Removes the given key from the transfer function.
    * Also deletes the passed key.
    *
    * @param key the key that will be removed.
    */
    TGT_API void removeKey(TransFuncAlphaKey* key);

    TGT_API  void removeKey(TransFuncColorKey* key);

    /**
    * Removes all keys from the transfer function.
    */
    TGT_API void clearKeys();

    /**
    * This method returns whether the mapping function is empty.
    * i.e., it contains no mapping keys.
    *
    * @return Is the mapping function empty?
    */
    TGT_API bool isEmpty() const;

    TGT_API  virtual int getNumDimensions() const { return 1; }

    TGT_API virtual glm::vec2 getDataDomain(int dimension = 0) const;
    TGT_API virtual void setDataDomain(glm::vec2 domain, int dimension = 0);

    TGT_API virtual glm::vec2 getWindowingDomain() const;
    TGT_API virtual void setWindowingDomain(glm::vec2 domain);

    TGT_API virtual glm::vec2 getWindowing() const;
    TGT_API virtual void setWindowing(glm::vec2 values);

    TGT_API virtual void reset();

    TGT_API virtual bool save(const std::string& filename) const;
    TGT_API virtual bool load(const std::string& filename);

    TGT_API virtual void serialize(XmlSerializer& s) const;
    TGT_API virtual void deserialize(XmlDeserializer& s);

  private:
    /**
    * Saves transfer function to a XML file. The extension of the file is xml.
    *
    * @param filename the name of the file the transfer function will be saved to
    * @return true, if the operation was successful, false otherwise
    */
    bool saveXml(const std::string& filename) const;

    /**
    * Loads a transfer function from an file with ending tfi.
    *
    * @param filename the name of the file, which should be opened
    * @return true if loading succeeds, false otherwise
    */
    bool loadXml(const std::string& filename);

    std::vector<TransFuncAlphaKey*> alphaKeys_; ///< internal representation of the transfer function as a set of keys
    std::vector<TransFuncColorKey*> colorKeys_;
    glm::vec2 dataDomain_;                      ///< transfer function data domain in real world
    glm::vec2 windowingDomain_;                 ///< windowing domain in real world

    static const std::string loggerCat_; ///< logger category
  };

} // end namespace tgt