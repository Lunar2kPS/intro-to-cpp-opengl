#include <GLFW/glfw3.h>

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

    //Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        //Render here
        glClear(GL_COLOR_BUFFER_BIT);

        //WARNING: Legacy OpenGL code here: just for testing purposes!
        glBegin(GL_TRIANGLES);

        //NOTE: Vertices here can range in [-1, 1] for the edges of the screen (xy).
        glVertex2f(-1, -1);
        glVertex2f(0, 0.5f);
        glVertex2f(0.5f, -0.5f);

        glEnd();
        //--- --- ---

        //Swap front and back buffers
        glfwSwapBuffers(window);

        //Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
