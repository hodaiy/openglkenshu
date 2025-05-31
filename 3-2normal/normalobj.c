#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../rot_qua/vec3.h"

float* vertices;
unsigned int* indices;
float* vnormals;
int vcount = 0, icount = 0;

void loadObj(const char* filename);
void calcNormals(void);
void outputObj(const char* filename);

int main(void) {
    loadObj("../3-2/bun_zipper_res4.obj");
    calcNormals();
    outputObj("../3-2/bun_zipperNormal_res4.obj");
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

void outputObj(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open file for writing: %s\n", filename);
        return;
    }

    for (int i = 0; i < vcount; i++) {
        fprintf(fp, "v %f %f %f\n", vertices[i * 3], vertices[i * 3 + 1],
                vertices[i * 3 + 2]);
        fprintf(fp, "vn %f %f %f\n", vnormals[i * 3], vnormals[i * 3 + 1],
                vnormals[i * 3 + 2]);
    }

    for (int i = 0; i < icount; i++) {
        fprintf(fp, "f %d//%d %d//%d %d//%d\n", indices[i * 3] + 1,
                indices[i * 3] + 1, indices[i * 3 + 1] + 1,
                indices[i * 3 + 1] + 1, indices[i * 3 + 2] + 1,
                indices[i * 3 + 2] + 1);
    }

    fclose(fp);
}