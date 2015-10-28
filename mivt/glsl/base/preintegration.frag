#version 330

#include "modules/mod_sampler2d.frag"

out vec4 FragData0;

uniform sampler1D tfTex_;
uniform TextureParameters texParams_;

//uniform int resolution_;

uniform float samplingStepSize_;

void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 fragCoordNorm = gl_FragCoord.xy * texParams_.dimensionsRCP_;

    vec4 fragColor = vec4(0.0);

    //if intensity values at start and end of the segment are equal: lookup transfer function
    if (fragCoord.x == fragCoord.y)
        fragColor = texture(tfTex_, fragCoordNorm.x);
    else {
        //else: perform compositing in transfer function space
        int sf = int(fragCoord.x);
        int sb = int(fragCoord.y);
        float scale = 1 / abs(float(sb - sf));

        int incr = (sb - sf) / abs(sb - sf);

        //for (int s = sf; (incr == 1 ? s <= sb : s >= sb) && (fragColor.a < 0.95); s += incr) {
        for (int s = sf; ((incr * s) <= (incr * sb)) && (fragColor.a < 0.95); s += incr) {
            vec4 curColor = texture(tfTex_, s * texParams_.dimensionsRCP_.x);

            if (curColor.a > 0.0) {
                // apply opacity correction to accomodate for variable sampling intervals
                curColor.a = 1.0 - pow(1.0 - curColor.a, samplingStepSize_ * 200.0 * scale);

                //actual compositing
                vec3 result_rgb = fragColor.xyz + (1.0 - fragColor.a) * curColor.a * curColor.xyz;
                fragColor.a = fragColor.a + (1.0 - fragColor.a) * curColor.a;

                fragColor.xyz = result_rgb;
            }
        }

        fragColor.xyz /= max(fragColor.a, 0.001);
        fragColor.a = 1.0 - pow(1.0 - fragColor.a, 1.0 / (samplingStepSize_ * 200.0));
        fragColor = clamp(fragColor, 0.0, 1.0);
    }

    FragData0 = fragColor;
}
