#include <GL/glut.h>
#include <cglm/cglm.h>

#include "../rot_qua/rot_qua.h"
#include "../rot_qua/vec3.h"

void display(void);
void onResize(int width, int height);
void mouse(int button, int state, int x, int y);
void move(int x, int y);

double eye[3] = {0.0, 2.0, 5.0};
double pov[3] = {0.0, 0.0, 0.0};
double worldUp[3] = {0.0, 1.0, 0.0};
double viewDir[3];
double rightDir[3];
double upDir[3];

int currentBtn = -1, lastX = 0, lastY = 0;

int main(int argc, char* argv[]) {
    sub(pov, eye, viewDir);
    nrm(viewDir, viewDir);
    crs(viewDir, worldUp, rightDir);
    crs(rightDir, viewDir, upDir);
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat light_position[] = {0.0f, 0.0f, -1.0f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], upDir[0],
              upDir[1], upDir[2]);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidTeapot(0.5f);
    glEnd();
    glFlush();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
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
    crs(viewDir, upDir, rightDir);
    nrm(rightDir, rightDir);
    nrm(upDir, upDir);
    if (currentBtn == GLUT_LEFT_BUTTON) {
        mul(-dx / 1000.0, rightDir, moveVec);
        add(moveVec, eye, eye);
        add(moveVec, pov, pov);
        mul(dy / 1000.0, upDir, moveVec);
        add(moveVec, eye, eye);
        add(moveVec, pov, pov);
    } else if (currentBtn == GLUT_MIDDLE_BUTTON) {
        mul(-dy / 1000.0, viewDir, moveVec);
        add(moveVec, eye, eye);
    } else if (currentBtn == GLUT_RIGHT_BUTTON) {
        double rad = -dx * M_PI / 2.0 / 100.0;
        rot(eye, upDir, pov, rad, eye);
        crs(rightDir, viewDir, upDir);

        rad = -dy * M_PI / 2.0 / 100.0;
        rot(eye, rightDir, pov, rad, eye);
        crs(rightDir, viewDir, upDir);
    }

    glutPostRedisplay();
}