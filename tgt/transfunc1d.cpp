#include "transfunc1d.h"
#include "transfuncmappingkey.h"
#include "logmanager.h"
#include "volume.h"
#include "volumeminmax.h"
#include "xmlserializer.h"
#include "xmldeserializer.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace tgt {

  const std::string TransFunc1D::loggerCat_("TransFunc1D");

  TransFunc1D::TransFunc1D(int width)
    : TransFunc(width, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, Texture::NEAREST)
    , dataDomain_(0.f, 255.f)
    , windowingDomain_(0.f, 255.f)
  {
    setToStandardFunc();
  }

  TransFunc1D::~TransFunc1D() {
    for (size_t i = 0; i < alphaKeys_.size(); ++i)
      delete alphaKeys_[i];
    for (size_t i = 0; i < colorKeys_.size(); ++i)
      delete colorKeys_[i];
  }

  void TransFunc1D::setToStandardFunc() {
    clearKeys();
    colorKeys_.push_back(new TransFuncColorKey(dataDomain_.x, glm::col3(0)));
    colorKeys_.push_back(new TransFuncColorKey(dataDomain_.y, glm::col3(255)));
    alphaKeys_.push_back(new TransFuncAlphaKey(dataDomain_.x, 0.f));
    alphaKeys_.push_back(new TransFuncAlphaKey(dataDomain_.y, 1.f));
    setGammaValue(1.f);

    invalidateTexture();
  }

  void TransFunc1D::fitVolume(Volume *handle) {
    if (handle->getDerivedData<VolumeMinMax>()) {
      setDataDomain(glm::vec2(handle->getDerivedData<VolumeMinMax>()->getMinHu(),
        handle->getDerivedData<VolumeMinMax>()->getMaxHu()));
    }

    if (handle->getWindowWidth() > 0) {
      setWindowingDomain(glm::vec2(handle->getWindowCenter() - 0.5f * handle->getWindowWidth(),
        handle->getWindowCenter() + 0.5f * handle->getWindowWidth()));
    }
    else {
      setWindowingDomain(getDataDomain());
    }

    clearKeys();
    colorKeys_.push_back(new TransFuncColorKey(dataDomain_.x, glm::col3(0)));
    colorKeys_.push_back(new TransFuncColorKey(dataDomain_.y, glm::col3(255)));
    alphaKeys_.push_back(new TransFuncAlphaKey(dataDomain_.x, 0.f));
    alphaKeys_.push_back(new TransFuncAlphaKey(dataDomain_.y, 1.f));
    setGammaValue(1.f);

    invalidateTexture();
  }

  glm::col3 TransFunc1D::getMappingForColorValue(float value) const {
    // If there are no keys, any further calculation is meaningless
    if (colorKeys_.empty())
      return glm::col3(0, 0, 0);

    // Restrict value to domain
    //value = (value < windowingDomain_.x) ? windowingDomain_.x : value;
    //value = (value > windowingDomain_.y) ? windowingDomain_.y : value;
    if (value < windowingDomain_.x || value > windowingDomain_.y) {
      return glm::col3(0);
    }

    //use gamma correction
    if (gammaValue_ != 1.f) {
      value = powf(value, gammaValue_);
    }

    // iterate through all keys until we get to the correct position
    std::vector<TransFuncColorKey*>::const_iterator keyIterator = colorKeys_.begin();

    while ((keyIterator != colorKeys_.end()) && (value > (*keyIterator)->getIntensity()))
      keyIterator++;

    glm::col3 color;
    if (keyIterator == colorKeys_.begin())
      color = colorKeys_[0]->getColor();
    else if (keyIterator == colorKeys_.end())
      color = (*(keyIterator - 1))->getColor();
    else{
      // calculate the value weighted by the destination to the next left and right key
      TransFuncColorKey* leftKey = *(keyIterator - 1);
      TransFuncColorKey* rightKey = *keyIterator;
      float fraction = (value - leftKey->getIntensity()) / (rightKey->getIntensity() - leftKey->getIntensity());
      glm::col3 leftDest = leftKey->getColor();
      glm::col3 rightDest = rightKey->getColor();
      color = leftDest;
      color.r += static_cast<uint8_t>((rightDest.r - leftDest.r) * fraction);
      color.g += static_cast<uint8_t>((rightDest.g - leftDest.g) * fraction);
      color.b += static_cast<uint8_t>((rightDest.b - leftDest.b) * fraction);
    }

    return color;
  }

  float TransFunc1D::getMappingForAlphaValue(float value) const {
    // If there are no keys, any further calculation is meaningless
    if (alphaKeys_.empty())
      return 0;

    // Restrict value to domain
    //value = (value < windowingDomain_.x) ? windowingDomain_.x : value;
    //value = (value > windowingDomain_.y) ? windowingDomain_.y : value;
    if (value < windowingDomain_.x || value > windowingDomain_.y) {
      return 0;
    }

    //use gamma correction
    if (gammaValue_ != 1.f) {
      value = powf(value, gammaValue_);
    }

    // iterate through all keys until we get to the correct position
    std::vector<TransFuncAlphaKey*>::const_iterator keyIterator = alphaKeys_.begin();

    while ((keyIterator != alphaKeys_.end()) && (value > (*keyIterator)->getIntensity()))
      keyIterator++;

    float alpha;
    if (keyIterator == alphaKeys_.begin())
      alpha = alphaKeys_[0]->getAlpha();
    else if (keyIterator == alphaKeys_.end())
      alpha = (*(keyIterator - 1))->getAlpha();
    else{
      // calculate the value weighted by the destination to the next left and right key
      TransFuncAlphaKey* leftKey = *(keyIterator - 1);
      TransFuncAlphaKey* rightKey = *keyIterator;
      float fraction = (value - leftKey->getIntensity()) / (rightKey->getIntensity() - leftKey->getIntensity());
      float leftDest = leftKey->getAlpha();
      float rightDest = rightKey->getAlpha();
      alpha = leftDest + (rightDest - leftDest) * fraction;
    }

    return alpha;
  }

  void TransFunc1D::updateTexture() {

    if (!tex_ || (tex_->getDimensions() != dimensions_))
      createTex();
    assert(tex_);

    for (int x = 0; x < dimensions_.x; ++ x) {
      float value = (float)x / (dimensions_.x - 1) * (windowingDomain_.y - windowingDomain_.x) + windowingDomain_.x;
      glm::col3 color = getMappingForColorValue(value);
      float alpha = getMappingForAlphaValue(value);
      tex_->texel<glm::col4>(x) = glm::col4(color, static_cast<uint8_t>(alpha * 255.f));
    }

    tex_->uploadTexture();
    LGL_ERROR;

    textureInvalid_ = false;
  }

  void TransFunc1D::setDataDomain(glm::vec2 domain, int dimension) {
    assert(dimension == 0);
    if (dimension == 0)
      dataDomain_ = domain;
  }

  glm::vec2 TransFunc1D::getDataDomain(int dimension) const {
    assert(dimension == 0);
    if (dimension == 0)
      return dataDomain_;
    else
      return glm::vec2(0.0f);
  }

  glm::vec2 TransFunc1D::getWindowingDomain() const {
    return windowingDomain_;
  }

  void TransFunc1D::setWindowingDomain(glm::vec2 domain) {
    windowingDomain_ = domain;
  }

  glm::vec2 TransFunc1D::getWindowing() const {
    glm::vec2 values;
    values.x = windowingDomain_.y - windowingDomain_.x;
    values.y = windowingDomain_.x + values.x * 0.5f;
    return values;
  }

  void TransFunc1D::setWindowing(glm::vec2 values) {
    windowingDomain_.x = values.y - values.x * 0.5f;
    windowingDomain_.y = values.y + values.x * 0.5f;
  }

  int TransFunc1D::getNumColorKeys() const {
    return static_cast<int>(colorKeys_.size());
  }

  int TransFunc1D::getNumAlphaKeys() const {
    return static_cast<int>(alphaKeys_.size());
  }

  const TransFuncAlphaKey* TransFunc1D::getAlphaKey(int i) const {
    return alphaKeys_.at(i);
  }

  TransFuncAlphaKey* TransFunc1D::getAlphaKey(int i) {
    return alphaKeys_.at(i);
  }

  const std::vector<TransFuncAlphaKey*> TransFunc1D::getAlphaKeys() const {
    return alphaKeys_;
  }

  const TransFuncColorKey* TransFunc1D::getColorKey(int i) const {
    return colorKeys_.at(i);
  }

  TransFuncColorKey* TransFunc1D::getColorKey(int i) {
    return colorKeys_.at(i);
  }

  const std::vector<TransFuncColorKey*> TransFunc1D::getColorKeys() const {
    return colorKeys_;
  }

  void TransFunc1D::setAlphaKeys(std::vector<TransFuncAlphaKey*> keys) {
    alphaKeys_ = keys;
    invalidateTexture();
  }

  void TransFunc1D::setColorKeys(std::vector<TransFuncColorKey*> keys) {
    colorKeys_ = keys;
    invalidateTexture();
  }

  void TransFunc1D::reset(){
    setToStandardFunc();
  }

  void TransFunc1D::addKey(TransFuncAlphaKey* key) {
    // insert key at appropriate location
    if (alphaKeys_.empty()) {
      alphaKeys_.push_back(key);
      return;
    }
    std::vector<TransFuncAlphaKey *>::iterator keyIterator = alphaKeys_.begin();
    // Fast-forward to the correct position
    while ((keyIterator != alphaKeys_.end()) && (key->getIntensity() > (*keyIterator)->getIntensity()))
      keyIterator++;
    alphaKeys_.insert(keyIterator, key);

    invalidateTexture();
  }

  void TransFunc1D::addKey(TransFuncColorKey* key) {
    // insert key at appropriate location
    if (colorKeys_.empty()) {
      colorKeys_.push_back(key);
      return;
    }
    std::vector<TransFuncColorKey *>::iterator keyIterator = colorKeys_.begin();
    // Fast-forward to the correct position
    while ((keyIterator != colorKeys_.end()) && (key->getIntensity() > (*keyIterator)->getIntensity()))
      keyIterator++;
    colorKeys_.insert(keyIterator, key);

    invalidateTexture();
  }

  bool sortAlphaFunction(TransFuncAlphaKey* a, TransFuncAlphaKey* b) {
    return a->getIntensity() < b->getIntensity();
  }

  bool sortColorFunction(TransFuncColorKey* a, TransFuncColorKey* b) {
    return a->getIntensity() < b->getIntensity();
  }

  void TransFunc1D::updateKey() {
    std::sort(alphaKeys_.begin(), alphaKeys_.end(), sortAlphaFunction);
    std::sort(colorKeys_.begin(), colorKeys_.end(), sortColorFunction);

    invalidateTexture();
  }

  void TransFunc1D::removeKey(TransFuncAlphaKey* key) {
    std::vector<TransFuncAlphaKey *>::iterator keyIterator = find(alphaKeys_.begin(), alphaKeys_.end(), key);
    if (keyIterator != alphaKeys_.end())
      alphaKeys_.erase(keyIterator);
    delete key;

    invalidateTexture();
  }

  void TransFunc1D::removeKey(TransFuncColorKey* key) {
    std::vector<TransFuncColorKey *>::iterator keyIterator = find(colorKeys_.begin(), colorKeys_.end(), key);
    if (keyIterator != colorKeys_.end())
      colorKeys_.erase(keyIterator);
    delete key;

    invalidateTexture();
  }

  void TransFunc1D::clearKeys() {
    std::vector<TransFuncAlphaKey *>::iterator alphaKeyIterator = alphaKeys_.begin();
    // First delete all the referenced objects in the heap
    while (alphaKeyIterator != alphaKeys_.end()) {
      delete (*alphaKeyIterator);
      ++alphaKeyIterator;
    }
    // then delete the entries in the vector
    alphaKeys_.clear();

    std::vector<TransFuncColorKey *>::iterator colorKeyIterator = colorKeys_.begin();
    // First delete all the referenced objects in the heap
    while (colorKeyIterator != colorKeys_.end()) {
      delete (*colorKeyIterator);
      ++colorKeyIterator;
    }
    // then delete the entries in the vector
    colorKeys_.clear();

    invalidateTexture();
  }

  bool TransFunc1D::isEmpty() const {
    return alphaKeys_.empty() || colorKeys_.empty();
  }

  bool TransFunc1D::save(const std::string& filename) const {
    //look for fileExtension
    std::string fileExtension;
    size_t dotPosition = filename.rfind(".");
    if (dotPosition == std::string::npos)
      return false;
    else
      fileExtension = filename.substr(dotPosition + 1);

    if (fileExtension == "xml")
      return saveXml(filename);
    return false;
  }

  bool TransFunc1D::load(const std::string& filename) {
    // Extract the file extension
    std::string fileExtension;
    size_t dotPosition = filename.rfind(".");
    if (dotPosition != std::string::npos)
      // => the last (seperating) dot was found
      fileExtension = filename.substr(dotPosition + 1);
    else
      return false;

    if (fileExtension == "xml")
      return loadXml(filename);
    return false;
  }

  bool TransFunc1D::saveXml(const std::string& filename) const {

    // open file stream
    std::ofstream stream(filename.c_str(), std::ios_base::out);
    if (stream.fail()) {
      LWARNING("Unable to open file " << filename << " for writing.");
      return false;
    }

    // serialize to stream
    bool success = true;
    try {
      XmlSerializer s;
      s.serialize("TransFunc1D", this);
    
      s.write(stream);
      if (stream.bad()) {
        LWARNING("Unable to write to file: " << filename);
        success = false;
      }
      stream.close();
    }
    catch (SerializationException &e) {
      LWARNING("SerializationException: " << e.what());
      stream.close();
      success = false;
    }

    // log result
    if (success)
      LINFO("Saved transfer function to file: " << filename);
    else
      LWARNING("Saving transfer function failed.");

    return success;
  }

  bool TransFunc1D::loadXml(const std::string& filename) {
    // open file stream
    std::ifstream stream(filename.c_str(), std::ios_base::in);
    if (stream.fail()) {
      LWARNING("Unable to open file " << filename << " for reading.");
      return false;
    }

    // deserialize from stream
    bool success = true;
    try {
      XmlDeserializer d;
      d.read(stream);
      d.deserialize("TransFunc1D", *this);
      stream.close();
    }
    catch (SerializationException &e) {
      LWARNING("SerializationException: " << e.what());
      stream.close();
      success = false;
    }

    // log result
    if (success)
      LDEBUG("Loaded transfer function from file: " << filename);
    else
      LWARNING("Loading transfer function failed.");

    invalidateTexture();
    return success;
  }

  void TransFunc1D::serialize(XmlSerializer& s) const {
    TransFunc::serialize(s);
    s.serialize("domain", windowingDomain_);
    s.serialize("ColorKeys", colorKeys_, "key");
    s.serialize("AlphaKeys", alphaKeys_, "key");
  }

  void TransFunc1D::deserialize(XmlDeserializer& s) {
    TransFunc::deserialize(s);
    s.deserialize("domain", windowingDomain_);
    s.deserialize("ColorKeys", colorKeys_, "key");
    s.deserialize("AlphaKeys", alphaKeys_, "key");
  }

} // end namespace tgt
