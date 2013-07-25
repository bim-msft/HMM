#include "HMM.h"
#include <stdlib.h>
#include <stdio.h>

HMM::HMM(int _N, int _M, double* _Pi, double** _A, double** _B)
{
    N = _N;
    M = _M;

    Pi = (double*)malloc(sizeof(double) * N);
    for (int i = 0; i < N; i++)
        *(Pi + i) = *(_Pi + i);

    A = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
    {
        *(A + i) = (double*)malloc(sizeof(double) * N);
        for (int j = 0; j < N; j++)
            *(*(A + i) + j) = *(*(_A + i) + j);
    }

    B = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
    {
        *(B + i) = (double*)malloc(sizeof(double) * M);
        for (int j = 0; j < M; j++)
            *(*(B + i) + j) = *(*(_B + i) + j);
    }
    printf("N = %d M = %d\n\n", N, M);

    printf("Pi:\n");
    for (int i = 0; i < N; i++)
        printf("%.4lf\n", Pi[i]);
    printf("\n");

    printf("A:\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%.4lf ", A[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("B:\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%.4lf ", B[i][j]);
        printf("\n");
    }
    printf("\n");
}

double HMM::Forward(int obCount, int* ob)
{
    double ans = 0.0;
    double** Alpha = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
        *(Alpha + i) = (double*)malloc(sizeof(double) * N);

    for (int j = 0; j < N; j++)
    {
        Alpha[0][j] = Pi[j] * B[j][ob[0]];
        printf("Alpha[%d][%d] = %.8lf\n", 0, j, Alpha[0][j]);
    }

    for (int i = 1; i < obCount; i++)
    {
        for (int j = 0; j < N; j++)
        {
            Alpha[i][j] = 0.0;
            for (int k = 0; k < N; k++)
                Alpha[i][j] += Alpha[i - 1][k] * A[k][j];
            Alpha[i][j] *= B[j][ob[i]];

            if (i == obCount - 1)
                ans += Alpha[i][j];
        }
    }
    return ans;
}

double HMM::Backward(int obCount, int* ob)
{
    double ans = 0.0;
    double** Beta = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
        *(Beta + i) = (double*)malloc(sizeof(double) * N);

    for (int j = 0; j < N; j++)
    {
        Beta[obCount - 1][j] = 1;
        printf("Beta[%d][%d] = %.8lf\n", obCount - 1, j, Beta[obCount - 1][j]);
    }

    for (int i = obCount - 2; i >= 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            Beta[i][j] = 0.0;
            for (int k = 0; k < N; k++)
                Beta[i][j] += Beta[i + 1][k] * A[j][k] * B[k][ob[i + 1]];

            printf("Beta[%d][%d] = %.8lf\n", i, j, Beta[i][j]);

            if (i == 0)
                ans += Beta[i][j];
        }
    }
    return ans;
}
