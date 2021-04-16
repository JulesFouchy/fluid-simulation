#version 430 core

in vec2 vTexCoords;

uniform bool u_bFlipFlop;

uniform vec2 u_resolution;
uniform int u_resolutionX;
uniform int u_resolutionY;

layout(std430, binding=0) buffer myBuffer0 {
   float data0[];
};

layout(std430, binding=1) buffer myBuffer1 {
   float data1[];
};

float sample_grid(int x, int y) {
    if (x < 0 || x >= u_resolutionX ||
        y < 0 || y >= u_resolutionY)
    {
       return 0.;
    }
    return u_bFlipFlop 
        ? data1[x + y * u_resolutionX]
        : data0[x + y * u_resolutionX];
}


void main() {
    ivec2 gid = ivec2(floor(vTexCoords * u_resolution));
    // Background color
    const vec3 bgCol = vec3(7, 0, 46) / 255.;
    // Alive color
    vec3 col = vec3(sample_grid(gid.x, gid.y));
    //
    gl_FragColor = vec4(col, 1.);
}