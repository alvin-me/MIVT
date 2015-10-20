uniform vec2 screenDim_;
uniform vec2 screenDimRCP_;

/*
* This struct contains information about a texture, like
* its dimensions. Additionally, the reciprocal
* values of all parameters are available (suffix RCP) .
*/
struct TextureParameters {
  vec2 dimensions_;        // the texture's resolution, e.g. [256.0, 128.0]
  vec2 dimensionsRCP_;
  mat4 matrix_;            // texture coordinate transformation
};

// Texture lookup function for 2D textures,
// expecting normalized texture coordinates, i.e., [0,1].
vec4 textureLookup2D(in sampler2D myTexture, in TextureParameters texParams, in vec2 texCoords) {
  vec2 texCoordsTransformed = (texParams.matrix_ * vec4(texCoords, 0.0, 1.0)).xy;
  return texture(myTexture, texCoordsTransformed);
}