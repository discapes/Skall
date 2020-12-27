#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out VStoFS {
    vec3 pos;
    vec2 uv;
} fs;

layout (std140, binding = 0) uniform matrices {
    mat4 MVP;
    mat4 MV;
};

void main()
{
    gl_Position = MVP * vec4(pos, 1);
    fs.uv = uv;
}