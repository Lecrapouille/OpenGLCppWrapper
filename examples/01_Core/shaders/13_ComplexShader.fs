// Copyright: Universe Nursery by andrewray
// https://shaderfrog.com/app/view/47

#version 150

// http://casual-effects.blogspot.com/2013/08/starfield-shader.html
#extension GL_OES_standard_derivatives : enable

#define iterations 17
#define volsteps 3
#define sparsity 0.5
#define stepsize 0.2
#define frequencyVariation   1.3

precision highp float;
precision highp int;

in vec2 vUv;

uniform vec3 color;
uniform float time;
uniform float twinkleSpeed;
uniform float speed;
 
uniform float brightness;
uniform float distfading;
out vec4 FragColor;

#define PI 3.141592653589793238462643383279

void main( void ) {

    vec2 uv = vUv.xy + 0.5;
    uv.x += time * speed * 0.1;
 
    vec3 dir = vec3(uv * 2.0, 1.0);
 
    float s = 0.1, fade = 0.01;
    vec3 starColor = vec3(0.0);
     
    for (int r = 0; r < volsteps; ++r) {
        vec3 p =  (time * speed * twinkleSpeed) + dir * (s * 0.5);
        p = abs(vec3(frequencyVariation) - mod(p, vec3(frequencyVariation * 2.0)));
 
        float prevlen = 0.0, a = 0.0;
        for (int i = 0; i < iterations; ++i) {
            p = abs(p);
            p = p * (1.0 / dot(p, p)) + (-sparsity); // the magic formula            
            float len = length(p);
            a += abs(len - prevlen); // absolute sum of average change
            prevlen = len;
        }
         
        a *= a * a; // add contrast
         
        // coloring based on distance        
        starColor += (vec3(s, s*s, s*s*s) * a * brightness + 1.0) * fade;
        fade *= distfading; // distance fading
        s += stepsize;
    }
     
    starColor = min(starColor, vec3(1.2));
 
    // Detect and suppress flickering single pixels (ignoring the huge gradients that we encounter inside bright areas)
    float intensity = min(starColor.r + starColor.g + starColor.b, 0.7);
 
    vec2 sgn = (vec2(vUv.xy)) * 2.0 - 1.0;
    vec2 gradient = vec2(dFdx(intensity) * sgn.x, dFdy(intensity) * sgn.y);
    float cutoff = max(max(gradient.x, gradient.y) - 0.1, 0.0);
    starColor *= max(1.0 - cutoff * 6.0, 0.3);
 
    // Motion blur; increases temporal coherence of undersampled flickering stars
    // and provides temporal filtering under true motion.  
    FragColor = vec4( starColor * color, 1.0 );
}
