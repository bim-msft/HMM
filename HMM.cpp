#include "HMM.h"
#include <string.h>
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

    hiddenState = (char**)malloc(sizeof(char*) * N);
    for (int i = 0; i < N; i++)
        hiddenState[i] = (char*)malloc(sizeof(char) * 100);
    strcpy(hiddenState[0], "AsuraKing");
    strcpy(hiddenState[1], "Asura");
    strcpy(hiddenState[2], "Rasetsu");

    ObservationalState = (char**)malloc(sizeof(char*) * M);
    for (int i = 0; i < M; i++)
        ObservationalState[i] = (char*)malloc(sizeof(char) * 100);
    strcpy(ObservationalState[0], "Laugh");
    strcpy(ObservationalState[1], "Sad");
    strcpy(ObservationalState[2], "Cry");
    strcpy(ObservationalState[3], "Angry");
}

double HMM::Forward(int obCount, int* ob)
{
    double ans = 0.0;
    double** Alpha = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
        *(Alpha + i) = (double*)malloc(sizeof(double) * N);

    for (int j = 0; j < N; j++)
        Alpha[0][j] = Pi[j] * B[j][ob[0]];

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
        Beta[obCount - 1][j] = 1;

    for (int i = obCount - 2; i >= 0; i--)
    {
        for (int j = 0; j < N; j++)
        {
            Beta[i][j] = 0.0;
            for (int k = 0; k < N; k++)
                Beta[i][j] += Beta[i + 1][k] * A[j][k] * B[k][ob[i + 1]];

            if (i == 0)
                ans += Beta[i][j];
        }
    }
    return ans;
}

void HMM::Viterbi(int obCount, int* ob, int* hs)
{
    double **x = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; i++)
        *(x + i) = (double*)malloc(sizeof(double) * N);

    double _max = 0.0;
    int _hsIndex = 0;
    for (int j = 0; j < N; j++)
    {
        x[0][j] = Pi[j] * B[j][ob[0]];
        if (_max < x[0][j])
        {
            _max = x[0][j];
            _hsIndex = j;
        }
    }
    hs[0] = _hsIndex;

    for (int i = 1; i < obCount; i++)
    {
        _max = 0.0;
        _hsIndex = 0;
        for (int j = 0; j < N; j++)
        {
            double _maxPrevious = 0.0;
            for (int k = 0; k < N; k++)
            {
                double tmp = x[i - 1][k] * A[k][j];
                _maxPrevious = _maxPrevious > tmp ? _maxPrevious : tmp;
            }
            x[i][j] = _maxPrevious * B[j][ob[i]];

            if (_max < x[i][j])
            {
                _max = x[i][j];
                _hsIndex = j;
            }
        }
        hs[i] = _hsIndex;
    }
}
