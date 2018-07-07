#include "App.h"
#include <iostream>
#include <functional>

App::App()
{
}


App::~App()
{
    glfwTerminate();
}

void App::init( int width, int height, const std::string& title) 
{

    /*initialize the glfw system*/
    if (!glfwInit()) {
        glfwTerminate();
        return;
    }
    window = glfwCreateWindow( width, height, title.c_str(), 
                               nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this); // <-- this is new
    glfwSetKeyCallback(window, key_callback_static); // <-- call a static version


    GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::cout << "GLEW error:" <<
            glewGetErrorString(result) << std::endl;
        getchar();
        return;
    }

    const auto glVersion = glGetString(GL_VERSION);
    std::cout << "GL version : " << glVersion << std::endl;

    const GLubyte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "GLSL version : " << shaderVersion << std::endl;

    // some default OpenGL stages for alpha blending and depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void App::Start() {

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // Render color
        glClearColor(0.0f, 0.3f, 0.8f, 1.0f); // change color



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        std::cout << "I pressed E " << std::endl;
    }
        
}

void App::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    App* that = static_cast<App*>(glfwGetWindowUserPointer(window));
    that->key_callback(window, key, scancode, action, mods);
}