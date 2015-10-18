#include <stdio.h>

int test_matrix[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

/* test_matrix_cw90 == test_matrix_ccw270 */
int test_matrix_cw90[4][4] = {
    13, 9, 5, 1,
    14, 10, 6, 2,
    15, 11, 7, 3,
    16, 12, 8, 4
};

/* test_matrix_cw180 == test_matrix_ccw180 */
int test_matrix_cw180[4][4] = {
    16, 15, 14, 13,
    12, 11, 10, 9,
    8, 7, 6, 5,
    4, 3, 2, 1
};

/* test_matrix_cw270 == test_matrix_ccw90 */
int test_matrix_cw270[4][4] = {
    4, 8, 12, 16,
    3, 7, 11, 15,
    2, 6, 10, 14,
    1, 5, 9, 13
};
int test_matrix_flipH[4][4] = {
    4, 3, 2, 1,
    8, 7, 6, 5,
    12, 11, 10, 9,
    16, 15, 14, 13
};

int test_matrix_flipV[4][4] = {
    13, 14, 15, 16,
    9, 10, 11, 12,
    5, 6, 7, 8,
    1, 2, 3, 4
};

int test_matrix_transMain[4][4] = {
    1, 5, 9, 13,
    2, 6, 10, 14,
    3, 7, 11, 15,
    4, 8, 12, 16
};

int test_matrix_transSide[4][4] = {
    16, 12, 8, 4,
    15, 11, 7, 3,
    14, 10, 6, 2,
    13, 9, 5, 1
};

int equals(int m1[4][4], int m2[4][4]) {
    int i, j, n = 4;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (m1[i][j] != m2[i][j])
                return 0;
    return 1;
}

void printMatrix(int mat[4][4]) {
    int i, j;
    int n = 4;
    printf("{ ");
    for (i = 0; i < n; i++) {
        printf("{");
        for (j = 0; j < n; j++) {
            printf("%5i, ", mat[i][j]);
        }
        printf("}");
        if ( i < n - 1)
            printf("\n  ");
    }
    printf(" };\n");
}

int main() {
    return 0;
}
