#version 430 core

in vec2 vTexCoords;

uniform bool u_bFlipFlop;

uniform vec2 u_resolution;
uniform int u_resolutionX;
uniform int u_resolutionY;

uniform float u_cellRoundness;
uniform float u_cellThreshold;
uniform float u_cellBlur;

layout(std430, binding=0) buffer myBuffer0 {
   int data0[];
};

layout(std430, binding=1) buffer myBuffer1 {
   int data1[];
};

int isAlive(int x, int y) {
    if (x < 0 || x >= u_resolutionX ||
        y < 0 || y >= u_resolutionY)
    {
       return 0;
    }
    return u_bFlipFlop 
        ? data1[x + y * u_resolutionX]
        : data0[x + y * u_resolutionX];
}

float distTo(int x, int y) {
    return smoothstep(u_cellRoundness, 0., length(vTexCoords * u_resolution - vec2(x, y) - vec2(0.5)));
}

float colorContribution(int x, int y) {
    return isAlive(x, y) == 1 ? distTo(x, y) : 0.;
}

void main() {
    ivec2 gid = ivec2(floor(vTexCoords * u_resolution));
    // Background color
    const vec3 bgCol = vec3(7, 0, 46) / 255.;
    // Alive color
    float t = 0.;
    t += colorContribution(gid.x + 1, gid.y + 1);
    t += colorContribution(gid.x + 1, gid.y + 0);
    t += colorContribution(gid.x + 1, gid.y - 1);
    t += colorContribution(gid.x + 0, gid.y + 1);
    t += colorContribution(gid.x + 0, gid.y + 0);
    t += colorContribution(gid.x + 0, gid.y - 1);
    t += colorContribution(gid.x - 1, gid.y + 1);
    t += colorContribution(gid.x - 1, gid.y + 0);
    t += colorContribution(gid.x - 1, gid.y - 1);
    t = smoothstep(u_cellThreshold - u_cellBlur, u_cellThreshold + u_cellBlur, t);
    vec3 col = mix(bgCol, vec3(1.), t);
    //
    gl_FragColor = vec4(col, 1.);
}