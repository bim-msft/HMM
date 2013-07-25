#ifndef HMM_H_INCLUDED
#define HMM_H_INCLUDED

class HMM
{
private:
    enum X {XLW, AXL, LCS};
    enum O {Laugh, Sad, Cry, Angry};

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
};

#endif // HMM_H_INCLUDED
