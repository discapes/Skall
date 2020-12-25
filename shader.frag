#version 330 core
out vec4 color;

in VStoFS{
    vec2 uv;
} vs ;

uniform sampler2D tex;

void main()
{
    color = texture(tex, vs.uv);
} 