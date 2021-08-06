#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace std;

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

/// <summary>
/// An example of drawing a triangle using legacy OpenGL 1.0, which didn't require glew.
/// </summary>
void drawLegacyTriangle();

ShaderProgramSource parseShader(const string& filePath);
unsigned int createShader(string& vertexShader, string& fragmentShader);

int main() {
    //Initialize the library
    if (!glfwInit())
        return -1;

    //OpenGL 3.3, Core context specified below:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //As opposed to GLFW_OPENGL_COMPAT_PROFILE

    //NOTE: See the following for OpenGL Core vs. Compatibility context:
    //  https://www.reddit.com/r/opengl/comments/aln1jt/what_is_difference_between_core_profile_and/
    //  https://www.khronos.org/opengl/wiki/OpenGL_Context

    //The compatibility OpenGL profile makes Vertex Array Object (VAO) object 0 a DEFAULT OBJECT!
    //The core OpenGL profile makes Vertex Array Object (VAO) object 0 NOT AN OBJECT AT ALL!
    //      In core, you would need to create & bind one first, or else you'll receive OpenGL error 1282 (invalid operation) for glEnableVertexAttribArray(...).

    //Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Important for making our framerate steady.. (VSync?)
    glfwSwapInterval(1);

    //NOTE: This requires a valid rendering context first
    if (glewInit() != GLEW_OK)
        return -1;

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

    {
        const int POSITION_COUNT = 8;
        float positions[POSITION_COUNT] = {
            0.5f,   -0.5f,
            -0.5f,  -0.5f,
            0.5f,   0.5f,
            -0.5f,  0.5f
        };

        const int INDEX_COUNT = 6;
        unsigned int indices[INDEX_COUNT] = {
            0, 1, 2,
            3, 2, 1
        };

        //vao = Vertex Array Object
        //Creating our VAO is required in OpenGL Core context, since the default is invalid, as opposed to a valid default in OpenGL Compatibility context.
        unsigned int vao;
        GLCALL(glGenVertexArrays(1, &vao));
        GLCALL(glBindVertexArray(vao));

        //vbo = Vertex Buffer Object
        VertexBuffer vb = VertexBuffer(positions, POSITION_COUNT * sizeof(float));

        //Call this PER vertex attribute
        GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL));
        GLCALL(glEnableVertexAttribArray(0));

        IndexBuffer ib = IndexBuffer(indices, INDEX_COUNT);

        //This would UNBIND the current buffer.
        //Binding is like "selecting" stuff in Photoshop. You need to select stuff before you can do anything with it.
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        ShaderProgramSource source = parseShader("res/shaders/Basic.glsl");
        cout << "VERTEX SHADER:" << endl;
        cout << source.vertexSource << endl;
        cout << "FRAGMENT SHADER:" << endl;
        cout << source.fragmentSource << endl;

        unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
        GLCALL(glUseProgram(shader));

        //Demonstrating how to retrieve and set a shader uniform variable!
        GLCALL(int uniColorLocation = glGetUniformLocation(shader, "uniColor"));

        //This would happen if there was no "uniColor" uniform, OR it was stripped out of the shader since it was unused.
        ASSERT(uniColorLocation != -1);

        GLCALL(glUniform4f(uniColorLocation, 0.2f, 0.6f, 0.8f, 1));
        float r = 0;
        float increment = 0.05f;

        //Unbind everything
        GLCALL(glBindVertexArray(NULL));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));
        GLCALL(glBindBuffer(GL_ARRAY_BUFFER, NULL));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            //Render here
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            //Rebind everything
            GLCALL(glUseProgram(shader));
            GLCALL(glUniform4f(uniColorLocation, r, 0.6f, 0.8f, 1));

            GLCALL(glBindVertexArray(vao));
            ib.bind();

            //MODERN OpenGL! Issuing a draw call!
            GLCALL(glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, NULL)); //REQUIRES an index buffer, and NULL for using the already-bound GL_ELEMENT_ARRAY_BUFFER slot.

            if (r > 1)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;
            r += increment;

            //Swap front and back buffers
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();
        }

        GLCALL(glDeleteProgram(shader));
    } //Delete our stack-allocated data BEFORE terminating GLFW/OpenGL context, so everything we were using is cleaned up first.

    glfwTerminate();
    return 0;
}

void drawLegacyTriangle() {
    glBegin(GL_TRIANGLES);

    //NOTE: Vertices here can range in [-1, 1] for the edges of the screen (xy).
    glVertex2f(-1, -1);
    glVertex2f(0, 0.5f);
    glVertex2f(0.5f, -0.5f);

    glEnd();
}

ShaderProgramSource parseShader(const string& filePath) {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    ifstream stream = ifstream(filePath);
    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int) type] << line << '\n';
        }
    }

    return ShaderProgramSource {
        ss[(int) ShaderType::VERTEX].str(),
        ss[(int) ShaderType::FRAGMENT].str()
    };
}

unsigned int compileShader(unsigned int type, string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    //TODO: Error handling for this shader
    int result;

    //NOTE: iv means int, vector.
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        //Can't do this in C++, ewww... so..
        //char message[length];

        //Do THIS instead! >:)
        //Allocate on the STACK still!! YAY TheCherno!!
        char* message = (char*) alloca(length * sizeof(char));

        GLCALL(glGetShaderInfoLog(id, length, &length, message));
        cout << "Failed to compile a shader!" << endl;
        cout << message << endl;

        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int createShader(string& vertexShader, string& fragmentShader) {
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));

    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    //TODO: Detach shaders after compiling? Maybe covered in a later TheCherno episode (after episode 7)

    return program;
}
