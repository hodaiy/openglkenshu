#include <GL/glut.h>
#include <cglm/cglm.h>
#include <math.h>
#include <string.h>

#include "../rot_qua/rot_qua.h"
#include "../rot_qua/vec3.h"

void loadObj(const char* filename);
void calcNormals(void);
void display(void);
void onResize(int width, int height);
void mouse(int button, int state, int x, int y);
void move(int x, int y);

float* vertices;
unsigned int* indices;
float* vnormals;
int vcount = 0, icount = 0;

double eye[3] = {15.0, 15.0, 15.0};
double pov[3] = {0.0, -5.0, 0.0};
double worldUp[3] = {0.0, 1.0, 0.0};
double viewDir[3];
double rightDir[3];
double upDir[3];

int currentBtn = -1, lastX = 0, lastY = 0;

int main(int argc, char* argv[]) {
    sub(pov, eye, viewDir);
    nrm(viewDir, viewDir);
    crs(viewDir, worldUp, rightDir);
    nrm(rightDir, rightDir);
    crs(rightDir, viewDir, upDir);
    nrm(upDir, upDir);
    loadObj("pyramid.obj");
    calcNormals();
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

void loadObj(const char* filename) {
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
    indices = malloc(icount * 3 * sizeof(unsigned int));
    if (!vertices || !indices) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }
    rewind(fp);
    // order is random
    int vpos = 0;
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
void calcNormals(void) {
    double* dvnormals = malloc(vcount * 3 * sizeof(double));
    vnormals = malloc(vcount * 3 * sizeof(float));

    for (int i = 0; i < vcount * 3; i++) {
        dvnormals[i] = 0.0;
    }

    if (!dvnormals || !vnormals) {
        printf("Memory allocation for normals failed\n");
        return;
    }
    double t[3], b[3], n[3];
    // 面ごとにループ
    for (int i = 0; i < icount; i++) {
        double v1[3] = {vertices[indices[i * 3] * 3],
                        vertices[indices[i * 3] * 3 + 1],
                        vertices[indices[i * 3] * 3 + 2]};
        double v2[3] = {vertices[indices[i * 3 + 1] * 3],
                        vertices[indices[i * 3 + 1] * 3 + 1],
                        vertices[indices[i * 3 + 1] * 3 + 2]};
        double v3[3] = {vertices[indices[i * 3 + 2] * 3],
                        vertices[indices[i * 3 + 2] * 3 + 1],
                        vertices[indices[i * 3 + 2] * 3 + 2]};
        sub(v2, v1, t);
        sub(v3, v1, b);
        crs(t, b, n);

        // 面を構成する頂点３つそれぞれの頂点法線に面法線を加算
        // ３をかけるのは、ポインタがベクトルの配列であるため、インデックス＊３つ分、飛ばす必要があるから
        for (int j = 0; j < 3; j++) {
            dvnormals[indices[i * 3] * 3 + j] += n[j];
            dvnormals[indices[i * 3 + 1] * 3 + j] += n[j];
            dvnormals[indices[i * 3 + 2] * 3 + j] += n[j];
        }
    }
    for (int i = 0; i < vcount; i++) {
        nrm(dvnormals + i * 3, dvnormals + i * 3);
        vnormals[i * 3] = (float)dvnormals[i * 3];
        vnormals[i * 3 + 1] = (float)dvnormals[i * 3 + 1];
        vnormals[i * 3 + 2] = (float)dvnormals[i * 3 + 2];
    }
    free(dvnormals);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {0.0f, 0.0f, -1.0f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glDisable(GL_LIGHTING);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0, 0.0, -1.0);
    glEnd();
    glVertex3f(light_position[0], light_position[1], light_position[2]);
    glEnd();
    gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], upDir[0],
              upDir[1], upDir[2]);
    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, vnormals);
    glDrawElements(GL_TRIANGLES, icount * 3, GL_UNSIGNED_INT, indices);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    mul(10.0, rightDir, rightDir);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(rightDir[0], rightDir[1], rightDir[2]);
    glColor3f(0.0f, 1.0f, 0.0f);
    mul(10.0, upDir, upDir);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(upDir[0], upDir[1], upDir[2]);
    glColor3f(0.0f, 0.0f, 1.0f);
    mul(10.0, viewDir, viewDir);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(viewDir[0], viewDir[1], viewDir[2]);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);
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
    double moveVec[3];
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
        mul(-dy / 100.0, viewDir, moveVec);
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