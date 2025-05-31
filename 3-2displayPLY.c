#include <GL/glut.h>
#include <cglm/cglm.h>
#include <string.h>

void loadPly(const char* filename);
void display(void);
void onResize(int width, int height);

float* vertices;
unsigned int* indices;
int icount;

int main(int argc, char* argv[]) {
    loadPly("bun_zipper.ply");
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutMainLoop();
    free(vertices);
    free(indices);
    return 0;
}

void loadPly(const char* filename) {
    FILE* fp;
    char line[512];

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    int vcount;
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "element vertex", 14) == 0)
            sscanf(line, "element vertex %d", &vcount);
        if (strncmp(line, "element face", 12) == 0)
            sscanf(line, "element face %d", &icount);
        if (strncmp(line, "end_header", 10) == 0) break;
    }
    printf("vcount: %d, icount: %d\n", vcount, icount);

    vertices = malloc(vcount * 3 * sizeof(float));
    indices = malloc(icount * 3 * sizeof(unsigned int));
    if (!vertices || !indices) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    for (int i = 0; i < vcount; i++) {
        if (!fgets(line, sizeof(line), fp)) {
            printf("Error reading vertex data at line %d\n", i + 1);
            fclose(fp);
            free(vertices);
            free(indices);
            return;
        }
        sscanf(line, "%f %f %f %*f %*f", &vertices[i * 3], &vertices[i * 3 + 1],
               &vertices[i * 3 + 2]);
    }
    for (int i = 0; i < icount; i++) {
        if (!fgets(line, sizeof(line), fp)) {
            break;
            printf("Error reading index data at line %d\n", i + 1);
            fclose(fp);
            free(vertices);
            free(indices);
            return;
        }
        sscanf(line, "%*d %d %d %d", &indices[i * 3], &indices[i * 3 + 1],
               &indices[i * 3 + 2]);
    }
    fclose(fp);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, icount * 3, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
}
void onResize(int width, int height) {
    glViewport(0, 0, width, height);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.01, 100.0);
    gluLookAt(0.3, 0.3, 0.3, 0.0, 0.1, 0.0, 0.0, 1.0, 0.0);
}