#ifndef DATARECEIVER_H
#define DATARECEIVER_H


#include <QWidget>

#include "qlistwidget.h"
#include "whitekf.h"
#include "cfir.h"
#include "idataprocessor.h"
#include "liblsl-master/include/lsl_cpp.h"

using namespace lsl;


class DataReceiver: public QObject
{
public:
    DataReceiver(IDataProcessor*, QObject *parent = nullptr);

    std::vector<stream_info> results;

    void fakeDataReceive();
    int resolve_and_print_streams(QListWidget* streamList);
    void lslDataReceive();
    void memStreamInfo();


    int stream_idx = 0;
    //stream_info* info;
    stream_inlet* inlet;

    unsigned long long totalsamplesreceived;
    int curposidx;

    unsigned int maxbufsec = 5;
    unsigned int maxbufsamples;

    double srate = 500.0;
    unsigned int Nch;


    QVector<QVector<double>> databuffer;
    VectorXd envelopebuffer;
    VectorXd phasebuffer;
    VectorXd processedbuffer;


    Vector2d x;




    // ПОКА БЕЗ ВИРТУАЛЬНОГО КЛАССА
    IDataProcessor *dataprocessor;
    //WhiteKF *dataprocessor;
    //CFIR *dataprocessor;

    VectorXd spat_filter;



};

#endif // DATARECEIVER_H
