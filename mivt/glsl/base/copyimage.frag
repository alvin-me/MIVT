#version 330

#include "modules/mod_sampler2d.frag"

uniform sampler2D colorTex_;
uniform TextureParameters texParams_;
uniform sampler2D depthTex_;

out vec4 FragData0;

void main() {
  vec2 fragCoord = gl_FragCoord.xy * screenDimRCP_;
  vec4 fragColor = textureLookup2D(colorTex_, texParams_, fragCoord);
  FragData0 = fragColor;

  gl_FragDepth = textureLookup2D(depthTex_, texParams_, fragCoord).z;
}