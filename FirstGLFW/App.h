#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App final
{
private:
    GLFWwindow * window = nullptr;
    int x;
public:
    App();
    ~App();

    void init(int width, int height, const std::string& title);
    void Start();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    
};

