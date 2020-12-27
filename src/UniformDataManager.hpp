#include "Wrappers.hpp"

class UniformDataManager
{
    GLBuffer MatrixUBO;
    GLBuffer LightSSBO;
public:
	UniformDataManager() {
        // init buffers
    }
    const GLBuffer& Matrices() const { return MatrixUBO; }
    const GLBuffer& Lights() const { return LightSSBO; }

};