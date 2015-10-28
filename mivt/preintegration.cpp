#include "preintegration.h"
#include "transfunc1d.h"
#include "logmanager.h"
#include "rendertarget.h"
#include "shadermanager.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::col3;

namespace mivt {

  PreIntegration::PreIntegration(size_t resolution, bool computeOnGPU, bool useIntegral)
    : transFunc_(0)
    , resolution_(resolution)
    , samplingStepSize_(0)
    , useIntegral_(useIntegral)
    , computeOnGPU_(computeOnGPU)
    , table_(0)
    , tex_(0)
    , renderTarget_(0)
    , program_(0)
  {
    //check values
    if (resolution <= 1)
      resolution_ = 256;

    //initialize render target if necessary
    if (computeOnGPU_) {
      renderTarget_ = new tgt::RenderTarget();
      renderTarget_->initialize();
      renderTarget_->resize(glm::ivec2(static_cast<int>(resolution_)));

      program_ = ShdrMgr.loadSeparate("passthrough.vert", "preintegration.frag", "", false);
    }
    else {
      table_ = new vec4[resolution_ * resolution_];
      tex_ = new tgt::Texture(reinterpret_cast<GLubyte*>(table_), glm::ivec3(static_cast<int>(resolution_),
        static_cast<int>(resolution_), 1), GL_RGBA, GL_RGBA32F, GL_FLOAT, tgt::Texture::LINEAR);
    }
  }

  PreIntegration::~PreIntegration()
  {
    //deinitialize render target or delete texture
    if (computeOnGPU_) {
      renderTarget_->deinitialize();
      DELPTR(renderTarget_);

      ShdrMgr.dispose(program_);
      program_ = 0;
    }
    else {
      DELARR(table_);
      DELPTR(tex_);
    }
  }

  void PreIntegration::computeTable()
  {
    if (!transFunc_)
      return;

    //buffer for TF values
    vec4* tfBuffer = new vec4[resolution_];

    vec2 domain = transFunc_->getWindowingDomain();
    for (int x = 0; x < resolution_; ++x) {
      float value = (float)x / (resolution_ - 1) * (domain.y - domain.x) + domain.x;
      col3 color = transFunc_->getMappingForColorValue(value);
      float alpha = transFunc_->getMappingForAlphaValue(value);
      tfBuffer[x] = vec4(vec3(color) / 255.f, alpha);
    }

    if (!useIntegral_) { // Correct (but slow) calculation of PI-table:
      int lookupindex = 0;
      for (int sb = 0; sb < static_cast<int>(resolution_); ++sb) {
        for (int sf = 0; sf < static_cast<int>(resolution_); ++sf) {
          if (sb != sf) {
            float scale = 1.0f / fabs(static_cast<float>(sf - sb));

            int incr = 1;
            if (sf < sb)
              incr = -1;

            vec4 result = vec4(0.0f);
            for (int s = sb; (incr == 1 ? s <= sf : s >= sf) && (result.a < 0.95); s += incr) {
              vec4 curCol = tfBuffer[s];

              if (curCol.a > 0.0f) {
                // apply opacity correction to accomodate for variable sampling intervals
                curCol.a = 1.f - pow(1.f - curCol.a, samplingStepSize_ * 200.0f * scale);

                //actual compositing
                result.x += (1.0f - result.a) * curCol.a * curCol.x;
                result.y += (1.0f - result.a) * curCol.a * curCol.y;
                result.z += (1.0f - result.a) * curCol.a * curCol.z;
                result.a += (1.0f - result.a) * curCol.a;
              }
            }
            result.x /= glm::max(result.a, 0.001f);
            result.y /= glm::max(result.a, 0.001f);
            result.z /= glm::max(result.a, 0.001f);
            result.a = 1.f - pow(1.f - result.a, 1.0f / (samplingStepSize_ * 200.0f));
            table_[lookupindex] = glm::clamp(result, 0.f, 1.f);
          }
          else {
            table_[lookupindex] = glm::clamp(tfBuffer[sf], 0.f, 1.f);
          }
          lookupindex++;
        }
      }
    }
    else { //faster version using integral functions, see Real-Time Volume Graphics, p96
      //compute integral functions
      vec4* intFunc = new vec4[resolution_];

      vec4 accumResult(0.f);
      vec4 curColor;

      for (int i = 0; i < static_cast<int>(resolution_); ++i) {
        //fetch current value from TF
        vec4 curCol = tfBuffer[i];

        //calculate new integral function
        if (curCol.a > 0.0f) {
          //actual compositing
          accumResult.x += curCol.x * curCol.a;
          accumResult.y += curCol.y * curCol.a;
          accumResult.z += curCol.z * curCol.a;
          accumResult.a += curCol.a;
        }
        intFunc[i] = accumResult;
      }
      float factor;

      int lookupindex = 0;

      int endIndex = static_cast<int>(resolution_);

      for (int sb = 0; sb < endIndex; ++sb) {
        for (int sf = 0; sf < endIndex; ++sf) {
          int smin = glm::min(sb, sf);
          int smax = glm::max(sb, sf);

          vec4 col;
          if (smax != smin) {
            factor = samplingStepSize_ * 200.f / static_cast<float>(smax - smin);

            col.x = (intFunc[smax].x - intFunc[smin].x) * factor;
            col.y = (intFunc[smax].y - intFunc[smin].y) * factor;
            col.z = (intFunc[smax].z - intFunc[smin].z) * factor;
            col.a = 1.f - exp(-(intFunc[smax].a - intFunc[smin].a) * factor);

            col.x /= glm::max(col.a, 0.001f);
            col.y /= glm::max(col.a, 0.001f);
            col.z /= glm::max(col.a, 0.001f);
            col.a = 1.f - pow(1.f - col.a, 1.0f / (samplingStepSize_ * 200.0f));
          }
          else {
            col = tfBuffer[smin];
          }

          table_[lookupindex] = glm::clamp(col, 0.f, 1.f);

          lookupindex++;
        }
      }
      delete[] intFunc;
    }

    delete[] tfBuffer;
  }

  const tgt::Texture* PreIntegration::getTexture(tgt::TransFunc1D *transFunc, float d)
  {
    if (transFunc != transFunc_ || samplingStepSize_ != d || transFunc->isPreinteTextureInvalid()) 
    {
      transFunc_ = transFunc;
      samplingStepSize_ = d;
      transFunc->validPreinteTexture();

      if (computeOnGPU_) {
        computeTableGPU();
        LGL_ERROR;

        //set output texture
        tex_ = renderTarget_->getColorTexture();
      }
      else {
        computeTable();
        tex_->setWrapping(tgt::Texture::CLAMP_TO_BORDER);
        tex_->setPixelData(reinterpret_cast<GLubyte*>(table_));
        tex_->uploadTexture();
        LGL_ERROR;

        // prevent deleting twice
        tex_->setPixelData(0);
      }
    }
    return tex_;
  }

  void PreIntegration::computeTableGPU()
  {
    //render pre-integration texture into render target
    renderTarget_->activateTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //get texture of transfer function
    tgt::TextureUnit transferUnit;
    transferUnit.activate();

    transFunc_->getTexture()->bind();
    transFunc_->getTexture()->enable();

    // activte shader program
    program_->activate();

    program_->setUniform("samplingStepSize_", samplingStepSize_);

    //set transfer function texture
    program_->setUniform("tfTex_", transferUnit.getUnitNumber());

    bool oldIgnoreError = program_->getIgnoreUniformLocationError();
    program_->setIgnoreUniformLocationError(true);
    program_->setUniform("texParams_.dimensions_", glm::vec2((float)resolution_));
    program_->setUniform("texParams_.dimensionsRCP_", glm::vec2(1.f) / glm::vec2((float)resolution_));
    program_->setUniform("texParams_.matrix_", glm::mat4(1));
    program_->setIgnoreUniformLocationError(oldIgnoreError);

    //render quad
    glDepthFunc(GL_ALWAYS);
    glBegin(GL_QUADS);
    glVertex2f(-1.f, -1.f);
    glVertex2f(1.f, -1.f);
    glVertex2f(1.f, 1.f);
    glVertex2f(-1.f, 1.f);
    glEnd();
    glDepthFunc(GL_LESS);

    //clean up
    transFunc_->getTexture()->disable();
    program_->deactivate();
    renderTarget_->deactivateTarget();
    

    tgt::TextureUnit::setZeroUnit();
    LGL_ERROR;
  }

  bool PreIntegration::computeOnGPU() {
    return computeOnGPU_;
  }

} // end namespace mivt

