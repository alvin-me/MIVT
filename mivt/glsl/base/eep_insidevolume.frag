#version 330

#include "modules/mod_sampler2d.frag"

out vec4 FragData0;

uniform sampler2D firstBack_;      // ray entry points
//uniform sampler2D firstBackDepth_; // depth values of entry points
uniform TextureParameters firstBackParameters_;

uniform sampler2D firstFront_;       // ray exit points
uniform sampler2D firstFrontDepth_; // depth values of entry points
uniform TextureParameters firstFrontParameters_;

uniform float near_;
uniform float far_;
uniform mat4 viewMatrixInverse_;
uniform mat4 projectionMatrixInverse_;
uniform mat4 worldToTexture_;

uniform vec3 llf_;
uniform vec3 urb_;

void main() {
    vec2 p = gl_FragCoord.xy;
    p *= screenDimRCP_;

    vec4 firstBackPos = textureLookup2D(firstBack_, firstBackParameters_, p);
    //float firstBackDepth = textureLookup2D(firstBackDepth_, firstBackParameters_, p).z;

    vec4 firstFrontPos = textureLookup2D(firstFront_, firstFrontParameters_, p);
    float firstFrontDepth = textureLookup2D(firstFrontDepth_, firstFrontParameters_, p).z;

    vec4 fragColor;
    float fragDepth;

    if (firstBackPos.a == 0.0) { // we have no exit pos
        fragColor = vec4(0.0);
        fragDepth = 1.0;
    } else {
        //if (firstFrontDepth < firstBackDepth) {
            //fragColor = firstFrontPos;
            //fragDepth = firstFrontDepth;
        //}
        //else { // fill entry points:
            // use NDCs from -1 to 1
            vec4 wGeom = vec4(2.0*p - 1.0, -1.0, 1.0);

            // reverse perspective division by w (which is equal to the camera-space z)
            float origZG = 2.0*far_*near_ / ((far_ + near_) - wGeom.z*(far_ - near_));
            wGeom *= origZG;

            // unproject and reverse camera-transform
            vec4 result = vec4((viewMatrixInverse_ * (projectionMatrixInverse_ * wGeom)).xyz, 1.0);

            // For single-volume-raycasting we want standard (Krüger-Westermann) entry-exit points:
            result.xyz = (worldToTexture_ * result).xyz;

            if(any(greaterThan(result.xyz, urb_)) || any(lessThan(result.xyz, llf_))) {
                fragColor = firstFrontPos;
                fragDepth = firstFrontDepth;
            }
            else {
                fragColor = result;
                fragDepth = 0.0;
            }
        //}
    }

    FragData0 = fragColor;
    gl_FragDepth = fragDepth;
}
