#include "OpenGLUtil.h"
#include "Renderer.h"

void Renderer::clear() const {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.bind();
    ib.bind();

    //MODERN OpenGL! Issuing a draw call!
    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, NULL)); //REQUIRES an index buffer, and NULL for using the already-bound GL_ELEMENT_ARRAY_BUFFER slot.
}
