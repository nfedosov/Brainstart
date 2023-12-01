#ifndef SIGNALPLOTWIN_H
#define SIGNALPLOTWIN_H

#include <QWidget>
#include <QPushButton.h>
#include <QTimer>
#include <QVector>
#include "qcustomplot.h"
#include "datareceiver.h"
#include "savedata.h"
#include "firwin.h"
#include "simplebarfbwin.h"

#include "code_iir/IIR.h"


class SignalPlotWin : public QWidget
{
    Q_OBJECT
public:
    explicit SignalPlotWin(uint Nch, DataReceiver*, QWidget *parent = nullptr);

    QCustomPlot* plot;
    QCustomPlot** plots_processed;
    QCPItemLine *verticalLine;

    DataReceiver* datareceiver;
    SimpleBarFBWin* fbwin;
    SaveData* savedata;

    FirWin* firwin_bp;


    QLineEdit *low_cut_edit;
    QLineEdit *high_cut_edit;


    QCPItemText** chNames;

    QStackedLayout *stackedLayout;


    QWidget *plotLayoutWidget;
    QWidget *plotProcLayoutWidget;


    const uint MAXSECWIN = 50;
    const uint MINSECWIN = 1;
    //uint cumwinsamples;
    uint Nch;
    int srate;
    QTimer timer;
    //uint samplesfromstart = 0;
    //uint samplesinwin;
    int curwinidx = 0;
    int prevbufidx =0; // old position in the buffer where data comes from
    int curbufidx = 0;
    uint defaultwinlen = 15; //sec
    int curlenwin; // the size of window in samples
    double scale;
    double rng;

    QStringList ch_names_string;

    IIR::BiquadsCascade iir_low_bqC;
    IIR::BiquadsCascade iir_high_bqC;
    IIR::BiquadsCascade iir_50_bqC;
    IIR::BiquadsCascade iir_100_bqC;
    IIR::BiquadsCascade iir_150_bqC;
    IIR::BiquadsCascade iir_200_bqC;

    double low_cutoff = 2.0;
    double high_cutoff = 40.0;
    bool to_Notch = true;
    bool to_Low = true;
    bool to_High = true;


    bool is_started = false;

    uint record_pos;

    QVector<double> timedata;
    QVector<QVector<double>> visdata;

    Eigen::MatrixXd data;


    //QVector<QVector<double>> firvisbuf;
    float def_bandpass_lensec = 0.5;
    int bandpass_len;




    bool isRecorded = false;
    bool isShowProcessed = false;

    QVector<double> processeddata;
    QVector<double> envelopedata;
    QVector<double> phasedata;




signals:

private slots:
    void onstartButtonclicked();
    void onrecordButtonclicked();
    void onzoomInButtonclicked();
    void onzoomOutButtonclicked();
    void onshowProcessedRaw();
    void onLowCutEntered();
    void onHighCutEntered();



    void updGraphs();
    void resizeEvent(QResizeEvent *event);

private:

    QPushButton *startButton;

    QPushButton *recordButton;
    //QRadioButton *showProcessed;
    //QRadioButton *showRaw;
    QPushButton *showProcessedRaw;

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QToolButton *zoomLeftButton;
    QToolButton *zoomRightButton;

};

#endif // SIGNALPLOTWIN_H
