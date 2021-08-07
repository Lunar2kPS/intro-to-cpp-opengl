#include "OpenGLUtil.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    this->count = count;

    GLCALL(glGenBuffers(1, &rendererId));
    bind();
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCALL(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::bind() const {
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::unbind() const {
    //TODO: Use or not use NULL from vcruntime.h?
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
