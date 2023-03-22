#define ARENA_IMPLEMENTATION

#include "texterer.h"
#include "texterer_memory.h"

internal void ResizeWindowCallback(GLFWwindow* win, i32 w, i32 h);

int
main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Texterer", 0, 0);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindowCallback);

    glfwSwapInterval(1);

    while(!glfwWindowShouldClose(window)) {

        glClearColor(.5f, .8f, .9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

internal void
ResizeWindowCallback(GLFWwindow* win, i32 w, i32 h)
{
    glViewport(0, 0, w, h);
}
