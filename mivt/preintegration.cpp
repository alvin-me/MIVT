#include "preintegration.h"
#include "transfunc1d.h"
#include "logmanager.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::col3;

namespace mivt {

  PreIntegration::PreIntegration(size_t resolution, bool useIntegral)
    : transFunc_(0)
    , resolution_(resolution)
    , samplingStepSize_(0)
    , useIntegral_(useIntegral)
    , table_(0)
    , tex_(0)
  {
    //check values
    if (resolution <= 1)
      resolution_ = 256;

    table_ = new vec4[resolution_ * resolution_];
    tex_ = new tgt::Texture(reinterpret_cast<GLubyte*>(table_), glm::ivec3(static_cast<int>(resolution_), 
      static_cast<int>(resolution_), 1), GL_RGBA, GL_RGBA32F, GL_FLOAT, tgt::Texture::LINEAR);
  }

  PreIntegration::~PreIntegration()
  {
    DELARR(table_);
    DELPTR(tex_);
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
    if (transFunc != transFunc_ || samplingStepSize_ != d) {
      transFunc_ = transFunc;
      samplingStepSize_ = d;

      computeTable();
      tex_->setWrapping(tgt::Texture::CLAMP_TO_BORDER);
      tex_->setPixelData(reinterpret_cast<GLubyte*>(table_));
      tex_->uploadTexture();
      LGL_ERROR;

      // prevent deleting twice
      tex_->setPixelData(0);
    }
    return tex_;
  }

} // end namespace mivt

