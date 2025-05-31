#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include "../rot_qua/rot_qua.h"
#include "../rot_qua/vec3.h"

double eye[3] = {0.0, 0.0, 5.0};
double pov[3] = {0.0, 0.0, 0.0};
double up[3] = {0.0, 1.0, 0.0};
double cup[3] = {0.0, 1.0, 0.0};
double fovh = 100.0, fovv = 60.0;

int currentBtn = -1, lastX = 0, lastY = 0;

void display(void);
void onResize(int width, int height);
void mouse(int button, int state, int x, int y);
void move(int x, int y);

int main(int argc, char* argv[]) {
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glutMouseFunc(mouse);
    glutMotionFunc(move);
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutMainLoop();
    return 0;
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], up[0], up[1],
              up[2]);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidTeapot(1.5f);
    glutSwapBuffers();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovv, fovh / fovv, 1.0, 100.0);
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        currentBtn = button;
        lastX = x;
        lastY = y;
    }
}

void move(int x, int y) {
    double viewDir[3], rightDir[3], moveVec[3];
    int dx = x - lastX;
    int dy = y - lastY;
    lastX = x;
    lastY = y;

    sub(pov, eye, viewDir);
    nrm(viewDir, viewDir);
    crs(viewDir, up, rightDir);
    nrm(rightDir, rightDir);
    nrm(up, up);
    if (currentBtn == GLUT_LEFT_BUTTON) {
        mul(-dx / 1000.0, rightDir, moveVec);
        add(moveVec, eye, eye);
        add(moveVec, pov, pov);
        mul(dy / 1000.0, up, moveVec);
        add(moveVec, eye, eye);
        add(moveVec, pov, pov);
    } else if (currentBtn == GLUT_MIDDLE_BUTTON) {
        mul(-dy / 1000.0, viewDir, moveVec);
        add(moveVec, eye, eye);
    } else if (currentBtn == GLUT_RIGHT_BUTTON) {
        double rad = -dx * M_PI / 2.0 / 100.0;
        rot(eye, up, pov, rad, eye);
        crs(rightDir, viewDir, up);

        rad = -dy * M_PI / 2.0 / 100.0;
        rot(eye, rightDir, pov, rad, eye);
        crs(rightDir, viewDir, up);
    }

    glutPostRedisplay();
}