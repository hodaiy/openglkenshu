#include <GL/glut.h>
#include <cglm/cglm.h>
#include <math.h>
#include <string.h>

#include "../rot_qua/rot_qua.h"
#include "../rot_qua/vec3.h"

void loadObjNormal(const char* filename);
void display(void);
void onResize(int width, int height);
void mouse(int button, int state, int x, int y);
void move(int x, int y);

float* vertices;
unsigned int* indices;
float* vnormals;
int vcount = 0, icount = 0;

double eye[3] = {0.0, 0.2, 0.2};
double pov[3] = {0.0, 0.1, 0.0};
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
    loadObjNormal("bun_zipperNormal_res4.obj");
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

void loadObjNormal(const char* filename) {
    FILE* fp;
    char line[50];

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        char* token = strtok(line, " ");
        if (!token) continue;
        if (strcmp(token, "v") == 0)
            vcount++;
        else if (strcmp(token, "f") == 0)
            icount++;
    }

    vertices = malloc(vcount * 3 * sizeof(float));
    vnormals = malloc(vcount * 3 * sizeof(float));
    indices = malloc(icount * 3 * sizeof(unsigned int));
    if (!vertices || !vnormals || !indices) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }
    rewind(fp);
    int vpos = 0;
    int vnpos = 0;
    int ipos = 0;
    while (fgets(line, sizeof(line), fp)) {
        char* token = strtok(line, " ");
        if (!token) continue;
        if (strcmp(token, "v") == 0) {
            char* x = strtok(NULL, " ");
            char* y = strtok(NULL, " ");
            char* z = strtok(NULL, " ");
            if (x && y && z) {
                vertices[vpos * 3] = atof(x);
                vertices[vpos * 3 + 1] = atof(y);
                vertices[vpos * 3 + 2] = atof(z);
                vpos++;
            }
        }
        if (strcmp(token, "vn") == 0) {
            char* nx = strtok(NULL, " ");
            char* ny = strtok(NULL, " ");
            char* nz = strtok(NULL, " ");
            if (nx && ny && nz) {
                vnormals[vnpos * 3] = atof(nx);
                vnormals[vnpos * 3 + 1] = atof(ny);
                vnormals[vnpos * 3 + 2] = atof(nz);
                vnpos++;
            }
        }
        if (strcmp(token, "f") == 0) {
            char* i1 = strtok(NULL, " ");
            char* i2 = strtok(NULL, " ");
            char* i3 = strtok(NULL, " ");
            if (i1 && i2 && i2) {
                indices[ipos * 3] = atoi(i1) - 1;
                indices[ipos * 3 + 1] = atoi(i2) - 1;
                indices[ipos * 3 + 2] = atoi(i3) - 1;
                ipos++;
            }
        }
    }

    fclose(fp);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {0.0f, 0.0f, 0.0f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], upDir[0],
              upDir[1], upDir[2]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, vnormals);
    glDrawElements(GL_TRIANGLES, icount * 3, GL_UNSIGNED_INT, indices);
    glFlush();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.01, 100.0);
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