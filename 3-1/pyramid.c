#include <stdio.h>

int main(void) {
    FILE* fp;
    fp = fopen("pyramid.obj", "w");
    if (!fp) {
        printf("Failed to open file for writing\n");
        return 1;
    }
    {
        /* code */
    }

    float l = 0.8f;
    int n;
    printf("Enter the number of pyramids (n): ");
    scanf("%d", &n);
    for (int i = 0; i < n * 2; i = i + 2) {
        float hl = l * (i + 1);
        fprintf(fp, "v %f %f %f\n", -hl, -l * i, -hl);
        fprintf(fp, "v %f %f %f\n", -hl, -l * i, hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i, -hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i, hl);
        fprintf(fp, "v %f %f %f\n", -hl, -l * i + l * 2, -hl);
        fprintf(fp, "v %f %f %f\n", -hl, -l * i + l * 2, hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i + l * 2, -hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i + l * 2, hl);
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "f %d %d %d\n", 1 + 8 * i, 2 + 8 * i, 3 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 4 + 8 * i, 3 + 8 * i, 2 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 3 + 8 * i, 4 + 8 * i, 7 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 4 + 8 * i, 7 + 8 * i, 8 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 2 + 8 * i, 4 + 8 * i, 6 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 4 + 8 * i, 6 + 8 * i, 8 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 1 + 8 * i, 2 + 8 * i, 5 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 2 + 8 * i, 5 + 8 * i, 6 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 1 + 8 * i, 3 + 8 * i, 5 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 3 + 8 * i, 5 + 8 * i, 7 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 5 + 8 * i, 6 + 8 * i, 7 + 8 * i);
        fprintf(fp, "f %d %d %d\n", 6 + 8 * i, 7 + 8 * i, 8 + 8 * i);
    }
    fclose(fp);
}