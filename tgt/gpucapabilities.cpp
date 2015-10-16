#include "gpucapabilities.h"
#include "tgt_gl.h"
#include "logmanager.h"

using std::string;

namespace tgt {

  const std::string GpuCapabilities::loggerCat_("GpuCapabilities");

  GpuCapabilities::GpuCapabilities(bool detectCaps) {
    if (detectCaps) {
      detectCapabilities();
    }
  }

  GpuCapabilities::~GpuCapabilities() {
  }

  GpuCapabilities::GlVersion GpuCapabilities::getGlVersion() {
    return glVersion_;
  }

  bool GpuCapabilities::isOpenGlVersionSupported(GpuCapabilities::GlVersion version) {
    return (glVersion_ >= version);
  }

  GpuCapabilities::GpuVendor GpuCapabilities::getVendor() {
    return vendor_;
  }

  std::string GpuCapabilities::getVendorAsString() {
    switch (vendor_) {
    case GPU_VENDOR_NVIDIA:
      return "NVIDIA";
    case GPU_VENDOR_ATI:
      return "ATI";
    case GPU_VENDOR_INTEL:
      return "Intel";
    default:
      return "unknown";
    }
  }

  bool GpuCapabilities::isExtensionSupported(string extension) const {
    return (glExtensionsString_.find(extension) != string::npos);
  }

  string GpuCapabilities::getGlVersionString() {
    return glVersionString_;
  }

  string GpuCapabilities::getGlVendorString() {
    return glVendorString_;
  }

  string GpuCapabilities::getGlRendererString() {
    return glRendererString_;
  }

  string GpuCapabilities::getShadingLanguageVersionString() {
    return glslVersionString_;
  }

  string GpuCapabilities::getGlExtensionsString() {
    return glExtensionsString_;
  }

  bool GpuCapabilities::areShadersSupported() {
    return shaderSupport_;
  }

  bool GpuCapabilities::areShadersSupportedARB() {
    return shaderSupportARB_;
  }

  GpuCapabilities::GlVersion GpuCapabilities::getShaderVersion() {
    return shaderVersion_;
  }

  GpuCapabilities::ShaderModel GpuCapabilities::getShaderModel() {
    return shaderModel_;
  }

  bool GpuCapabilities::isShaderModelSupported(GpuCapabilities::ShaderModel shaderModel) {
    return (shaderModel_ >= shaderModel);
  }

  int GpuCapabilities::getMaxTextureSize() {
    return maxTexSize_;
  }

  bool GpuCapabilities::is3DTexturingSupported() {
    return texturing3D_;
  }

  int GpuCapabilities::getMax3DTextureSize() {
    return max3DTexSize_;
  }

  int GpuCapabilities::retrieveAvailableTextureMemory() const {
    int availableTexMem = -1;

    if (vendor_ == GPU_VENDOR_NVIDIA) {
      if (isExtensionSupported("GL_NVX_gpu_memory_info")){
#ifndef GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX
#define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#endif
        GLint retVal;
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &retVal);
        availableTexMem = static_cast<int>(retVal);
      }
      else {
        LDEBUG("No GL_NVX_gpu_memory_info support!!!");
      }
    }
    else if (vendor_ == GPU_VENDOR_ATI) {
      if (isExtensionSupported("GL_ATI_meminfo")) {
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#endif
        GLint retVal[4];
        glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, retVal);
        availableTexMem = static_cast<int>(retVal[1]); //0=total 1=availabel
      }
      else {
        LDEBUG("No GL_ATI_meminfo support");
      }
    }

    return availableTexMem;
  }

  int GpuCapabilities::retrieveTotalTextureMemory() const {
    int availableTexMem = -1;

    if (vendor_ == GPU_VENDOR_NVIDIA) {
      if (isExtensionSupported("GL_NVX_gpu_memory_info")){
#ifndef GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX
#define GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX 0x9047
#endif
        GLint retVal;
        glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &retVal);
        availableTexMem = static_cast<int>(retVal);
      }
      else {
        LDEBUG("No GL_NVX_gpu_memory_info support!!!");
      }
    }
    else if (vendor_ == GPU_VENDOR_ATI) {
      if (isExtensionSupported("GL_ATI_meminfo")) {
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#endif
        GLint retVal[4];
        glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, retVal);
        availableTexMem = static_cast<int>(retVal[0]); //0=total 1=availabel
      }
      else {
        LDEBUG("No GL_ATI_meminfo support");
      }
    }

    return availableTexMem;
  }

  int GpuCapabilities::getNumTextureUnits() {
    return numTextureUnits_;
  }

  bool GpuCapabilities::isNpotSupported() {
    return npotTextures_;
  }

  bool GpuCapabilities::areTextureRectanglesSupported() {
    return textureRectangles_;
  }

  bool GpuCapabilities::isAnisotropicFilteringSupported() {
    return anisotropicFiltering_;
  }

  float GpuCapabilities::getMaxTextureAnisotropy() {
    return maxTextureAnisotropy_;
  }

  int GpuCapabilities::getMaxGeometryShaderVertices() {
    return maxGeometryShaderVertices_;
  }

  bool GpuCapabilities::isTextureCompressionSupported() {
    return textureCompression_;
  }

  bool GpuCapabilities::areFramebufferObjectsSupported() {
    return framebufferObjects_;
  }

  int GpuCapabilities::getMaxColorAttachments() {
    return maxColorAttachments_;
  }

  void GpuCapabilities::logCapabilities(bool extensionsString) {
    LINFO("OpenGL Version:      " << glVersionString_);
    LINFO("OpenGL Renderer:     " << glRendererString_);
    if (glRendererString_.find("Cheetah") != string::npos) {
      LWARNING("It seems that NVIDIA GPU emulation is running on this system.");
      LWARNING("This sometimes leads to strange errors.");
      LWARNING("It is therefore strongly recommended to turn the emulation off!");
    }
    LINFO("GPU Vendor:          " << glVendorString_ << " (" << getVendorAsString() << ")");

    if (extensionsString)
      LINFO("OpenGL Extensions:   " << glExtensionsString_);

    std::stringstream features;
    features << "Texturing:           " << (isOpenGlVersionSupported(GlVersion::TGT_GL_VERSION_1_1) ? "yes" : "no");
    if (isOpenGlVersionSupported(GlVersion::TGT_GL_VERSION_1_1))
      features << ", max size: " << getMaxTextureSize();
    features << ", 3D: " << (is3DTexturingSupported() ? "yes" : "no");
    if (is3DTexturingSupported())
      features << ", max 3D size: " << getMax3DTextureSize();
    LINFO(features.str());

    features.clear();
    features.str("");
    features << "Texture features:    " << getNumTextureUnits() << " units, "
      << (isNpotSupported() ? "" : "no ") << "NPOT, "
      << (areTextureRectanglesSupported() ? "" : "no") << "rectangles, "
      << (isTextureCompressionSupported() ? "" : "no") << "compression, ";
    if (isAnisotropicFilteringSupported())
      features << getMaxTextureAnisotropy() << "x anisotropic";
    else
      features << "no anisotropic";

    LINFO(features.str());

    features.clear();
    features.str("");
    features << "Framebuffer Objects: " << (areFramebufferObjectsSupported() ? "yes" : "no");
    if (areFramebufferObjectsSupported())
      features << ", max " << getMaxColorAttachments() << " color attachments";
    LINFO(features.str());

    features.clear();
    features.str("");
    features << "Shaders:             " << (areShadersSupported() ? "yes (OpenGL 2.0)" : "no");
    if (!areShadersSupported()) {
      LINFO(features.str());
    }
    else {
      features << ", GLSL Version " << shaderVersion_;
      features << ", Shader Model ";
      if (shaderModel_ == SHADER_MODEL_5)
        features << "5.0";
      else if (shaderModel_ == SHADER_MODEL_4)
        features << "4.0";
      else if (shaderModel_ == SHADER_MODEL_3)
        features << "3.0";
      else if (shaderModel_ == SHADER_MODEL_2)
        features << "2.0";
      else
        features << "unknown";

      if (maxGeometryShaderVertices_ >= 0)
        features << ", max GeometryShader Output Vertices: " << maxGeometryShaderVertices_;

      LINFO(features.str());

      if (GLEW_NV_fragment_program2) {
        features.clear();
        features.str("");
        features << "Shader Capabilities: ";

        GLint i = -1;
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOOP_COUNT_NV, &i);
        features << "MAX_PROGRAM_LOOP_COUNT=" << i;
        glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV, &i);
        features << ", MAX_PROGRAM_EXEC_INSTRUCTIONS=" << i;
        LDEBUG(features.str());
      }
    }

    if (retrieveTotalTextureMemory() != -1)
      LINFO("Total Graphics Memory Size:        " << retrieveTotalTextureMemory() / 1024 << " MB");
    else
      LINFO("Total Graphics Memory Size:        No Information");

    if (retrieveAvailableTextureMemory() != -1)
      LINFO("Available Graphics Memory Size:    " << retrieveAvailableTextureMemory() / 1024 << " MB");
    else
      LINFO("Available Graphics Memory Size:    No Information");
  }

  void GpuCapabilities::detectCapabilities() {
    glVersionString_ = string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    glVendorString_ = string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    glRendererString_ = string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    glExtensionsString_ = string(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));

    // Prevent segfault
    const char* glslVS = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    if (glslVS)
      glslVersionString_ = string(glslVS);
    else
      glslVersionString_ = "";

    if (!glVersion_.parseVersionString(glVersionString_)) {
      LERROR("Malformed OpenGL version string: " << glVersionString_);
    }

    // GPU Vendor
    if (glVendorString_.find("NVIDIA") != std::string::npos)
      vendor_ = GPU_VENDOR_NVIDIA;
    else if (glVendorString_.find("ATI") != std::string::npos)
      vendor_ = GPU_VENDOR_ATI;
    else if (glVendorString_.find("INTEL") != std::string::npos ||
      glVendorString_.find("Intel") != std::string::npos)
      vendor_ = GPU_VENDOR_INTEL;
    else {
      LWARNING("Unknown graphics board vendor: " << glVendorString_);
      vendor_ = GPU_VENDOR_UNKNOWN;
    }

    // Shaders
    shaderSupport_ = (glVersion_ >= GlVersion::TGT_GL_VERSION_2_0);
    shaderSupportARB_ = (isExtensionSupported("GL_ARB_vertex_program") &&
      isExtensionSupported("GL_ARB_fragment_program"));

    if (!shaderVersion_.parseVersionString(glslVersionString_)) {
      LERROR("Malformed GLSL version string: " << glslVersionString_);
    }

    // Shader model
    // see http://www.opengl.org/wiki/Shading_languages:_How_to_detect_shader_model%3F
    // for information about shader models in OpenGL
    if (isExtensionSupported("GL_ARB_tessellation_shader"))
      shaderModel_ = SHADER_MODEL_5;
    else if (isExtensionSupported("GL_ARB_geometry_shader4") ||
      isExtensionSupported("GL_EXT_geometry_shader4"))
      shaderModel_ = SHADER_MODEL_4;
    else if (isExtensionSupported("GL_NV_vertex_program3") ||
      isExtensionSupported("GL_ATI_shader_texture_lod"))
      shaderModel_ = SHADER_MODEL_3;
    else if (glVersion_ >= GlVersion::TGT_GL_VERSION_2_0)
      shaderModel_ = SHADER_MODEL_2;
    else
      shaderModel_ = SHADER_MODEL_UNKNOWN;

    //Geometry Shader is supported since ShaderModel 4.0
    if (shaderModel_ >= SHADER_MODEL_4)
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxGeometryShaderVertices_);
    else
      maxGeometryShaderVertices_ = -1;

    // Texturing
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *)&maxTexSize_);
    texturing3D_ = (glVersion_ >= GlVersion::TGT_GL_VERSION_1_2 ||
      isExtensionSupported("GL_EXT_texture3D"));

    if (texturing3D_) {
      if (glVersion_ >= GlVersion::TGT_GL_VERSION_2_0)
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, (GLint *)&max3DTexSize_);
      else
        glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, (GLint *)&max3DTexSize_);
    }
    else {
      max3DTexSize_ = 0;
    }

    // see http://developer.nvidia.com/object/General_FAQ.html#t6
    // for information about texture units
    numTextureUnits_ = -1;
    if (isExtensionSupported("GL_ARB_fragment_program"))
      glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, (GLint *)&numTextureUnits_);

    if (numTextureUnits_ < 0)
      glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint *)&numTextureUnits_);

    npotTextures_ = (isExtensionSupported("GL_ARB_texture_non_power_of_two"));

    textureRectangles_ = (isExtensionSupported("GL_ARB_texture_rectangle"));
    anisotropicFiltering_ = (isExtensionSupported("GL_EXT_texture_filter_anisotropic"));
    if (anisotropicFiltering_)
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxTextureAnisotropy_);
    else
      maxTextureAnisotropy_ = 1.0;
    textureCompression_ = (isExtensionSupported("GL_ARB_texture_compression"));

    framebufferObjects_ = (isExtensionSupported("GL_EXT_framebuffer_object"));

    if (framebufferObjects_) {
      glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &maxColorAttachments_);
    }
    else
      maxColorAttachments_ = -1;

  }

  bool GpuCapabilities::overrideGLSLVersion(const std::string& versionString){
    GlVersion overrideVersion;
    if (overrideVersion.parseVersionString(versionString)) {
      shaderVersion_ = overrideVersion;
      return true;
    }
    else {
      LERROR("GLSL version string '" << versionString << "' could not be parsed. "
        << "Keeping detected version: " << shaderVersion_);
      return false;
    }
  }

  //------------------------------------------------------------------------------

  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_0(1, 0, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_1(1, 1, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_2(1, 2, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_3(1, 3, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_4(1, 4, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_1_5(1, 5, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_2_0(2, 0, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_2_1(2, 1, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_3_0(3, 0, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_3_1(3, 1, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_3_2(3, 2, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_3_3(3, 3, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_4_0(4, 0, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_4_1(4, 1, 0);
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::TGT_GL_VERSION_4_2(4, 2, 0);

  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_110(1, 10); ///< GLSL version 1.10
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_120(1, 20); ///< GLSL version 1.20
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_130(1, 30); ///< GLSL version 1.30
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_140(1, 40); ///< GLSL version 1.40
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_150(1, 50); ///< GLSL version 1.50
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_330(3, 30); ///< GLSL version 3.30
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_400(4, 0); ///< GLSL version 4.00
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_410(4, 10); ///< GLSL version 4.10
  const GpuCapabilities::GlVersion GpuCapabilities::GlVersion::SHADER_VERSION_420(4, 20); ///< GLSL version 4.20

  GpuCapabilities::GlVersion::GlVersion(int major, int minor, int release)
    : major_(major), minor_(minor), release_(release)
  {}

  bool GpuCapabilities::GlVersion::parseVersionString(const string& st) {
    major_ = -1;
    minor_ = -1;
    release_ = -1;

    string str;

    //ignore vendor specific part of the string:
    size_t spacePos = st.find_first_of(" ");
    if (spacePos != string::npos)
      str = st.substr(0, spacePos);
    else
      str = st;

    //explode version string with delimiter ".":
    std::vector<string> exploded;
    size_t found;
    found = str.find_first_of(".");
    while (found != string::npos){
      if (found > 0){
        exploded.push_back(str.substr(0, found));
      }
      str = str.substr(found + 1);
      found = str.find_first_of(".");
    }
    if (str.length() > 0){
      exploded.push_back(str);
    }

    // parse numbers
    if (exploded.size() < 2) {
      LWARNING("Version string too short to parse!");
      return false;
    }

    std::stringstream vstr;

    vstr << exploded[0];
    vstr >> major_;
    if (vstr.fail()) {
      LERROR("Failed to parse major version! Version string: " << st);
      major_ = -1;
      return false;
    }

    vstr.clear();
    vstr.str("");

    vstr << exploded[1];
    vstr >> minor_;
    if (vstr.fail()) {
      LERROR("Failed to parse minor version! Version string: " << st);
      major_ = -1;
      minor_ = -1;
      return false;
    }

    vstr.clear();
    vstr.str("");

    //try to parse release number (not always there):
    if (exploded.size() > 2) {
      vstr << exploded[2];
      vstr >> release_;
      if (vstr.fail())
        release_ = 0;
    }
    else
      release_ = 0;

    return true;
  }


  bool operator==(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if ((x.major_ == y.major_) && (x.minor_ == y.minor_) && (x.release_ == y.release_))
      return true;
    else
      return false;
  }

  bool operator!=(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if ((x.major_ != y.major_) || (x.minor_ != y.minor_) || (x.release_ != y.release_))
      return true;
    else
      return false;
  }

  bool operator<(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if (x.major_ < y.major_)
      return true;
    else if (x.major_ == y.major_) {
      if (x.minor_ < y.minor_)
        return true;
      else if (x.minor_ == y.minor_)
      if (x.release_ < y.release_)
        return true;
    }
    return false;
  }

  bool operator<=(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if (x.major_ < y.major_)
      return true;
    else if (x.major_ == y.major_) {
      if (x.minor_ < y.minor_)
        return true;
      else if (x.minor_ == y.minor_)
      if (x.release_ <= y.release_)
        return true;
    }
    return false;
  }

  bool operator>(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if (x.major_ > y.major_)
      return true;
    else if (x.major_ == y.major_) {
      if (x.minor_ > y.minor_)
        return true;
      else if (x.minor_ == y.minor_)
      if (x.release_ > y.release_)
        return true;
    }
    return false;
  }

  bool operator>=(const GpuCapabilities::GlVersion& x, const GpuCapabilities::GlVersion& y) {
    if (x.major_ > y.major_)
      return true;
    else if (x.major_ == y.major_) {
      if (x.minor_ > y.minor_)
        return true;
      else if (x.minor_ == y.minor_)
      if (x.release_ >= y.release_)
        return true;
    }
    return false;
  }

  std::ostream& operator<<(std::ostream& s, const GpuCapabilities::GlVersion& v) {
    if (v.major_ == -1)
      return (s << "unknown");
    else if (v.release_ == 0)
      return (s << v.major_ << "." << v.minor_);
    else
      return (s << v.major_ << "." << v.minor_ << "." << v.release_);
  }

} // end namespace tgt