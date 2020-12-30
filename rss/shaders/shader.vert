#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out VStoFS {
    vec3 pos; // view space
    vec3 rawNormal;
    vec2 uv;
} fs;

uniform mat4 M;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(pos, 1);
    fs.pos = (M * vec4(pos, 1)).xyz;
    fs.rawNormal = mat3(transpose(inverse(M))) * normal; 
    fs.uv = uv;
}