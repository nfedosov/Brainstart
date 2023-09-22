#ifndef WHITEKF_H
#define WHITEKF_H

#include "idataprocessor.h"
#include <C:/Users/Fedosov/Documents/projects/Brainstart/Brainstart/eigen-3.4.0/Eigen/Dense>


using namespace Eigen;

class WhiteKF: public IDataProcessor
{
public:
    double A;
    double freq;
    double srate;
    double q;
    double r;

    double default_A = 0.99;
    double default_freq = 10.0;
    double default_srate = 500.0;
    double default_r = 0.1;
    double default_q = 0.00001;


    Matrix2d Psi = MatrixXd(2,2);
    Vector2d x = VectorXd(2);
    Matrix2d P = MatrixXd(2,2);
    Matrix<double, 1,2> H;
    Matrix<double, 1,1> res;
    Matrix<double, 1,1> S;
    Matrix<double, 1,1> R;
    Matrix<double, 2,2> Q;
    Matrix<double, 2,1> K;


    //double **Psi;
    //WhiteKF(double *, double *, double *, int);
    WhiteKF();


    void reset_states();
    void init_params(double freq, double srate, double A, double r, double q);
    //void lfilter(QVector<double> data) override;
    void grid_search();
    Vector2d step(double) override;
};

#endif // WHITEKF_H
