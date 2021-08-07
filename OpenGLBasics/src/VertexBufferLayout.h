#pragma once

#include <vector>
#include <GL/glew.h>
#include "OpenGLUtil.h"

using namespace std;

struct VertexBufferAttribute {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
    private:
    vector<VertexBufferAttribute> attributes;
    unsigned int stride;

    public:
    VertexBufferLayout()
        : stride(0) { }

    inline unsigned int getStride() const { return stride; };
    inline const vector<VertexBufferAttribute>& GetAttributes() const { return attributes; }

    template<typename T>
    void push(unsigned int count) {
        static_assert(false);
    }

    template<> void push<float>(unsigned int count);
    template<> void push<unsigned int>(unsigned int count);
    template<> void push<unsigned char>(unsigned int count);
};
