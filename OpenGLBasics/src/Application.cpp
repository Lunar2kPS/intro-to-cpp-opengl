#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

using std::cout;
using std::endl;

/// <summary>
/// An example of drawing a triangle using legacy OpenGL 1.0, which didn't require glew.
/// </summary>
void drawLegacyTriangle();

int main() {
    //Initialize the library
    if (!glfwInit())
        return -1;

    //OpenGL 3.3, Core context specified below:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //As opposed to GLFW_OPENGL_COMPAT_PROFILE

    //NOTE: See the following for OpenGL Core vs. Compatibility context:
    //  https://www.reddit.com/r/opengl/comments/aln1jt/what_is_difference_between_core_profile_and/
    //  https://www.khronos.org/opengl/wiki/OpenGL_Context

    //The compatibility OpenGL profile makes Vertex Array Object (VAO) object 0 a DEFAULT OBJECT!
    //The core OpenGL profile makes Vertex Array Object (VAO) object 0 NOT AN OBJECT AT ALL!
    //      In core, you would need to create & bind one first, or else you'll receive OpenGL error 1282 (invalid operation) for glEnableVertexAttribArray(...).

    //Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //Important for making our framerate steady.. (VSync?)
    glfwSwapInterval(1);

    //NOTE: This requires a valid rendering context first
    if (glewInit() != GLEW_OK)
        return -1;

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

    {
        const int POSITION_COUNT = 8;
        float positions[POSITION_COUNT] = {
            0.5f,   -0.5f,
            -0.5f,  -0.5f,
            0.5f,   0.5f,
            -0.5f,  0.5f
        };

        const int INDEX_COUNT = 6;
        unsigned int indices[INDEX_COUNT] = {
            0, 1, 2,
            3, 2, 1
        };

        VertexArray va;
        VertexBuffer vb = VertexBuffer(positions, POSITION_COUNT * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        va.addBuffer(vb, layout);

        IndexBuffer ib = IndexBuffer(indices, INDEX_COUNT);

        Shader shader = Shader("res/shaders/Basic.glsl");
        shader.bind();
        shader.setUniform4f("uniColor", 0.2f, 0.6f, 0.8f, 1);

        float r = 0;
        float increment = 0.05f;

        //Unbind everything, just to demonstrate
        va.unbind();
        vb.unbind();
        ib.unbind();
        shader.unbind();

        Renderer renderer;

        //Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            //Render here
            renderer.clear();

            //Rebind everything
            shader.bind();
            shader.setUniform4f("uniColor", r, 0.6f, 0.8f, 1);

            renderer.draw(va, ib, shader);

            if (r > 1)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;
            r += increment;

            //Swap front and back buffers
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();
        }
    } //Delete our stack-allocated data BEFORE terminating GLFW/OpenGL context, so everything we were using is cleaned up first.

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
