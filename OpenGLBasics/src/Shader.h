#pragma once

#include <string>

using namespace std;

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

class Shader {
    private:
    string filePath;
    unsigned int rendererId;

    public:
    Shader(const string& fileName);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const string& parameterName, float f0, float f1, float f2, float f3);

    private:
    int getUniformLocation(const string& parameterName);
    unsigned int compileShader(unsigned int type, string& source);
    unsigned int createShader(string& vertexShader, string& fragmentShader);
    ShaderProgramSource parseShader(const string& filePath);
};
