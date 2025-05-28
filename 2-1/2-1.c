#include <GL/glut.h>
#include <cglm/cglm.h>

GLfloat vertex[][3] = {
    {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f},   {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, 0.5f},   {0.5f, -0.5f, 0.5f},   {-0.5f, 0.5f, 0.5f},
    {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},   {-0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
};

void display(void);
void onResize(int width, int height);

int main(int argc, char* argv[]) {
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutMainLoop();
    return 0;
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 13; i++) glVertex3fv(vertex[i]);
    glEnd();
    glFlush();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glLoadIdentity();
    // glOrtho(-width / 200.0f, width / 200.0f, -height / 200.0f, height
    // /200.0f,-1.0f, 1.0f);
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    gluLookAt(2.0, 2.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}