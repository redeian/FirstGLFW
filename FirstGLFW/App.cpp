#include "App.h"
#include <iostream>
#include <functional>

#include "thirdparty/glm/glm/gtc/matrix_transform.hpp"
#include "thirdparty/glm/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void linkCheck(GLuint ID) {
    GLint linkStatus, validateStatus;
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        cout << "Shader Linking FAILED" << endl;
        GLchar messages[256];
        glGetProgramInfoLog(ID, sizeof(messages), 0, &messages[0]);
        cout << messages;
    }

    glValidateProgram(ID);
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &validateStatus);

    cout << "Link: " << linkStatus << "  Validate: " << validateStatus << endl;
    if (linkStatus == GL_FALSE) {
        cout << "Shader Validation FAILED" << endl;
        GLchar messages[256];
        glGetProgramInfoLog(ID, sizeof(messages), 0, &messages[0]);
        cout << messages;
    }
}

App::App()
{
}


App::~App()
{
    glfwTerminate();
}

void App::init( int width, int height, const std::string& title) 
{
    this->width = width;
    this->height = height;

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

    initShaderProgram();
    initMesh();
    initTexture();
}

void App::initMesh() {
    // In the CPU RAM
    vertices.push_back( Vertex(-0.6f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 0,0));
    vertices.push_back( Vertex( 0.6f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0,1));
    vertices.push_back( Vertex( 0.0f,  0.6f, 0.0f, 0.0f, 1.0f, 0.0f, 1,1));

    // Move to GPU RAM (VRAM)
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    GLuint bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
        vertices.data(), GL_STATIC_DRAW);

    mvp_location = glGetUniformLocation(programID, "MVP");
    GLint vpos_location = glGetAttribLocation(programID, "vPos");
    GLint vcolor_location = glGetAttribLocation(programID, "vCol");
    GLint vtex_location = glGetAttribLocation(programID, "vTex");

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcolor_location);
    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT,
        GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(vcolor_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)(sizeof(float) * 3));
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)(sizeof(float) * 6));
}

void App::initTexture() {
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // load a texture
    int imgWidth;
    int imgHeight;
    int n;
    unsigned char *data = stbi_load("test2.png", &imgWidth, &imgHeight, &n, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, imgWidth, imgHeight, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void App::initShaderProgram() {
    //1. write two shader codes
    static const char* vertexShaderCode =
        "uniform mat4 MVP; \n"
        "attribute vec3 vPos; \n"
        "attribute vec3 vCol; \n"
        "attribute vec2 vTex; \n"
        "varying vec3 color; \n"
        "varying vec2 texCoord; \n"
        "void main() \n "
        "{ \n"
        "   gl_Position = MVP * vec4(vPos, 1.0); \n"
        "   color = vCol; \n"
        "   texCoord = vTex; \n"
        "} \n";

    static const char* fragmentShaderCode =
        "uniform sampler2D texture; \n"
        "varying vec3 color; \n"
        "varying vec2 texCoord; \n"
        "void main() \n"
        "{ \n"
        "    gl_FragColor = texture2D( texture, texCoord);\n"
        "} \n";

    //2. load the shader codes
    programID = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    //3. compile the codes
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    //3.1 Check the compiling code
    // https://d.pr/D63ZbS

    //4. attech the codes
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    //5. link the codes
    glLinkProgram(programID);

    linkCheck(programID);

}


void App::Start() {

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Render color
        glClearColor(0.0f, 0.3f, 0.8f, 1.0f); // change color

        float ratio = width / static_cast<float>(height);
        glViewport(0, 0, width, height);

        // initialize transformation
        const glm::vec3 axis(0, 0, 1);
        glm::vec3 trs(0, 0, 0);
        glm::vec3 vscale(1, 1, 1);

        // create a model matrix
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), trs);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 
                            (glm::mediump_float)glfwGetTime(), axis);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), vscale);
        glm::mat4x4 m = translation * rotation * scale;
        glm::mat4x4 vp = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glm::mat4x4 mvp = m*vp;

        glUseProgram(programID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(vertexArrayID);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

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