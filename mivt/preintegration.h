#pragma once

#include "tgt_math.h"

namespace tgt {
  class TransFunc1D;
  class Texture;
  class RenderTarget;
  class Shader;
}

namespace mivt {

  class PreIntegration
  {
  public:
    /**
    * Constructor, automatically calls computeTable at the end.
    *
    * @param resolution the resolution of the table in both directions (if <= 1 the resolution is set to 256)
    * @param useIntegral Use integral functions to compute pre-integration table, which is faster but not quite as accurate.
    * @param computeOnGPU Compute the pre-integration table texture on the GPU.
    */
    PreIntegration(size_t resolution = 256, bool computeOnGPU = false, bool useIntegral = false);
    
    ~PreIntegration();

    /**
    * Returns the texture of the pre-integration table.
    * If no texture has been created yet, createTex() is called.
    *
    * @param transFunc the transfer function for with the pre-integration table is computed
    * @param d the segment length (= sampling step size) for which the pre-integration table is computed (if <= 0 the segment length is set to 1.0)
    */
    const tgt::Texture* getTexture(tgt::TransFunc1D *transFunc, float d);

    bool computeOnGPU();

  private:
    /// Compute the pre-integrated table for the given transfer function.
    void computeTable();

    ///  Compute the pre-integrated table on the GPU.
    void computeTableGPU();

  private:
    size_t      resolution_;                ///< resolution of the pre-integrated table
    float       samplingStepSize_;          ///< length of the segments
    bool        useIntegral_;               ///< true for approximative (but faster) computation using integral functions
    bool        computeOnGPU_;         ///< if true this pre-integration table is computed on the GPU
    glm::vec4   *table_;                    ///< the actual pre-integration table in row-major order
    tgt::TransFunc1D  *transFunc_;      ///< the 1D transfer function that is used to compute the pre-integration table
    tgt::Texture      *tex_;            ///< texture for the pre-integration table, is generated internally
    tgt::RenderTarget *renderTarget_;   ///< internal render target for computing the pre-integration table on the gpu
    tgt::Shader       *program_;        ///< shader program to compute the pre-integration table on the gpu
  };

} // end namespace mivt
