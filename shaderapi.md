# Shader API
## Problem
* Create a standard interface that all shader's can use
* Allow shaders to be switched without changing the surrounding code
 
## Solution
* Layout for vertex data is will always be the same
* All resources in this document can be automatically loaded into a shader

## How to use
* Create a new `MappedProgram` from a `GLProgram` with `MappedProgram(std::move(program));`, or do `MappedProgram`&nbsp;`mp(Loader::BuildProgram("shader.vert",`&nbsp;`"shader.frag"));`
    * Program can be accessed at `mp.program`
* Send uniforms to the `MappedProgram` using it's methods
* Bind the required buffers before the draw call with `MappedProgram.BindData(const`&nbsp;`ShaderResourceManager&)`
  * Buffers are pulled from a `ShaderResourceManager`

## Vertex Layout
0. `vec3` Position
1. `vec3` Normal
2. `vec2` UV
   
## Available Uniforms
### Uniforms
```
layout(bindless_sampler) uniform;
layout(bindless_image) uniform;
...
uniform sampler2D tex_diffuse; // Diffuse texture map
uniform sampler2D tex_specular; // Specular texture map
uniform sampler2D tex_normal; // Normal map
```
Textures use `GL_ARB_bindless_texture`. Set the texture handles in the program with:
* `MappedProgram.SetDiffuseTex(GLuint64 tex_diffuse)`
* `MappedProgram.SetSpecularTex(GLuint64 tex_specular)`
* `MappedProgram.SetNormalTex(GLuint64 tex_normal)`


### Uniforms blocks
```
layout (std140, binding = 0) uniform Matrices {
    mat4 MVP; // Model View Projection
    mat4 MV; // Model View
};
```
Set the matrices in the buffer with:
* `ShaderResourceManager.updateMVPMatrix(mat4 MVP)`
* `ShaderResourceManager.updateMVMatrix(mat4 MV)`
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

layout (std430, binding = 0) buffer lights {
    int nPointFloats; 
    int nDirectionalFloats;
    int nSpotFloats;
    float data[];
};
```
The raw light object data is in an array of floats. The three integers tell how many floats there are for each type of light.  
Add new lights to the buffer with:
* `ShaderResourceManager.addLight(PointLight pl)`
* `ShaderResourceManager.addLight(DirectionalLight dl)`
* `ShaderResourceManager.addLight(SpotLight sl)`