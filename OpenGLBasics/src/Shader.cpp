#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

#include "OpenGLUtil.h"
#include "Shader.h"

using namespace std;

Shader::Shader(const string& filePath)
    : filePath(filePath),
    rendererId(0) {
    
    ShaderProgramSource source = parseShader("res/shaders/Basic.glsl");
    cout << "VERTEX SHADER:" << endl;
    cout << source.vertexSource << endl;
    cout << "FRAGMENT SHADER:" << endl;
    cout << source.fragmentSource << endl;

    rendererId = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    GLCALL(glDeleteProgram(rendererId));
}

void Shader::bind() const {
    GLCALL(glUseProgram(rendererId));
}

void Shader::unbind() const {
    GLCALL(glUseProgram(0));
}

void Shader::setUniform4f(const string& parameterName, float v0, float v1, float v2, float v3) {
    GLCALL(glUniform4f(getUniformLocation(parameterName), v0, v1, v2, v3));
}

int Shader::getUniformLocation(const string& parameterName) {
    GLCALL(int location = glGetUniformLocation(rendererId, parameterName.c_str()));
    return location;
}

unsigned int Shader::compileShader(unsigned int type, string& source) {
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

unsigned int Shader::createShader(string& vertexShader, string& fragmentShader) {
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

ShaderProgramSource Shader::parseShader(const string& filePath) {
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

    return ShaderProgramSource{
        ss[(int) ShaderType::VERTEX].str(),
        ss[(int) ShaderType::FRAGMENT].str()
    };
}
