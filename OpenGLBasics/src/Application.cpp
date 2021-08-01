#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

/// <summary>
/// An example of drawing a triangle using legacy OpenGL 1.0, which didn't require glew.
/// </summary>
void drawLegacyTriangle();

unsigned int createShader(string& vertexShader, string& fragmentShader);

int main() {
    //Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //NOTE: This requires a valid rendering context first
    if (glewInit() != GLEW_OK)
        return -1;

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

    const int POSITION_COUNT = 6;
    float positions[POSITION_COUNT] = {
        -1,     -1,
        0,      0.5f,
        0.5f,   -0.5f
    };

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBufferData(GL_ARRAY_BUFFER, POSITION_COUNT * sizeof(float), positions, GL_STATIC_DRAW); //NOTE: See docs.gl for OpenGL documentation!!
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); //This is the CURRENTLY-bound

    //Call this PER vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    //This would UNBIND the current buffer.
    //Binding is like "selecting" stuff in Photoshop. You need to select stuff before you can do anything with it.
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    string vertexShader =
        "#version 330 core\n" //GLSL version 330, core means it won't let us use any deprecated functions
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main() {\n"
        "   gl_Position = position;\n"
        "}\n";

    string fragmentShader =
        "#version 330 core\n" //GLSL version 330, core means it won't let us use any deprecated functions
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main() {\n"
        "   color = vec4(1, 0, 0, 1);\n"
        "}\n";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        //Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //MODERN OpenGL! Issuing a draw call!
        //2 ways to draw:
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 3, ...); //REQUIRES an index buffer, which we didn't do yet!

        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }

    glDeleteProgram(shader);

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

unsigned int compileShader(unsigned int type, string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //TODO: Error handling for this shader
    int result;

    //NOTE: iv means int, vector.
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        //Can't do this in C++, ewww... so..
        //char message[length];

        //Do THIS instead! >:)
        //Allocate on the STACK still!! YAY TheCherno!!
        char* message = (char*) alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed to compile a shader!" << endl;
        cout << message << endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShader(string& vertexShader, string& fragmentShader) {
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    //TODO: Detach shaders after compiling? Maybe covered in a later TheCherno episode (after episode 7)

    return program;
}
