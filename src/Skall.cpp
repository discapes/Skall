#include <iostream>
#include <time.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assert.h>
#include "Wrappers.hpp"
#include "Defines.h"
#include "Loader.hpp"
#include "GLDebugContext.hpp"
using namespace std;
using namespace glm;

struct Vertex
{
    vec3 pos;
    vec2 uv;
};

Vertex triangle[3] =
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
        {{0.0f, 0.5f, 0.0f}, {0.5f, 1.0f}}};

void run()
{
    assert(glfwInit());
    LOG("Initialized GLFW");
#ifdef DBGOUT
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window;
    window = glfwCreateWindow(800, 600, "DEMO", NULL, NULL);
    assert(window);
    LOG("Created window");
    glfwMakeContextCurrent(window);
    assert(gladLoadGL());
    LOG("Loaded OpenGL extensions");

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); });
#ifdef DBGOUT
    enableDebug();
#endif
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    GLProgram program = Loader::BuildProgram("shader.vert", "shader.frag");
    GLTexture texture = Loader::LoadTexture("texture.jpg");

    LOG("Initialization complete")
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        VAO.bind();
        VBO.bind(0, 0, sizeof(Vertex));

        program.use();
        texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle) / sizeof(Vertex));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main()
{
    srand(time(0));
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << endl;
    run();
    cout << endl;
    return 0;
}