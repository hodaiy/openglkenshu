#include <GL/glut.h>
#include <cglm/cglm.h>
#include <string.h>

void loadObj(const char* filename);
void display(void);
void onResize(int width, int height);

float* vertices;
unsigned int* indices;
int icount = 0;

int main(int argc, char* argv[]) {
    loadObj("pyramid.obj");
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

void loadObj(const char* filename) {
    FILE* fp;
    char line[50];

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    int vcount = 0;
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

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, icount * 3, GL_UNSIGNED_INT, indices);
    glFlush();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    gluLookAt(20.0, 3.0, -20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}