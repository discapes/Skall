#pragma once
#include <vector>
#include <tuple>
#include "SSBO.hpp"
#include "Camera.hpp"
#include "Defines.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

class Renderer {
    struct LightVars {
        ALIGNED(vec3) ambient;
        ALIGNED(bool) flashlight;
        ALIGNED(uint) nLights;
    };
    SSBO<LightVars, Light> lightSSBO;
    std::vector<Mesh> meshes;
    GLVertexArray VAO;
    GLProgram program;
    Camera camera;

    Renderer() {
        VAO.VertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
		VAO.VertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
		VAO.VertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
		VAO.VertexAttribBinding(0, 0);
		VAO.VertexAttribBinding(1, 0);
		VAO.VertexAttribBinding(2, 0);
		VAO.EnableVertexAttrib(0);
		VAO.EnableVertexAttrib(1);
		VAO.EnableVertexAttrib(2);
    }

    void Render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        lightSSBO.Buffer().BindBase(GL_SHADER_STORAGE_BUFFER, 0);
        im.SetCamPos(camera.Pos());
		im.SetCamDir(camera.Forward());
        for (Mesh& mesh : meshes) {
            mat4 M = mesh.modelMatrix;
			mat4 MVP = camera.ProjMatrix() * camera.ViewMatrix() * M;
			im.SetM(M);
			im.SetMVP(MVP);
            mesh.materials.Buffer().BindBase(GL_SHADER_STORAGE_BUFFER, 1);
            VAO.BindElementBuffer(mesh.VBO);
	        VAO.BindVertexBuffer(0, mesh.VBO, mesh.lengthIndices, sizeof(Vertex));
	        glDrawElements(GL_TRIANGLES, mesh.lengthIndices / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        }

        Window::SwapBuffers();
    }
};