#include "mainwindow.h"

#include "./ui_mainwindow.h"
#include<QDebug>
#include<QtGlobal>
#include "kalmanparamchoice.h"

#include "signalplotwin.h"
#include "whitekf.h"
#include "cfir.h"
#include "datareceiver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);


    QVBoxLayout *vLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *hLayout_1 = new QHBoxLayout();
    QHBoxLayout *hLayout_2 = new QHBoxLayout();

    QVBoxLayout *vLayKF_CFIR = new QVBoxLayout();
    QVBoxLayout *vLayENV_PHASE = new QVBoxLayout();



    streamListWidget = new QListWidget(this);
    //streamListWidget->resize(300,200);





    demoButton = new QPushButton("Start Demo", this);
    setKalmanManual = new QPushButton("Set Kalman Params", this);
    findStreams = new QPushButton("find streams",this);

    envelopeFBButton = new QRadioButton("Envelope\nexperiment", this);
    phaseFBButton = new QRadioButton("Phase\nexperiment", this);


    useKFButton = new QRadioButton("Use\nKalman Filter", this);
    useCFIRButton = new QRadioButton("Use\nCFIR", this);


    buttonGroupEP = new QButtonGroup();
    buttonGroupEP->addButton(envelopeFBButton);
    buttonGroupEP->addButton(phaseFBButton);


    // Set default checked button
    envelopeFBButton->setChecked(true);


    buttonGroupKC = new QButtonGroup();
    buttonGroupKC->addButton(useKFButton, 0);
    buttonGroupKC->addButton(useCFIRButton, 1);

    // Set default checked button
    useKFButton->setChecked(true);

    // Connect signals and slots for button group
    //connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleButtonGroupClick(int)));



    hLayout_1->addWidget(demoButton);
    hLayout_1->addWidget(setKalmanManual);
    hLayout_1->addWidget(findStreams);
    hLayout_1->addWidget(streamListWidget);

    hLayout_1->setStretch(0,1);
    hLayout_1->setStretch(1,1);
    hLayout_1->setStretch(2,1);
    hLayout_1->setStretch(3,1);


    vLayENV_PHASE->addWidget(envelopeFBButton);
    vLayENV_PHASE->addWidget(phaseFBButton);

    vLayKF_CFIR->addWidget(useKFButton);
    vLayKF_CFIR->addWidget(useCFIRButton);



    hLayout_2->addLayout(vLayKF_CFIR);
    hLayout_2->addLayout(vLayENV_PHASE);

    vLayout->addLayout(hLayout_1);
    QWidget *emptyWidget = new QWidget();


    vLayout->addWidget(emptyWidget);
    vLayout->addLayout(hLayout_2);

    vLayout->setStretch(0,1);
    vLayout->setStretch(1,2);
    vLayout->setStretch(2,1);



    centralWidget->setLayout(vLayout);


    // default processor
    dataproc = new WhiteKF();//new CFIR();//new CFIR(100, 8.0, 12.0, 500.0,"hamming");//new WhiteKF();
//new WhiteKF();
    datareceiver = new DataReceiver(dataproc);


    //setKalmanManual->move(200, 100);

    connect(findStreams, &QPushButton::clicked, this, &MainWindow::onfindStreamsClicked);
    connect(demoButton, &QPushButton::clicked, this, &MainWindow::ondemoButtonclicked);
    connect(setKalmanManual, &QPushButton::clicked, this, &MainWindow::onsetKalmanButtonclicked);
    connect(streamListWidget, &QListWidget::itemClicked, this, &MainWindow::handleStreamSelected);

    //connect(useCFIRButton, &QRadioButton::clicked, this, &MainWindow::onusecfirButtonClicked);


    //connect(buttonGroupKC, &QButtonGroup::buttonToggled,
    //    this, &MainWindow::handleButtonGroupKCClick);
    //connect(buttonGroupKC, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::handleButtonGroupKCClick);
    connect(buttonGroupKC, &QButtonGroup::idClicked, this, &MainWindow::handleButtonGroupKCClick);

}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ondemoButtonclicked()
{


    signalplotwin = new SignalPlotWin(datareceiver->Nch,500, datareceiver);
    signalplotwin->show();


    //feedbackwin = new FeedBackWin();
    //feedbackwin->show();


    qInfo() << "C++ Style Info Message";
    qInfo( "C Style Info Message" );




    demoButton->setText("clicked!");


}
void MainWindow::onfindStreamsClicked()
{
    datareceiver->resolve_and_print_streams(streamListWidget);
}


void MainWindow::handleStreamSelected()
{
    datareceiver->stream_idx = streamListWidget->currentRow();
    datareceiver->Nch =datareceiver->results[datareceiver->stream_idx].channel_count();
    datareceiver->spat_filter.resize(datareceiver->Nch);
    datareceiver->databuffer.resize(datareceiver->Nch);
    for (int i = 0; i < datareceiver->Nch; i++) {
        datareceiver->spat_filter[i] = 0.5; // CHANGE!!

        datareceiver->databuffer[i].resize(datareceiver->maxbufsamples);
        datareceiver->databuffer[i].fill(0.0);
    }

}



void MainWindow::onsetKalmanButtonclicked()
{

    WhiteKF *kf = new WhiteKF();
    kalmanparamchoice = new KalmanParamChoice(kf);


    //kalmanparamchoice->setModal(true);


    kalmanparamchoice->show();

}


void MainWindow::handleButtonGroupKCClick(int id)
{
    qInfo( "I'm here" );
    delete dataproc;
    //delete datareceiver;
    if (id == 0)//...?
    {
        dataproc = new WhiteKF();
    }
    if (id == 1)
    {
        setKalmanManual->setText("set CFIR params");
        dataproc = new CFIR();//new CFIR(100, 8.0, 12.0, 500.0,"hamming");//new WhiteKF();

    }
    datareceiver->dataprocessor = dataproc;
    //datareceiver = new DataReceiver(dataproc);

//

}


