#pragma once
#include "tgt_math.h"
#include "renderbase.h"

namespace tgt {
  class Texture;
  class RenderTarget;
  class Shader;
}

namespace mivt {
  class RenderBackground : public RenderBase
  {
  public:
    RenderBackground();
    ~RenderBackground();

    void Initialize();
    void Deinitialize();
    void Resize(const glm::ivec2& newSize);
    void Process(tgt::RenderTarget *input);
    tgt::RenderTarget* GetOutput();

    void SetFirstColor(const glm::vec4 color);
    glm::vec4 GetFirstColor();

    void SetSecondColor(const glm::vec4 color);
    glm::vec4 GetSecondColor();

    void SetColorMode(const std::string& mode);
    std::string GetColorMode();

  private:

    /**
    * Render the background
    */
    void renderBackground();

    /**
    * load (and create) needed textures
    */
    void loadTexture();

    /**
    * create an alpha-circle
    */
    void createRadialTexture();

    /**
    * create an empty texture, i.e. all texels set to zero
    */
    void createEmptyTexture();

  private:
    glm::vec4 firstcolor_;
    glm::vec4 secondcolor_;
    int       angle_;
    std::string mode_;

    tgt::Texture        *tex_;
    tgt::RenderTarget   *output_;
    tgt::RenderTarget   *privatetarget_;
    tgt::Shader         *program_;

    static const std::string loggerCat_;
  };
}


