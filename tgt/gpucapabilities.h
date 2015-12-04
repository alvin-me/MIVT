#pragma once

#include "singleton.h"
#include "config.h"

#include <string>
#include <stdint.h>

namespace tgt {

  class GpuCapabilities;
  template class TGT_API Singleton<GpuCapabilities>;

  /**
  * This Singleton provides information about the graphics system.
  * This information includes:
  *  - OpenGL version
  *  - Supported OpenGL extensions
  *  - GPU vendor
  *  - Texturing and shader capabilities
  *
  * All data is exclusively retrieved
  * through the OpenGL API and can thus be regarded as reliable.
  *
  * The global identifier of this class' singleton is <tt>GpuCaps</tt>.
  */
  class GpuCapabilities : public Singleton<GpuCapabilities> {
  public:

    /**
    * Specifies the version
    * of the OpenGL/glsl implementation.
    * TGT_GL_VERSION_x_y denotes OpenGL version x.y.
    */
    class GlVersion {
    public:
      GlVersion(int major = 0, int minor = 0, int release = 0);

      /**
      * Parse OpenGL version string as specified:
      *
      * The GL_VERSION and GL_SHADING_LANGUAGE_VERSION strings begin with a version number.
      * The version number uses one of these forms:
      *      major_number.minor_number
      *      major_number.minor_number.release_number
      *
      * Vendor-specific information may follow the version number.
      * Its format depends on the implementation, but a space always separates the version number and the vendor-specific information.
      */
      bool parseVersionString(const std::string& st);

      int major_;
      int minor_;
      int release_;

      int major() const { return major_; }
      int minor() const { return minor_; }
      int release() const { return release_; }

      friend bool operator==(const GlVersion& x, const GlVersion& y);
      friend bool operator!=(const GlVersion& x, const GlVersion& y);
      friend bool operator<(const GlVersion& x, const GlVersion& y);
      friend bool operator<=(const GlVersion& x, const GlVersion& y);
      friend bool operator>(const GlVersion& x, const GlVersion& y);
      friend bool operator>=(const GlVersion& x, const GlVersion& y);
      friend std::ostream& operator<<(std::ostream& s, const GlVersion& v);

      static const GlVersion TGT_GL_VERSION_1_0;
      static const GlVersion TGT_GL_VERSION_1_1;
      static const GlVersion TGT_GL_VERSION_1_2;
      static const GlVersion TGT_GL_VERSION_1_3;
      static const GlVersion TGT_GL_VERSION_1_4;
      static const GlVersion TGT_GL_VERSION_1_5;
      static const GlVersion TGT_GL_VERSION_2_0;
      static const GlVersion TGT_GL_VERSION_2_1;
      static const GlVersion TGT_GL_VERSION_3_0;
      static const GlVersion TGT_GL_VERSION_3_1;
      static const GlVersion TGT_GL_VERSION_3_2;
      static const GlVersion TGT_GL_VERSION_3_3;
      static const GlVersion TGT_GL_VERSION_4_0;
      static const GlVersion TGT_GL_VERSION_4_1;
      static const GlVersion TGT_GL_VERSION_4_2;
      static const GlVersion TGT_GL_VERSION_4_3;

      static const GlVersion SHADER_VERSION_110; ///< GLSL version 1.10
      static const GlVersion SHADER_VERSION_120; ///< GLSL version 1.20
      static const GlVersion SHADER_VERSION_130; ///< GLSL version 1.30
      static const GlVersion SHADER_VERSION_140; ///< GLSL version 1.40
      static const GlVersion SHADER_VERSION_150; ///< GLSL version 1.50
      static const GlVersion SHADER_VERSION_330; ///< GLSL version 3.30
      static const GlVersion SHADER_VERSION_400; ///< GLSL version 4.00
      static const GlVersion SHADER_VERSION_410; ///< GLSL version 4.10
      static const GlVersion SHADER_VERSION_420; ///< GLSL version 4.20
      static const GlVersion SHADER_VERSION_430; ///< GLSL version 4.30
    };


    /**
    * Identifies the vendor of the GPU
    * (not the graphics board's vendor).
    */
    enum GpuVendor {
      GPU_VENDOR_NVIDIA,
      GPU_VENDOR_ATI,
      GPU_VENDOR_INTEL,
      GPU_VENDOR_UNKNOWN
    };

    /**
    * Defines the DirectX shader model
    * supported by the GPU. This value
    * can be used to derive information
    * about the GPU's GLSL shader capabilities.
    * The shader model is fully downwards compatible.
    */
    enum ShaderModel {
      SHADER_MODEL_UNKNOWN,
      SHADER_MODEL_2,     ///< implied by OpenGL version 2.0
      SHADER_MODEL_3,     ///< extension NV_vertex_program3 or ATI_shader_texture_lod
      SHADER_MODEL_4,     ///< extension ARB_geometry_shader4
      SHADER_MODEL_5      ///< extension ARB_tessellation_shader
    };

    /**
    * Holds several representations of a Windows file version.
    *
    * Windows file versions are usually represented as a string
    * of the form d1.d2.d3.d4 where d1,d2,d3,d4 are 16-bit unsigned integers,
    * e.g. 6.14.10.6389.
    */
    struct FileVersion {
      int d1, d2, d3, d4;             ///< Parts d1,d2,d3,d4 of the file version, separated
      std::string versionString;      ///< String representation of file version: 'd1.d2.d3.d4'
      uint64_t version;               ///< Binary representation of file version: d1d2d3d4
    };

    /**
    * Contains the graphics driver's version and date.
    */
    struct GraphicsDriverInformation {
      FileVersion driverVersion;      ///< Version of driver dll (the dll is vendor specific)
      std::string driverDate;         ///< Date of last modification of driver dll, format: YYYY-MM-DD
    };


    /**
    * Creates an object for the detection of graphics system properties. If detectCapabilities
    * is set to false, the capabilities of the graphics card aren't detected right away in the
    * constructor. This way you can use GpuCapabilities to detect the amount of memory
    * on the graphics card before initGL() is called. Otherwise GpuCapabilities tries to
    * detect GL values while initGL() isn't called yet and produces a crash.
    */
    TGT_API GpuCapabilities(bool detectCaps = true);

    TGT_API virtual ~GpuCapabilities();

    /**
    * Returns the OpenGL version implemented by the driver.
    *
    * @see GlVersion
    */
    TGT_API GlVersion getGlVersion();

    /**
    * Returns whether a certain OpenGL version is supported.
    *
    * @param version the OpenGL version to check
    *
    * @see GlVersion
    */
    TGT_API bool isOpenGlVersionSupported(GlVersion version);

    /**
    * Returns the vendor of the GPU.
    *
    * @see GpuVendor
    */
    TGT_API GpuVendor getVendor();

    /**
    * Returns the vendor of the GPU.
    *
    * @see GpuVendor
    */
    TGT_API std::string getVendorAsString();

    /**
    * Returns wether a certain OpenGL extension
    * is supported by this implementation. The
    * check is done by parsing the OpenGL
    * extensions-string provided by the graphics driver.
    *
    * @param extension the exact name string of the extension
    *      as found in http://www.opengl.org/registry/
    */
    TGT_API bool isExtensionSupported(std::string extension) const;

    /**
    * Returns the complete OpenGL version string
    * retrieved by <tt>glGetString(GL_VERSION)</tt>.
    */
    TGT_API std::string getGlVersionString();

    /**
    * Returns the complete OpenGL vendor string
    * retrieved by <tt>glGetString(GL_VENDOR)</tt>.
    */
    TGT_API std::string getGlVendorString();

    /**
    * Returns the complete OpenGL renderer string
    * retrieved by <tt>glGetString(GL_RENDERER)</tt>.
    */
    TGT_API std::string getGlRendererString();

    /**
    * Returns the complete OpenGL extensions-string
    * retrieved by <tt>glGetString(GL_EXTENSIONS)</tt>.
    * This strings contains all OpenGL extensions supported
    * by this OpenGL implementation, separated by spaces.
    */
    TGT_API std::string getGlExtensionsString();

    /**
    * Returns the complete Shading Language Version string
    * retrieved by <tt>glGetString(GL_SHADING_LANGUAGE_VERSION)</tt>.
    */
    TGT_API std::string getShadingLanguageVersionString();

    /**
    * Returns wether shaders are supported, which
    * is true for OpenGL version 2.0 or later.
    */
    TGT_API bool areShadersSupported();

    /**
    * Returns wether compute shaders are supported, which
    * is true for OpenGL version 4.3 or later.
    */
    TGT_API bool areComputeShadersSupported();

    /**
    * Returns wether the ARB shader extensions
    * are supported (GL_ARB_vertex_program and
    * GL_ARB_fragment_program).
    *
    * \warning If you want to use shaders based on these
    * extensions, you have call the ARB variants of the
    * shader functions, e.g. <tt>glCompileShaderARB</tt> instead of
    * <tt>glCompileShader</tt>
    */
    TGT_API bool areShadersSupportedARB();

    /**
    * Returns the GLSL shading language version
    * supported by the GPU.
    *
    * @see GlVersion
    */
    TGT_API GlVersion getShaderVersion();

    /**
    * Returns the DirectX shader model
    * supported by the GPU.
    *
    * @see ShaderModel
    */
    TGT_API ShaderModel getShaderModel();

    /**
    * Returns wether a certain shader model
    * is supported.
    *
    * @param shaderModel the shader model to check
    */
    TGT_API bool isShaderModelSupported(ShaderModel shaderModel);

    /**
    * Returns the maximal side length of 1D and 2D textures.
    *
    * @see getMax3DTextureSize
    */
    TGT_API  int getMaxTextureSize();

    /**
    * Queries the currently available texture memory in Kilobytes through the OpenGL API.
    *
    * @note Only supported for NVIDIA and ATI graphics boards.
    *  On other platforms, -1 is returned.
    */
    TGT_API int retrieveAvailableTextureMemory() const;

    /**
    * Queries the total texture memory in Kilobytes through the OpenGL API.
    *
    * @note Only supported for NVIDIA and ATI graphics boards.
    *  On other platforms, -1 is returned.
    */
    TGT_API virtual int retrieveTotalTextureMemory() const;

    /**
    * Returns wether 3D textures are supported.
    * This is the case for OpenGL version 1.2
    * and later.
    */
    TGT_API bool is3DTexturingSupported();

    /**
    * Returns the maximal side length
    * of 3D textures. If 3D texturing
    * is not supported, 0 is returned.
    */
    TGT_API  int getMax3DTextureSize();

    /**
    * Returns the number of texture units
    * provided by the GPU.
    */
    TGT_API int getNumTextureUnits();

    /**
    * Returns wether non-power-of-two textures
    * are supported (extension GL_ARB_texture_non_power_of_two).
    */
    TGT_API bool isNpotSupported();

    /**
    * Returns wether texture rectangles
    * are supported (extension GL_ARB_texture_rectangle).
    */
    TGT_API bool areTextureRectanglesSupported();

    /**
    * Returns wether anisotropic texture filtering
    * is supported (extension GL_EXT_texture_filter_anisotropic).
    */
    TGT_API bool isAnisotropicFilteringSupported();

    /**
    * Returns the maximum anisotropy. If
    * anisotropic texture filtering is not
    * supported, 1.0 is returned.
    */
    TGT_API float getMaxTextureAnisotropy();

    /**
    * Returns wether texture compression
    * is supported (extension GL_ARB_texture_compression).
    */
    TGT_API bool isTextureCompressionSupported();

    /**
    * Returns wether FramebufferObjects (FBOs)
    * are supported (extension GL_EXT_framebuffer_object).
    */
    TGT_API  bool areFramebufferObjectsSupported();

    ///Returns the maximal number of color attachments for a FBO
    TGT_API int getMaxColorAttachments();

    /**
    * Returns the maximum vertices the geometry shader can output.
    * If geometry shader is not supported, -1 is returned.
    */
    TGT_API int getMaxGeometryShaderVertices();

    /**
    * Overrides the detected GLSL language version.
    *
    * @return false, if the passed version string could not be parsed.
    *      In this case, the detected GLSL version is kept.
    */
    TGT_API bool overrideGLSLVersion(const std::string& versionString);

    /**
    * Writes the most important GPU features to the
    * log (debug-level info).
    *
    * @param extensionsString determines wether the
    *      extensions string is also written. This is disabled by default
    *      since the extensions string is usually very long.
    */
    TGT_API virtual void logCapabilities(bool extensionsString = false);

  protected:
    /**
    * Is called by the constructor and performs the
    * complete hardware detection. The results
    * are internally stored.
    */
    virtual void detectCapabilities();

    static const std::string loggerCat_;

  private:
    // detection results are stored in the following members

    GlVersion glVersion_;
    std::string glVersionString_;
    std::string glExtensionsString_;
    std::string glVendorString_;
    std::string glRendererString_;
    std::string glslVersionString_;
    GpuVendor vendor_;

    bool shaderSupport_;
    bool shaderSupportARB_;
    bool shaderSupportCompute_;
    GlVersion shaderVersion_;
    ShaderModel shaderModel_;

    int maxTexSize_;
    bool texturing3D_;
    int max3DTexSize_;
    int numTextureUnits_;
    bool npotTextures_;
    bool textureRectangles_;
    bool anisotropicFiltering_;
    float maxTextureAnisotropy_;
    bool textureCompression_;
    bool framebufferObjects_;
    int maxColorAttachments_;
    int maxGeometryShaderVertices_;
  };

} // end namespace tgt

#define GpuCaps tgt::Singleton<tgt::GpuCapabilities>::getRef()
