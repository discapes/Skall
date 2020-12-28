#version 430 core
#extension GL_ARB_bindless_texture : enable
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
    int nPointFloats; 
    int nDirectionalFloats;
    int nSpotFloats;
    float data[];
};

uniform sampler2D tex_diffuse;

void main()
{
    color = texture(tex_diffuse, vs.uv);
} 