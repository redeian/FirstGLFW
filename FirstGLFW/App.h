#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Vertex.h"

class App final
{
private:
    GLFWwindow * window = nullptr;
    std::vector<Vertex> vertices;

    GLuint programID;
    GLuint vertexArrayID;
    GLint mvp_location;
    GLuint textureID;
    GLuint normalID;
    GLuint lightPosID;

    int width;
    int height;
public:
    App();
    ~App();

    void init(int width, int height, const std::string& title);
    void initMesh();
    void initShaderProgram();
    void initTexture();
    void Start();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    
};

