#include "OpenGLUtil.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
    GLCALL(glGenVertexArrays(1, &rendererId));
}

VertexArray::~VertexArray() {
    GLCALL(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const vector<VertexBufferAttribute>& attributes = layout.GetAttributes();

    unsigned int offset = 0;
    for (unsigned int i = 0; i < attributes.size(); i++) {
        const VertexBufferAttribute attribute = attributes[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized, layout.getStride(), (const void*) offset));
        offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
    }

}

void VertexArray::bind() const {
    GLCALL(glBindVertexArray(rendererId));
}

void VertexArray::unbind() const {
    GLCALL(glBindVertexArray(0));
}
