#include "Wrappers.hpp"
#include "Mesh.hpp"
#include <string_view>

namespace Loader {
    GLTexture LoadTexture(std::string_view name);
    GLProgram BuildProgram(std::string_view vertexShader, std::string_view fragmentShader);
    Mesh LoadMesh(std::string_view name);
}