// For documentation about this code, see the glfw3 quick start guide
//   https://www.glfw.org/docs/3.3/quick.html

#include "AGL.h"
#include "ppm_image.h"
#include <cmath>
#include <iostream>

extern void ray_trace(agl::ppm_image &image);

const GLchar *vertexShader[] =
        {
                "#version 400\n"
                "in vec3 VertexPosition;"
                "out vec2 uv;"
                "void main() {"
                "  uv = VertexPosition.xy * 0.5 + vec2(0.5);"
                "  uv.y = 1 - uv.y;"
                "  gl_Position = vec4(VertexPosition, 1.0);"
                "}"
        };

const GLchar *fragmentShader[] =
        {
                "#version 400\n"
                "uniform sampler2D image;"
                "in vec2 uv;"
                "out vec4 FragColor;"
                "void main() { FragColor = vec4(texture(image, uv).rgb, 1.0); }"
        };

static void PrintShaderErrors(GLuint id, const std::string label) {
    std::cerr << label << " failed\n";
    GLint logLen;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        char *log = (char *) malloc(logLen);
        GLsizei written;
        glGetShaderInfoLog(id, logLen, &written, log);
        std::cerr << "Shader log: " << log << std::endl;
        free(log);
    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char **argv) {
    GLFWwindow *window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    int width = 0.5 * 1280;
    int height = 0.5 * 720;
    window = glfwCreateWindow(width, height, "Image Viewer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    // Tell GLFW to call our callback fun when a key is pressed
    glfwSetKeyCallback(window, key_callback);

#ifndef APPLE
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }
#endif

    GLint result;
    GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshaderId, 1, vertexShader, NULL);
    glCompileShader(vshaderId);
    glGetShaderiv(vshaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        PrintShaderErrors(vshaderId, "Vertex shader");
        return -1;
    }

    GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshaderId, 1, fragmentShader, NULL);
    glCompileShader(fshaderId);
    glGetShaderiv(fshaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        PrintShaderErrors(fshaderId, "Fragment shader");
        return -1;
    }

    GLuint shaderId = glCreateProgram();
    glAttachShader(shaderId, vshaderId);
    glAttachShader(shaderId, fshaderId);
    glLinkProgram(shaderId);
    glGetShaderiv(shaderId, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        PrintShaderErrors(shaderId, "Shader link");
        return -1;
    }

    glUseProgram(shaderId);

    // Define a square that will cover the entire screen
    const float positions[] =
            {
                    -1.0f, -1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    -1.0f, 1.0f, 0.0f
            };

    GLuint vboId;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);
    glEnableVertexAttribArray(0); // 0 -> from our glBindAttribLocation call
    glBindBuffer(GL_ARRAY_BUFFER, vboId); // as a habit -> always bind before setting data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);

    agl::ppm_image image(width, height);
    ray_trace(image);
    std::cout << "Loaded image: " << image.width() << "x" << image.height() << std::endl;

    glEnable(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0);

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, image.width(), image.height());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
                    GL_RGB, GL_UNSIGNED_BYTE, image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint locId = glGetUniformLocation(shaderId, "image");
    glUniform1i(locId, 0);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the buffers

        // draw square
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

