#include "HMM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int initHMM(char *fileName, HMM** hmm)
{
    int N = 0, M = 0;
    char label[5];
    double **_A, **_B, *_Pi;
    FILE* fp = fopen(fileName, "r");

    fscanf(fp, "M=%d N=%d", &M, &N);

    for (int i = 0; i < 3; i++)
    {
        fscanf(fp, "%s", label);

        if (!strcmp(label, "A:") || !strcmp(label, "a:"))
        {
            _A = (double**)malloc(sizeof(double*) * N);
            for (int i = 0; i < N; i++)
            {
                *(_A + i) = (double*)malloc(sizeof(double) * N);
                for (int j = 0; j < N; j++)
                    fscanf(fp, "%lf", *(_A + i) + j);
            }
        }
        else if (!strcmp(label, "B:") || !strcmp(label, "b:"))
        {
            _B = (double**)malloc(sizeof(double*) * N);
            for (int i = 0; i < N; i++)
            {
                *(_B + i) = (double*)malloc(sizeof(double) * M);
                for (int j = 0; j < M; j++)
                    fscanf(fp, "%lf", *(_B + i) + j);
            }
        }
        else if (!strcmp(label, "pi:") || !strcmp(label, "PI:") || !strcmp(label, "Pi:"))
        {
            _Pi = (double*)malloc(sizeof(double) * N);
            for (int i = 0; i < N; i++)
                fscanf(fp, "%lf", _Pi + i);
        }
        else
            return -1;
    }

    *hmm = new HMM(N, M, _Pi, _A, _B);

    fclose(fp);
    return 0;
}

int readObSeq(char* fileName, int* seqLen, int* ob)
{
    char firstLine[35];
    FILE* fp = fopen(fileName, "r");

    fscanf(fp, "%s", firstLine);
    if (firstLine[0] == 'T')
        sscanf(firstLine, "T=%d", seqLen);
    else if (firstLine[0] == 't')
        sscanf(firstLine, "t=%d", seqLen);
    else
        return -1;

    for (int i = 0; i < *seqLen; i++)
    {
        fscanf(fp, "%d", &ob[i]);
        ob[i]--;
    }

    fclose(fp);
    return 0;
}

int main(int argc, char** argv)
{
    HMM* hmm;
    int retVal = initHMM("model.hmm", &hmm);

    if (retVal == -1)
    {
        printf("Invalid HMM file format!\n");
        return -1;
    }

    int ob[100];
    int obCount = 0;
    retVal = readObSeq("obs1.seq", &obCount, ob);

    if (retVal == -1)
    {
        printf("Invalid SEQ file format!\n");
        return -1;
    }
    int* hs = (int*)malloc(sizeof(int) * obCount);
    printf("SEQ: ");
    for (int i = 0; i < obCount; i++)
        printf("%d(%s) ", ob[i], hmm->ObservationalState[ob[i]]);
    printf("\n");

    printf("Forward: %.8lf\n", hmm->Forward(obCount, ob));
    printf("Backward: %.8lf\n", hmm->Backward(obCount, ob));
    hmm->Viterbi(obCount, ob, hs);
    printf("Viterbi: ");
    for (int i = 0; i < obCount; i++)
        printf("%d(%s) ", hs[i], hmm->hiddenState[hs[i]]);
    printf("\n");

    return 0;
}
