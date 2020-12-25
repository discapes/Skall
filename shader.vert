#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out VStoFS{
    vec2 uv;
} fs ;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1);
    fs.uv = uv;
}