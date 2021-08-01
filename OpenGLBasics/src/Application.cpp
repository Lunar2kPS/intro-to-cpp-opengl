#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

/// <summary>
/// An example of drawing a triangle using legacy OpenGL 1.0, which didn't require glew.
/// </summary>
void drawLegacyTriangle();

int main() {
    //Initialize the library
    if (!glfwInit())
        return -1;

    //Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current
    glfwMakeContextCurrent(window);

    //NOTE: This requires a valid rendering context first
    if (glewInit() != GLEW_OK)
        return -1;

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

    const int POSITION_COUNT = 6;
    float positions[POSITION_COUNT] = {
        -1,     -1,
        0,      0.5f,
        0.5f,   -0.5f
    };

    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBufferData(GL_ARRAY_BUFFER, POSITION_COUNT * sizeof(float), positions, GL_STATIC_DRAW); //NOTE: See docs.gl for OpenGL documentation!!
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); //This is the CURRENTLY-bound

    //Call this PER vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    //This would UNBIND the current buffer.
    //Binding is like "selecting" stuff in Photoshop. You need to select stuff before you can do anything with it.
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        //Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //MODERN OpenGL! Issuing a draw call!
        //2 ways to draw:
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 3, ...); //REQUIRES an index buffer, which we didn't do yet!

        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }

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
