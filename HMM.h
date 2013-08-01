#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

class HMM
{
private:

    double* Pi;
    double** A;
    double** B;

    int N;
    int M;

public:
    HMM() {};
    HMM(int _N, int _M, double* _Pi, double** _A, double** _B);

    double Forward(int obCount, int* ob);
    double Backward(int obCount, int* ob);
    void Viterbi(int obCount, int* ob, int* hs);

    char** hiddenState;
    char** ObservationalState;
};

#endif // HMM_H_INCLUDED
