#pragma once

#include "tgt_gl.h"
#include "tgt_math.h"
#include "exception.h"
#include "resourcemanager.h"

#include <vector>
#include <sstream>
#include <list>

namespace tgt {

  class ShaderManager;
  template class TGT_API Singleton<ShaderManager>;

  /**
  * Type of a shader object, can be vertex, fragment or geometry shader
  *
  * #include statements are allowed.
  *
  * Geometry shaders can be controled using directives in shader source.
  * Accepted directives:
  * GL_GEOMETRY_INPUT_TYPE_EXT(GL_POINTS | GL_LINES | GL_LINES_ADJACENCY_EXT | GL_TRIANGLES | GL_TRIANGLES_ADJACENCY_EXT)
  * GL_GEOMETRY_OUTPUT_TYPE_EXT(GL_POINTS | GL_LINE_STRIP | GL_TRIANGLE_STRIP)
  * GL_GEOMETRY_VERTICES_OUT_EXT(<int>)
  * No newline or space allowed between each pair of brackets.
  *
  * Example geometry shader header:
  * #version 120
  * #extension GL_EXT_geometry_shader4 : enable
  * //GL_GEOMETRY_INPUT_TYPE_EXT(GL_LINES)
  * //GL_GEOMETRY_OUTPUT_TYPE_EXT(GL_LINE_STRIP)
  * //GL_GEOMETRY_VERTICES_OUT_EXT(42)
  * [...]
  */
  class ShaderObject {
  public:
    friend class Shader;
    friend class ShaderPreprocessor;

    enum ShaderType {
      VERTEX_SHADER = GL_VERTEX_SHADER,
      FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
      GEOMETRY_SHADER = GL_GEOMETRY_SHADER_EXT
    };

    // Helper for resolving line number when includes are used in shader files
    struct LineInfo {
      LineInfo(int n, std::string s, int sn)
      : lineNumber_(n), filename_(s), sourceLineNumber_(sn) {}

      int lineNumber_;          //< line number in preprocessed file
      std::string filename_;    //< filename of included file
      int sourceLineNumber_;    //< line number in included file (needed when it itself
      //< includes another file)
    };

    /**
    * Creates a shader object of the specified type
    */
    TGT_API ShaderObject(const std::string& filename, ShaderType type = VERTEX_SHADER);

    /**
    * Deletes the shader and source
    */
    TGT_API ~ShaderObject();

    /**
    * Loads the shader source from the specified file.
    *
    * @throw Exception if loading failed.
    */
    TGT_API void loadSourceFromFile(const std::string& filename)
      throw (Exception);

    /**
    * Set directives using glProgramParameteriEXT(...), used for geometry shaders.
    * Call before compiling.
    * @param id Set the directives for this shader
    */
    TGT_API void setDirectives(GLuint id);

    TGT_API bool compileShader();

    TGT_API bool isCompiled() const { return isCompiled_; }

    TGT_API std::string getCompilerLog() const;

    TGT_API bool rebuildFromFile();

    /**
    * Use h as header for shadersource (copies h)
    */
    TGT_API void setHeader(const std::string& h);

    TGT_API ShaderType getType() { return shaderType_; }

    TGT_API void setSource(std::string source) {
      source_ = source;
      unparsedSource_ = source;
    }
    TGT_API const std::string getSource() { return unparsedSource_; }

    /**
    * Set geometry shader input type. For this change to take effect call setDirectives() and
    * re-link already linked shaders. Currently only GL_POINTS, GL_LINES,
    * GL_LINES_ADJACENCY_EXT, GL_TRIANGLES or GL_TRIANGLES_ADJACENCY_EXT can be used.
    */
    TGT_API void setInputType(GLint inputType) { inputType_ = inputType; }
    TGT_API GLint getInputType() const { return inputType_; }

    /**
    * Set geometry shader output type. For this change to take effect call setDirectives() and
    * re-link already linked shaders. Currently only GL_POINTS, GL_LINE_STRIP or
    * GL_TRIANGLE_STRIP can be used.
    */
    TGT_API void setOutputType(GLint outputType) { outputType_ = outputType; }
    TGT_API GLint getOuputType() const { return outputType_; }

    /**
    * Set maximum number of primitives a geometry shader can create.
    * For this change to take effect call setDirectives()
    * re-link already linked shaders. Limited by GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT.
    */
    TGT_API void setVerticesOut(GLint verticesOut) { verticesOut_ = verticesOut; }
    TGT_API GLint getVerticesOut() const { return verticesOut_; }

  protected:
    void uploadSource();

    std::string filename_;
    ShaderType shaderType_;

    GLuint id_;
    std::string source_;
    std::string unparsedSource_;
    std::string header_;
    bool isCompiled_;
    GLint inputType_;
    GLint outputType_;
    GLint verticesOut_;

    std::vector<LineInfo> lineTracker_; ///< keeps track of line numbers when includes are used
    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  /**
  * Represents an OpenGL shader program, consisting of linked ShaderObjects.
  *
  * @note Convenient loading of shaders from file is provided by ShaderManager.
  */
  class Shader {

    friend class ShaderManager;

  public:
    TGT_API Shader();

    /**
    * Detaches all shader objects, deletes them an disposes all textures
    */
    TGT_API ~Shader();

    /**
    * Attach shader object to Shader
    */
    TGT_API void attachObject(ShaderObject* obj);
    TGT_API void detachObject(ShaderObject* obj);
    TGT_API void detachObjectsByType(ShaderObject::ShaderType type);

    /**
    * Link all shader objects to one shader.
    * Will re-link already linked shaders.
    * @return true for success
    */
    TGT_API bool linkProgram();

    TGT_API bool rebuild();
    TGT_API bool rebuildFromFile();

    TGT_API void setHeaders(const std::string& customHeader);

    TGT_API void setFragmentHeader(const std::string& customHeader);

    TGT_API void bindFragDataLocation(GLuint colorNumber, std::string name);

    TGT_API GLint getID() { return id_; };
    /**
    * Activates the shader
    */
    TGT_API void activate();

    TGT_API static void activate(GLint id);

    TGT_API static void deactivate();

    TGT_API static GLint getCurrentProgram();

    /**
    * Returns whether the Shader has at least one attached shader object.
    */
    TGT_API bool hasObjects() const {
      return !objects_.empty();
    }

    /**
    * Returns whether the Shader is currently activated
    */
    TGT_API bool isActivated();

    TGT_API bool isLinked() { return isLinked_; }

    TGT_API std::string getLinkerLog() const;

    //
    // Uniform stuff
    //

    /**
    * Returns uniform location, or -1 on failure
    */
    TGT_API GLint getUniformLocation(const std::string& name);

    TGT_API void setIgnoreUniformLocationError(bool ignoreError);
    TGT_API bool getIgnoreUniformLocationError();

    // Floats
    TGT_API bool setUniform(const std::string& name, GLfloat value);
    TGT_API bool setUniform(const std::string& name, GLfloat v1, GLfloat v2);
    TGT_API bool setUniform(const std::string& name, GLfloat v1, GLfloat v2, GLfloat v3);
    TGT_API bool setUniform(const std::string& name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
    TGT_API bool setUniform(const std::string& name, GLfloat* v, int count);

    // Integers
    TGT_API bool setUniform(const std::string& name, GLint value);
    TGT_API bool setUniform(const std::string& name, GLint v1, GLint v2);
    TGT_API bool setUniform(const std::string& name, GLint v1, GLint v2, GLint v3);
    TGT_API bool setUniform(const std::string& name, GLint v1, GLint v2, GLint v3, GLint v4);
    TGT_API bool setUniform(const std::string& name, GLint* v, int count);

    // Booleans
    TGT_API bool setUniform(const std::string& name, bool value);
    TGT_API bool setUniform(const std::string& name, bool v1, bool v2);
    TGT_API bool setUniform(const std::string& name, bool v1, bool v2, bool v3);
    TGT_API bool setUniform(const std::string& name, bool v1, bool v2, bool v3, bool v4);
    TGT_API bool setUniform(const std::string& name, GLboolean* v, int count);

    // Vectors
    TGT_API bool setUniform(const std::string& name, const glm::vec2& value);
    TGT_API bool setUniform(const std::string& name, glm::vec2* vectors, GLsizei count = 1);
    TGT_API bool setUniform(const std::string& name, const glm::vec3& value);
    TGT_API bool setUniform(const std::string& name, glm::vec3* vectors, GLsizei count = 1);
    TGT_API bool setUniform(const std::string& name, const glm::vec4& value);
    TGT_API bool setUniform(const std::string& name, glm::vec4* vectors, GLsizei count = 1);
    TGT_API bool setUniform(const std::string& name, const glm::ivec2& value);
    TGT_API bool setUniform(const std::string& name, glm::ivec2* vectors, GLsizei count = 1);
    TGT_API bool setUniform(const std::string& name, const glm::ivec3& value);
    TGT_API bool setUniform(const std::string& name, glm::ivec3* vectors, GLsizei count = 1);
    TGT_API bool setUniform(const std::string& name, const glm::ivec4& value);
    TGT_API bool setUniform(const std::string& name, glm::ivec4* vectors, GLsizei count = 1);

    // Note: Matrix is transposed by OpenGL
    TGT_API bool setUniform(const std::string& name, const glm::mat2& value, bool transpose = false);
    TGT_API bool setUniform(const std::string& name, const glm::mat3& value, bool transpose = false);
    TGT_API bool setUniform(const std::string& name, const glm::mat4& value, bool transpose = false);

    // No location lookup
    //
    // Floats
    TGT_API static void setUniform(GLint l, GLfloat value);
    TGT_API static void setUniform(GLint l, GLfloat v1, GLfloat v2);
    TGT_API static void setUniform(GLint l, GLfloat v1, GLfloat v2, GLfloat v3);
    TGT_API static void setUniform(GLint l, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);

    // Integers
    TGT_API static void setUniform(GLint l, GLint value);
    TGT_API static void setUniform(GLint l, GLint v1, GLint v2);
    TGT_API static void setUniform(GLint l, GLint v1, GLint v2, GLint v3);
    TGT_API static void setUniform(GLint l, GLint v1, GLint v2, GLint v3, GLint v4);

    // Vectors
    TGT_API static void setUniform(GLint l, const glm::vec2& value);
    TGT_API static void setUniform(GLint l, const glm::vec3& value);
    TGT_API static void setUniform(GLint l, const glm::vec4& value);
    TGT_API static void setUniform(GLint l, const glm::ivec2& value);
    TGT_API static void setUniform(GLint l, const glm::ivec3& value);
    TGT_API static void setUniform(GLint l, const glm::ivec4& value);
    TGT_API static void setUniform(GLint l, const glm::mat2& value, bool transpose = false);
    TGT_API static void setUniform(GLint l, const glm::mat3& value, bool transpose = false);
    TGT_API static void setUniform(GLint l, const glm::mat4& value, bool transpose = false);

    // Attributes
    //
    // 1 component
    TGT_API static void setAttribute(GLint index, GLfloat v1);
    TGT_API static void setAttribute(GLint index, GLshort v1);
    TGT_API static void setAttribute(GLint index, GLdouble v1);

    // 2 components
    TGT_API static void setAttribute(GLint index, GLfloat v1, GLfloat v2);
    TGT_API static void setAttribute(GLint index, GLshort v1, GLshort v2);
    TGT_API static void setAttribute(GLint index, GLdouble v1, GLdouble v2);

    // 3 components
    TGT_API static void setAttribute(GLint index, GLfloat v1, GLfloat v2, GLfloat v3);
    TGT_API static void setAttribute(GLint index, GLshort v1, GLshort v2, GLshort v3);
    TGT_API static void setAttribute(GLint index, GLdouble v1, GLdouble v2, GLdouble v3);

    // 4 components
    TGT_API static void setAttribute(GLint index, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
    TGT_API static void setAttribute(GLint index, GLshort v1, GLshort v2, GLshort v3, GLshort v4);
    TGT_API static void setAttribute(GLint index, GLdouble v1, GLdouble v2, GLdouble v3, GLdouble v4);

    // For vectors
    TGT_API static void setAttribute(GLint index, const glm::vec2& v);
    TGT_API static void setAttribute(GLint index, const glm::vec3& v);
    TGT_API static void setAttribute(GLint index, const glm::vec4& v);
    TGT_API static void setAttribute(GLint index, const glm::ivec4& v);

    // Attribute locations
    TGT_API void setAttributeLocation(GLuint index, const std::string& name);
    TGT_API GLint getAttributeLocation(const std::string& name);

  protected:
    /**
    * Load filename.vert and filename.frag (vertex and fragment shader) and link shader.
    *
    * @param customHeader Header to be put in front of the shader source.
    *
    * @throw Exception if loading failed
    */
    void load(const std::string& filename, const std::string& customHeader = "")
      throw (Exception);

    /**
    * Load vertex shader \p vertFilename, geometry shader \p geomFilename,
    * fragment shader \p fragFilename.
    *
    * @param customHeader header to be put in front of the shader source
    *
    * @throw Exception if loading failed
    */
    void loadSeparate(const std::string& vertFilename, const std::string& geomFilename,
      const std::string& fragFilename, const std::string& customHeader = "")
      throw (Exception);

    typedef std::list<ShaderObject*> ShaderObjects;
    ShaderObjects objects_;

    GLuint id_;
    bool isLinked_;
    bool ignoreError_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  /**
  * Parses #include statements and geometry shader settings
  */
  class ShaderPreprocessor {
  public:
    enum Mode {
      MODE_NONE = 0,
      MODE_INCLUDE = 1,
      MODE_GEOMETRY = 2
    };

    TGT_API ShaderPreprocessor(ShaderObject* obj, Mode mode = Mode(MODE_INCLUDE | MODE_GEOMETRY));

    // Returns the parsed result
    TGT_API std::string getResult() const;

    TGT_API GLint getGeomShaderInputType() const;
    TGT_API GLint getGeomShaderOutputType() const;
    TGT_API GLint getGeomShaderVerticesOut() const;

  protected:
    void parse();
    void parsePart(const std::string& input, const std::string& name = "");

    void outputComment(const std::string& comment, const std::string& type = "INFO");

    /**
    *    Scan for geometry shader directives in shader source.
    *
    *    Accepted directives:
    *    GL_GEOMETRY_INPUT_TYPE_EXT(GL_POINTS | GL_LINES | GL_LINES_ADJACENCY_EXT | GL_TRIANGLES | GL_TRIANGLES_ADJACENCY_EXT)
    *    GL_GEOMETRY_OUTPUT_TYPE_EXT(GL_POINTS | GL_LINE_STRIP | GL_TRIANGLE_STRIP)
    *    GL_GEOMETRY_VERTICES_OUT_EXT(<int>)
    *    No newline or space allowed between each pair of brackets.
    *
    *    Example geometry shader header:
    *    #version 120
    *    #extension GL_EXT_geometry_shader4 : enable
    *
    *    //GL_GEOMETRY_INPUT_TYPE_EXT(GL_LINES)
    *    //GL_GEOMETRY_OUTPUT_TYPE_EXT(GL_LINE_STRIP)
    *    //GL_GEOMETRY_VERTICES_OUT_EXT(42)
    *    [...]
    */
    bool scanGeomShaderDirectives();

    std::string getGeomShaderDirective(const std::string& d);


    ShaderObject* shd_;
    std::vector<ShaderObject::LineInfo>& lineTracker_; ///< keeps track of line numbers when includes are used
    int activeLine_;
    std::ostringstream result_;

    GLint inputType_;
    GLint outputType_;
    GLint verticesOut_;

    static const std::string loggerCat_;
  };

  //------------------------------------------------------------------------------

  /**
  * Loads shaders from the file system, managing a shader search path.
  */
  class ShaderManager : public ResourceManager<Shader>, public Singleton<ShaderManager> {
  public:

    TGT_API ShaderManager();

    TGT_API virtual ~ShaderManager();

    /**
    * Load filename.vert and filename.frag (vertex and fragment shader), link shader and
    * activate it by default.
    *
    * @param customHeader Header to be put in front of the shader source
    * @param activate activate the shader after loading
    *
    * @return The loaded shader
    *
    * @throw Exception if loading failed
    */
    TGT_API Shader* load(const std::string& filename, const std::string& customHeader = "",
      bool activate = true)
      throw (Exception);

    /**
    * Load vertex shader \p vertFilename and fragment shader \p fragFilename,
    * link shader and activate it by default.
    *
    * You have to pass the complete filenames, inclusive file extensions (".vert", ".frag").
    *
    * @param customHeader header to be put in front of the shader source
    * @param activate activate the shader after loading
    *
    * @return The loaded shader
    *
    * @throw Exception if loading failed
    */
    TGT_API Shader* loadSeparate(const std::string& vertFilename, const std::string& fragFilename,
      const std::string& customHeader = "", bool activate = true)
      throw (Exception);

    /**
    * Load vertex shader \p vertFilename, geometry shader \p geomFilename,
    * fragment shader \p fragFilename, link shader and activate it by default.
    *
    * You have to pass the complete filenames, inclusive file extensions (".vert", ".geom", frag").
    *
    * @param customHeader header to be put in front of the shader source
    * @param activate activate the shader after loading
    *
    * @return The loaded shader
    *
    * @throw Exception if loading failed
    */
    TGT_API Shader* loadSeparate(const std::string& vertFilename, const std::string& geomFilename,
      const std::string& fragFilename,
      const std::string& customHeader, bool activate = true)
      throw(Exception);

    TGT_API bool rebuildAllShadersFromFile();

  protected:
    static const std::string loggerCat_;
  };

} // end namespace tgt

#define ShdrMgr tgt::Singleton<tgt::ShaderManager>::getRef()