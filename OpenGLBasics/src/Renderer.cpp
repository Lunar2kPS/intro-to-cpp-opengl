#include <iostream>

#include "Renderer.h"

using namespace std;

void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        cout << "[OpenGL Error] (" << error << "):\n" << function << "\n" << file << ":" << line << endl;
        return false;
    }
    return true;
}