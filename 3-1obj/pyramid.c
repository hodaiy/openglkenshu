#include <stdio.h>

int main(void) {
    FILE* fp;
    fp = fopen("../3-1/pyramid.obj", "w");
    if (!fp) {
        printf("Failed to open file for writing\n");
        return 1;
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
        fprintf(fp, "v %f %f %f\n", -hl, -l * i - l * 2, -hl);
        fprintf(fp, "v %f %f %f\n", -hl, -l * i - l * 2, hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i - l * 2, -hl);
        fprintf(fp, "v %f %f %f\n", hl, -l * i - l * 2, hl);
    }

    for (int i = 0; i < n; i++) {
        int v = 8 * i;

        // 上面
        fprintf(fp, "f %d %d %d\n", v + 1, v + 2, v + 3);
        fprintf(fp, "f %d %d %d\n", v + 2, v + 4, v + 3);

        // 下面
        if (i == n - 1) {
            fprintf(fp, "f %d %d %d\n", v + 5, v + 7, v + 6);
            fprintf(fp, "f %d %d %d\n", v + 6, v + 7, v + 8);
        }
        // 前面（
        fprintf(fp, "f %d %d %d\n", v + 2, v + 6, v + 4);
        fprintf(fp, "f %d %d %d\n", v + 4, v + 6, v + 8);

        // 背面
        fprintf(fp, "f %d %d %d\n", v + 1, v + 3, v + 5);
        fprintf(fp, "f %d %d %d\n", v + 3, v + 7, v + 5);

        // 左面
        fprintf(fp, "f %d %d %d\n", v + 1, v + 5, v + 2);
        fprintf(fp, "f %d %d %d\n", v + 2, v + 5, v + 6);

        // 右面
        fprintf(fp, "f %d %d %d\n", v + 3, v + 4, v + 7);
        fprintf(fp, "f %d %d %d\n", v + 4, v + 8, v + 7);
    }

    fclose(fp);
}
