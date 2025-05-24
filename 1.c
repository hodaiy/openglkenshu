#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

void CMYK2RGB(float c, float m, float y, float k, vec3 rgb);

int main(void) {
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW3", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vec3 cyan, yellow;
        CMYK2RGB(1.0f, 0.0f, 0.0f, 0.0f, cyan);
        CMYK2RGB(0.0f, 0.0f, 1.0f, 0.0f, yellow);
        glBegin(GL_TRIANGLES);
        glColor3f(cyan[0], cyan[1], cyan[2]);
        glVertex2f(-1.0f, -0.5f);
        glVertex2f(0.0f, -0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(yellow[0], yellow[1], yellow[2]);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.0f, -0.5f);
        glVertex2f(1.0f, 0.5f);
        glVertex2f(1.0f, -0.5f);
        glEnd();
        glFlush();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void CMYK2RGB(float c, float m, float y, float k, vec3 rgb) {
    rgb[0] = 1.0f - glm_min(1.0f, c * (1.0f - k) + k);
    rgb[1] = 1.0f - glm_min(1.0f, m * (1.0f - k) + k);
    rgb[2] = 1.0f - glm_min(1.0f, y * (1.0f - k) + k);
}