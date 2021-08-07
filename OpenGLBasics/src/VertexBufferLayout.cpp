#include "VertexBufferLayout.h"

template<> void VertexBufferLayout::push<float>(unsigned int count) {
    //C++ Brace Initialization (works on classes AND structs)
    VertexBufferAttribute attribute = {
        GL_FLOAT,
        count,
        GL_FALSE
    };
    attributes.push_back(attribute);
    stride += count * VertexBufferAttribute::GetSizeOfType(GL_FLOAT);
}

template<> void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    VertexBufferAttribute attribute = {
        GL_UNSIGNED_INT,
        count,
        GL_FALSE
    };
    attributes.push_back(attribute);
    stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT);
}

template<> void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    VertexBufferAttribute attribute = {
        GL_UNSIGNED_BYTE,
        count,
        GL_TRUE
    };
    attributes.push_back(attribute);
    stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
}
