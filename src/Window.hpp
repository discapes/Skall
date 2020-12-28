#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Window
{
    void SetWin(GLFWwindow* newWin);
    bool KeyDown(int key);
    glm::dvec2 CursorPos();
    void SetCursorPos(glm::dvec2 pos);
    void CenterCursor();

    double LastFrameDuration();
    double FPS();
    void NewFrame();
}