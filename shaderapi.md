# Shader API
## Goal
* Allow for custom shaders to be added easily and integrated into the program
  
## Design
* All vertex shaders have the same vertex input layout
* Available resources are listed here, the will automatically be sent to the driver by `Uniforms::LockAndLoad(program)`
  * `Uniforms` therefore requires all knowledge the shader itself might need

---
## Vertex Layout
0. `vec3` Position
1. `vec3` Normal
2. `vec2` UV
   
## Available Uniforms
### Uniforms
```
uniform sampler2D tex_diffuse;
uniform sampler2D tex_specular;
uniform sampler2D tex_normal;
```

### Uniforms blocks
```
layout (std140, binding = 0) uniform Matrices {
    mat4 MVP;
    mat4 MV;
};
```
### Shader Storage Buffer Objects
```
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

layout (std430, binding = 0) buffer Lights {
    int nPoint;
    int nDirectional;
    int nSpot;
    float lights[];
};
```