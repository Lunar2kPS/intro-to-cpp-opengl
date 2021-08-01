#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

    const int POSITION_COUNT = 12;
    float positions[POSITION_COUNT] = {
        -0.5f,  -0.5f,
        0.5f,   0.5f,
        0.5f,   -0.5f,

        0.5f,   0.5f,
        -0.5f,  -0.5f,
        -0.5f,  0.5f
    };

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    
    //NOTE: glBindBuffer(...) MUST be called in order for this next line to WORK!
    glBufferData(GL_ARRAY_BUFFER, POSITION_COUNT * sizeof(float), positions, GL_STATIC_DRAW);

    //Call this PER vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    //This would UNBIND the current buffer.
    //Binding is like "selecting" stuff in Photoshop. You need to select stuff before you can do anything with it.
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    ShaderProgramSource source = parseShader("res/shaders/Basic.glsl");
    cout << "VERTEX SHADER:" << endl;
    cout << source.vertexSource << endl;
    cout << "FRAGMENT SHADER:" << endl;
    cout << source.fragmentSource << endl;

    unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        //Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //MODERN OpenGL! Issuing a draw call!
        //2 ways to draw:
        glDrawArrays(GL_TRIANGLES, 0, POSITION_COUNT / 2);
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
