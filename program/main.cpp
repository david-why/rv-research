#include "riscv.h"

#include <Eigen/Dense>

volatile uint32_t *ramreads = (volatile uint32_t *)IO_MEMREADS;

void test_eigen_fixed(void)
{
    printf("------ Testing with Eigen (fixed) ------\n");
    int cycles0 = getcycles();
    int reads0 = *ramreads;
    Eigen::Matrix4i a;
    Eigen::Matrix4i b;
    a << 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16; 
    b << 16, 15, 14, 13,
        12, 11, 10, 9,
        8, 7, 6, 5,
        4, 3, 2, 1;
    int cycles1 = getcycles();
    int reads1 = *ramreads;
    Eigen::Matrix4i result = a * b;
    int cycles2 = getcycles();
    int reads2 = *ramreads;
    printf("Init cycles: %d\n", cycles1 - cycles0);
    printf("Init memreads: %d\n", reads1 - reads0);
    printf("Calculate cycles: %d\n", cycles2 - cycles1);
    printf("Calculate memreads: %d\n", reads2 - reads1);
    printf("Result:\n");
    for (int i = 0; i < result.rows(); ++i)
    {
        for (int j = 0; j < result.cols(); ++j)
        {
            printf("%d ", result(i, j));
        }
        printf("\n");
    }
}

void test_eigen(void)
{
    printf("------ Testing with Eigen (dynamic) ------\n");
    int cycles0 = getcycles();
    int reads0 = *ramreads;
    Eigen::MatrixXi a(4, 4);
    Eigen::MatrixXi b(4, 4);
    a << 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16;
    b << 16, 15, 14, 13,
        12, 11, 10, 9,
        8, 7, 6, 5,
        4, 3, 2, 1;
    int cycles1 = getcycles();
    int reads1 = *ramreads;
    Eigen::MatrixXi result = a * b;
    int cycles2 = getcycles();
    int reads2 = *ramreads;
    printf("Init cycles: %d\n", cycles1 - cycles0);
    printf("Init memreads: %d\n", reads1 - reads0);
    printf("Calculate cycles: %d\n", cycles2 - cycles1);
    printf("Calculate memreads: %d\n", reads2 - reads1);
    printf("Result:\n");
    for (int i = 0; i < result.rows(); ++i)
    {
        for (int j = 0; j < result.cols(); ++j)
        {
            printf("%d ", result(i, j));
        }
        printf("\n");
    }
}

void test_naive(void)
{
    printf("------ Testing naive method ------\n");
    static const int N = 4;
    int cycles0 = getcycles();
    int reads0 = *ramreads;
    int a[N][N] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    int b[N][N] = {
        {16, 15, 14, 13},
        {12, 11, 10, 9},
        {8, 7, 6, 5},
        {4, 3, 2, 1}
    };
    int cycles1 = getcycles();
    int reads1 = *ramreads;
    int result[N][N] = {0};
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            for (int k = 0; k < N; ++k)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    int cycles2 = getcycles();
    int reads2 = *ramreads;
    printf("Init cycles: %d\n", cycles1 - cycles0);
    printf("Init memreads: %d\n", reads1 - reads0);
    printf("Calculate cycles: %d\n", cycles2 - cycles1);
    printf("Calculate memreads: %d\n", reads2 - reads1);
    printf("Result:\n");
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    test_eigen_fixed();
    test_eigen();
    test_naive();
    return 0;
}
