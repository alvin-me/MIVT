#version 330

#include "modules/mod_sampler2d.frag"

uniform sampler2D colorTex0_;
uniform sampler2D depthTex0_;
uniform TextureParameters textureParameters0_;
uniform sampler2D colorTex1_;
uniform sampler2D depthTex1_;
uniform TextureParameters textureParameters1_;

out vec4 FragData0;

void main() {
    vec2 p = gl_FragCoord.xy;
    vec4 color0 = textureLookup2D(colorTex0_, textureParameters0_, p*screenDimRCP_);
    float depth0 = textureLookup2D(depthTex0_, textureParameters0_, p*screenDimRCP_).z;
    vec4 color1 = textureLookup2D(colorTex1_, textureParameters1_, p*screenDimRCP_);
    float depth1 = textureLookup2D(depthTex1_, textureParameters1_, p*screenDimRCP_).z;

    vec4 fragColor;
    fragColor.rgb = color1.rgb * color1.a + color0.rgb * color0.a * (1.0 - color1.a);
    fragColor.a = color1.a + color0.a * (1.0 - color1.a);
    float fragDepth = min(depth0, depth1);

    FragData0 = fragColor;
    gl_FragDepth = fragDepth;
}
