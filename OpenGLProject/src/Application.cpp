#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define DRAW_TRIANGLE 1
#define DRAW_RECTANGLE 0
#define WIREFRAME_MODE 0

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static const std::string vertex_shader_source = 
    "#version 330 core \n"
    "layout (location = 0) in vec3 pos; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); \n"
    "}";

static const std::string fragment_shader_source = 
    "#version 330 core \n"
    "out vec4 color; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
    "}";

static const std::string fragment_shader_source_1 = 
    "#version 330 core \n"
    "out vec4 color; \n"
    "\n"
    "void main() \n"
    "{ \n"
    "color = vec4(1.0f, 1.0f, 0.0f, 1.0f); \n"
    "}";

static int CreateShader(unsigned int type, const std::string& source)
{
    int result = glCreateShader(type);
    const char* shader_source = source.c_str();
    glShaderSource(result, 1, &shader_source, NULL);
    glCompileShader(result);
    int success;
    glGetShaderiv(result, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(result, 512, NULL, info_log);
        std::cout << "ERROR: Create Shader Failed! \n" << info_log << std::endl;
    }
    return result;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

#if (DRAW_TRIANGLE)
    float vertices_left[] = {
       -1.0f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
    };
    float vertices_right[] = {
        0.0f, -0.5f, 0.0f,
        1.0f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };

    unsigned int vertex_array_object_1;
    glGenVertexArrays(1, &vertex_array_object_1);

    unsigned int vertex_buffer_object_0;
    glGenBuffers(1, &vertex_buffer_object_0);
    unsigned int vertex_buffer_object_1;
    glGenBuffers(1, &vertex_buffer_object_1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_left), vertices_left, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(vertex_array_object_1);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_right), vertices_right, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

#elif (DRAW_RECTANGLE)
    float vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

#endif

#if (WIREFRAME_MODE)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    unsigned int vertex_shader;
    vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_source);
    unsigned int fragment_shader;
    fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_source);
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR: Link Program Failed! \n" << info_log << std::endl;
    }

#if (DRAW_TRIANGLE)
    unsigned int fragment_shader_1;
    fragment_shader_1 = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_source_1);
    unsigned int shader_program_1 = glCreateProgram();
    glAttachShader(shader_program_1, vertex_shader);
    glAttachShader(shader_program_1, fragment_shader_1);
    glLinkProgram(shader_program_1);
    glGetProgramiv(shader_program_1, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(shader_program_1, 512, NULL, info_log);
        std::cout << "ERROR: Link Program Failed! \n" << info_log << std::endl;
    }
    glDeleteShader(fragment_shader_1);
#endif
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Input */
        ProcessInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader_program);
        glBindVertexArray(vertex_array_object); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
#if (DRAW_TRIANGLE)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shader_program_1);
        glBindVertexArray(vertex_array_object_1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
#elif (DRAW_RECTANGLE)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean up */
    //glDeleteBuffers(1, &vertex_buffer_object);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}