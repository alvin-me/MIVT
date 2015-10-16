#include "raycaster.h"
#include "rendertarget.h"
#include "shadermanager.h"
#include "camera.h"
#include "textureunit.h"

namespace mivt {

  Raycaster::Raycaster()
    : output_(0)
    , mainShader_(0)
    , camera_(0)
  {
  }

  Raycaster::~Raycaster()
  {
  }

  void Raycaster::Initialize()
  {
    mainShader_ = ShdrMgr.loadSeparate("passthrough.vert", "rc_basic.frag", "", false);

    output_ = new tgt::RenderTarget();
    output_->initialize();

    camera_ = new tgt::Camera(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
  }

  void Raycaster::Deinitialize()
  {
    ShdrMgr.dispose(mainShader_);
    mainShader_ = 0;

    output_->deinitialize();
    DELPTR(output_);

    DELPTR(camera_);
  }

  void Raycaster::GetPixels(unsigned char* buffer, int length)
  {
    buffer, length;
  }

  void Raycaster::Resize(int width, int height)
  {
    output_->resize(glm::ivec2(width, height));
  }

  void Raycaster::Process()
  {
    output_->activateTarget();
    output_->clearTarget();


    output_->deactivateTarget();

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }
}

