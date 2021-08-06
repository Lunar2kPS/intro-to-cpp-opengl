#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCALL(glGenBuffers(1, &rendererId));
    bind();
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    GLCALL(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::bind() const {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const {
    //TODO: Use or not use NULL from vcruntime.h?
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
