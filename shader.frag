#version 430 core
layout(bindless_sampler) uniform;
layout(bindless_image) uniform;

out vec4 color;

in VStoFS {
    vec3 pos; //view space
    vec2 uv;
} vs;

struct PointLight {
    vec3 pos;
};

struct DirectionalLight {
    vec3 dir;
};

struct Spotlight {
    vec3 dir;
    vec3 pos;
    float cutoff;
};

layout (std430, binding = 0) buffer lights {
    int nPoint;
    int nDirectional;
    int nSpot;
    float data[];
};

uniform sampler2D tex_diffuse;

void main()
{
    color = texture(tex, vs.uv);
} 