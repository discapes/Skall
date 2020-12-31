#include <glad/glad.h>
#include "Defines.h"

struct alignas(sizeof(GLuint64)) Material {
    ALIGNED(float) shine;
    ALIGNED(GLuint64) diffuse;
    ALIGNED(GLuint64) specular;
};