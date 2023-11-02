#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct _Model
{
    std::vector<float> vertices;
    uint VBO;
    uint VAO;
} Model;

Model* models[] = {
    new Model(),
    new Model(),
};

unsigned int shaderProgram;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initShader()
{
    int success = false;
    char infoLog[512];
    // shader
    const char *vertextShaderSource = R"fs(#version 330 core
                                      layout (location = 0) in vec3 aPos;

                                      void main()
                                      {
                                         gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                                      })fs";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertextShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    const char *fragmentShaderSource = R"fs(#version 330 core
                                       out vec4 FragColor;

                                       void main()
                                       {
                                          FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                                       })fs";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void initModel(Model* model)
{
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);

    // bind vertex array object
    glBindVertexArray(model->VAO);

    // copy vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(float), &*model->vertices.begin(), GL_STATIC_DRAW);

    // set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render()
{
    // Clear Buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    for (size_t i = 0; i < 2; i++)
    {
        glBindVertexArray(models[i]->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void error(int error_code, const char* description)
{
    std::cout << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    models[0]->vertices = {
        -0.7f, -0.5f, 0.0f,
        -0.3f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f};

    models[1]->vertices = {
        0.3f, -0.5f, 0.0f,
        0.7f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f};

    if (!glfwInit())
    {
        std::cout << "glfw init error" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "glfw create window error" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetErrorCallback((GLFWerrorfun)error);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glfw load gll error" << std::endl;
        return -1;
    }

    initShader();
    initModel(models[0]);
    initModel(models[1]);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (size_t i = 0; i < 2; i++)
    {
        glDeleteVertexArrays(1, &models[i]->VAO);
        glDeleteBuffers(1, &models[i]->VBO);
    }
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}