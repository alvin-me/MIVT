#include "raycaster.h"
#include "rendertarget.h"
#include "shadermanager.h"
#include "camera.h"
#include "trackball.h"
#include "textureunit.h"
#include "colorcuberender.h"
#include "rendertoscreen.h"
#include "trianglemeshgeometry.h"

namespace mivt {

  Raycaster::Raycaster()
    : output_(0)
    , mainShader_(0)
    , camera_(0)
    , proxyGeometry_(0)
    , colorCubeRender_(0)
    , renderToScreen_(0)
    , trackball_(0)
  {
  }

  Raycaster::~Raycaster()
  {
  }

  void Raycaster::Initialize()
  {
    //mainShader_ = ShdrMgr.loadSeparate("passthrough.vert", "rc_basic.frag", "", false);

    output_ = new tgt::RenderTarget();
    output_->initialize(GL_RGB);

    camera_ = new tgt::Camera(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    trackball_ = new tgt::Trackball(camera_);

    colorCubeRender_ = new ColorCubeRender();
    colorCubeRender_->Initialize();

    renderToScreen_ = new RenderToScreen();
    renderToScreen_->Initialize();

    // temperory create a geometry.
    const glm::vec3 texLlf(0, 0, 0);
    const glm::vec3 texUrb(1, 1, 1);
    proxyGeometry_ = tgt::TriangleMeshGeometryVec4Vec3::createCube(texLlf, texUrb, texLlf, texUrb, 1.0f);

    glm::vec3 cubeSize = texUrb - texLlf;
    float scale = 2.0f / glm::hmax(cubeSize);
    glm::mat4 matrix = glm::translate(-0.5f * scale * cubeSize) *
      glm::scale(glm::vec3(scale));
    proxyGeometry_->transform(matrix);
  }

  void Raycaster::Deinitialize()
  {
    //ShdrMgr.dispose(mainShader_);
    //mainShader_ = 0;

    output_->deinitialize();
    DELPTR(output_);

    DELPTR(camera_);
    DELPTR(trackball_);

    colorCubeRender_->Deinitialize();
    DELPTR(colorCubeRender_);

    renderToScreen_->Deinitialize();
    DELPTR(renderToScreen_);
  }

  void Raycaster::GetPixels(unsigned char* buffer, int length)
  {
    if (!buffer)
      return;

    Process();
    output_->readColorBuffer(buffer, length);
  }

  void Raycaster::Resize(const glm::ivec2& newSize)
  {
    output_->resize(newSize);
    colorCubeRender_->Resize(newSize);
    renderToScreen_->Resize(newSize);
  }

  void Raycaster::Process()
  {
    // create front & back color cube texture.
    colorCubeRender_->Process(proxyGeometry_, camera_);

    output_->activateTarget();
    output_->clearTarget();

    renderToScreen_->Process(colorCubeRender_->GetFrontFace());

    output_->deactivateTarget();

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  void Raycaster::Rotate(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->rotate(newMouse, lastMouse);
  }

  void Raycaster::Zoom(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    const glm::vec2 zoomInDirection(0.f, 1.f);
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->zoom(newMouse, lastMouse, zoomInDirection);
  }

  void Raycaster::Pan(const glm::ivec2& newPos, const glm::ivec2& lastPos)
  {
    glm::vec2 newMouse = scaleMouse(newPos, output_->getSize());
    glm::vec2 lastMouse = scaleMouse(lastPos, output_->getSize());
    trackball_->move(newMouse, lastMouse);
  }

  glm::vec2 Raycaster::scaleMouse(const glm::ivec2& coords, const glm::ivec2& viewport) const {
    return glm::vec2(static_cast<float>(coords.x*2.f) / static_cast<float>(viewport.x) - 1.f,
      static_cast<float>(coords.y*2.f) / static_cast<float>(viewport.y) - 1.0f);
  }
}

