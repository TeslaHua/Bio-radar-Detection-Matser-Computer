#include "mainwidget.h"

#include <QApplication>

using namespace std;

string MainWidget::getDate()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d",localtime(&timep) );
    return tmp;
}

string MainWidget::getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%H:%M:%S",localtime(&timep) );
    return tmp;
}


int xinlv1=0,huxi1=0,xinlv2=0,huxi2=0,xinlv3=0,huxi3=0,xinlv4=0,huxi4=0;


bool MainWidget::OpenDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");   //数据库驱动类型为SQL Server
    qDebug()<<"ODBC driver?"<<db.isValid();
    QString dsn = QString::fromLocal8Bit("kqq");      //数据源名称
    db.setHostName("10.1.149.15");                      //选择本地主机，127.0.1.1
    db.setDatabaseName(dsn);                            //设置数据源名称
    db.setUserName("sa");                               //登录用户
    db.setPassword("123456");                           //密码

    if(!db.open())                                      //打开数据库
    {
        qDebug()<<db.lastError().text();
        QMessageBox::critical(0, QObject::tr("Database error"), db.lastError().text());

        return false;                                   //打开失败
    }
    else
    {
        qDebug()<<"database open success!";
        QSqlQuery query(db); //查询表并输出，测试能否正常操作数据库
        query.exec("select * from 雷达阈值信息_info where 床位ID=1 or 床位ID=2 or 床位ID=3 or 床位ID=4");
        if (query.next())
        {
            xinlv1=query.value(1).toInt();
            huxi1=query.value(2).toInt();

        }
        if (query.next())
        {
            xinlv2=query.value(1).toInt();
            huxi2=query.value(2).toInt();
        }
        if (query.next())
        {
            xinlv3=query.value(1).toInt();
            huxi3=query.value(2).toInt();
        }
        if (query.next())
        {
            xinlv4=query.value(1).toInt();
            huxi4=query.value(2).toInt();
        }
        return true;                 //打开成功
    }
}



MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), settings("TurnMeOn", "SerialAsst")
{
    //初始化各参数列表
    isOpenDataBase =  OpenDatabase();   //首先判断数据库是否打开
    if(isOpenDataBase){
        QMessageBox::information(this, QString::fromUtf8("提示"),QString::fromUtf8("数据库打开成功"));
    }
    else {
        QMessageBox::information(this, QString::fromUtf8("提示"),QString::fromUtf8("数据库打开失败"));
    }
    BaudrateList << "256000"
                 << "230400"
                 << "128000"
                 << "115200"
                 << "76800"
                 << "57600"
                 << "43000"
                 << "38400"
                 << "19200"
                 << "14400"
                 << "9600"
                 << "4800"
                 << "2400"
                 << "1200";
    StopbitsList << "1"
                 << "1.5"
                 << "2";
    DatabitsList << "8"
                 << "7"
                 << "6"
                 << "5";
    ParityList << tr("无") << tr("奇校验") << tr("偶校验");

    //初始化ComboBox以及波特率信息的复选框
    COMBox1 = new QComboBox();
    COMBox2 = new QComboBox();
    COMBox3 = new QComboBox();
    COMBox4 = new QComboBox();
    BaudrateBox = new QComboBox();
    BaudrateBox->addItems(BaudrateList);
    BaudrateBox->setCurrentIndex(settings.value("Baud rate", 0).toInt());
    StopbitsBox = new QComboBox();
    StopbitsBox->addItems(StopbitsList);
    StopbitsBox->setCurrentIndex(settings.value("Stop bits", 0).toInt());
    DatabitsBox = new QComboBox();
    DatabitsBox->addItems(DatabitsList);
    DatabitsBox->setCurrentIndex(settings.value("Data bits", 0).toInt());
    ParityBox = new QComboBox();
    ParityBox->addItems(ParityList);
    ParityBox->setCurrentIndex(settings.value("Parity", 0).toInt());

    //初始化label
    QPalette palLabel;
    palLabel.setColor(QPalette::WindowText,Qt::yellow);

    //初始化字体
    QFont ft;
    ft.setFamily("宋体");
    //设置文字大小为50像素
    ft.setPixelSize(16);
    //设置文字为粗体
    ft.setBold(true);             //封装的setWeight函数

    //第1个串口号的所选Com口号显示
    COMLabel1 = new QLabel(tr("串口号 : 无   "));
    COMLabel1->setMinimumWidth(130);  //设置最小宽度
    COMLabel1->setPalette(palLabel);   //字体颜色
    COMLabel1->setFont(ft);               //设置字体
    COMLabel1->setStyleSheet("background-color:blue"); //底色

    //第2个串口号的所选Com口号显示
    COMLabel2 = new QLabel(tr("串口号 : 无   "));
    COMLabel2->setMinimumWidth(130);
    COMLabel2->setPalette(palLabel);   //字体颜色
    COMLabel2->setFont(ft);               //设置字体
    COMLabel2->setStyleSheet("background-color:blue"); //底色

    //第3个串口号的所选Com口号显示
    COMLabel3 = new QLabel(tr("串口号 : 无   "));
    COMLabel3->setMinimumWidth(130);
    COMLabel3->setPalette(palLabel);   //字体颜色
    COMLabel3->setFont(ft);               //设置字体
    COMLabel3->setStyleSheet("background-color:blue"); //底色

    //第4个串口号的所选Com口号显示
    COMLabel4 = new QLabel(tr("串口号 : 无   "));
    COMLabel4->setMinimumWidth(130);
    COMLabel4->setPalette(palLabel);   //字体颜色
    COMLabel4->setFont(ft);               //设置字体
    COMLabel4->setStyleSheet("background-color:blue"); //底色

    BaudrateLabel = new QLabel(tr("波特率"));
    StopbitsLabel = new QLabel(tr("停止位"));
    DatabitsLabel = new QLabel(tr("数据位"));
    ParityLabel = new QLabel(tr("校验位"));


    /*******************下面的信息每一个床位的相关信息标签**************************/

    BedLabel1 = new QLabel(tr("   1号床位\n\n   : 离床"));
    BedLabel1->setPalette(palLabel);   //字体颜色
    BedLabel1->setFont(ft);               //设置字体
    BedLabel1->setStyleSheet("background-color:blue"); //底色

    LightLabel1 = new QLabel(tr("   "));
    LightLabel1->setPalette(palLabel);   //字体颜色
    LightLabel1->setFont(ft);               //设置字体
    LightLabel1->setStyleSheet("background-color:red"); //底色

    HeartRateLabel1 = new QLabel(tr("    心率\n\n     无"));
    HeartRateLabel1->setPalette(palLabel);   //字体颜色
    HeartRateLabel1->setFont(ft);               //设置字体
    HeartRateLabel1->setStyleSheet("background-color:blue"); //底色

    BreathingLabel1 = new QLabel(tr("    呼吸\n\n     无"));
    BreathingLabel1->setPalette(palLabel);   //字体颜色
    BreathingLabel1->setFont(ft);               //设置字体
    BreathingLabel1->setStyleSheet("background-color:blue"); //底色

    BedLabel2 = new QLabel(tr("   2号床位\n\n   : 离床"));
    BedLabel2->setPalette(palLabel);   //字体颜色
    BedLabel2->setFont(ft);               //设置字体
    BedLabel2->setStyleSheet("background-color:blue"); //底色

    LightLabel2 = new QLabel(tr("   "));
    LightLabel2->setPalette(palLabel);   //字体颜色
    LightLabel2->setFont(ft);               //设置字体
    LightLabel2->setStyleSheet("background-color:red"); //底色

    HeartRateLabel2 = new QLabel(tr("    心率\n\n     无"));
    HeartRateLabel2->setPalette(palLabel);   //字体颜色
    HeartRateLabel2->setFont(ft);               //设置字体
    HeartRateLabel2->setStyleSheet("background-color:blue"); //底色

    BreathingLabel2 = new QLabel(tr("    呼吸\n\n     无"));
    BreathingLabel2->setPalette(palLabel);   //字体颜色
    BreathingLabel2->setFont(ft);               //设置字体
    BreathingLabel2->setStyleSheet("background-color:blue"); //底色

    BedLabel3 = new QLabel(tr("   3号床位\n\n   : 离床"));
    BedLabel3->setPalette(palLabel);   //字体颜色
    BedLabel3->setFont(ft);               //设置字体
    BedLabel3->setStyleSheet("background-color:blue"); //底色

    LightLabel3 = new QLabel(tr("   "));
    LightLabel3->setPalette(palLabel);   //字体颜色
    LightLabel3->setFont(ft);               //设置字体
    LightLabel3->setStyleSheet("background-color:red"); //底色

    HeartRateLabel3 = new QLabel(tr("    心率\n\n     无"));
    HeartRateLabel3->setPalette(palLabel);   //字体颜色
    HeartRateLabel3->setFont(ft);               //设置字体
    HeartRateLabel3->setStyleSheet("background-color:blue"); //底色

    BreathingLabel3 = new QLabel(tr("    呼吸\n\n     无"));
    BreathingLabel3->setPalette(palLabel);   //字体颜色
    BreathingLabel3->setFont(ft);               //设置字体
    BreathingLabel3->setStyleSheet("background-color:blue"); //底色

    BedLabel4 = new QLabel(tr("   4号床位\n\n   : 离床"));
    BedLabel4->setPalette(palLabel);   //字体颜色
    BedLabel4->setFont(ft);               //设置字体
    BedLabel4->setStyleSheet("background-color:blue"); //底色

    LightLabel4 = new QLabel(tr("   "));
    LightLabel4->setPalette(palLabel);   //字体颜色
    LightLabel4->setFont(ft);               //设置字体
    LightLabel4->setStyleSheet("background-color:red"); //底色

    HeartRateLabel4 = new QLabel(tr("    心率\n\n     无"));
    HeartRateLabel4->setPalette(palLabel);   //字体颜色
    HeartRateLabel4->setFont(ft);               //设置字体
    HeartRateLabel4->setStyleSheet("background-color:blue"); //底色

    BreathingLabel4 = new QLabel(tr("    呼吸\n\n     无"));
    BreathingLabel4->setPalette(palLabel);   //字体颜色
    BreathingLabel4->setFont(ft);               //设置字体
    BreathingLabel4->setStyleSheet("background-color:blue"); //底色

    /************************上面的信息每一个床位的相关信息标签*********************************/

    /*******************下面的信息是定义每一个床位的绘制心率和呼吸的容器**************************/

    //1号床位的心率数据波形
    plotHeartRate1 = new QCustomPlot();
    plotHeartRate1->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotHeartRate1->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotHeartRate1->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotHeartRate1->xAxis->setDateTimeFormat("mm:ss");   //设置横坐标的显示时间的格式
    plotHeartRate1->xAxis->setAutoTickStep(false);       //禁止横坐标自动设置刻度间距
    plotHeartRate1->xAxis->setTickStep(6);               //设置横坐标的刻度间距

    plotHeartRate1->yAxis->setRange(0,250);       //设置纵坐标的显示范围
    plotHeartRate1->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotHeartRate1->yAxis->setTickStep(50);       //设置刻度间距
    // 设置轴的标题
    plotHeartRate1->xAxis->setLabel("时间");
    plotHeartRate1->yAxis->setLabel("心率");

    //1号床位的呼吸数据波形
    plotBreathing1 = new QCustomPlot();
    plotBreathing1->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotBreathing1->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotBreathing1->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotBreathing1->xAxis->setDateTimeFormat("mm:ss");
    plotBreathing1->xAxis->setAutoTickStep(false);
    plotBreathing1->xAxis->setTickStep(4);

    plotBreathing1->yAxis->setRange(0,250);
    plotBreathing1->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotBreathing1->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotBreathing1->xAxis->setLabel("时间");
    plotBreathing1->yAxis->setLabel("呼吸");

    //2号床位的心率数据波形
    plotHeartRate2 = new QCustomPlot();
    plotHeartRate2->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotHeartRate2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotHeartRate2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotHeartRate2->xAxis->setDateTimeFormat("mm:ss");
    plotHeartRate2->xAxis->setAutoTickStep(false);
    plotHeartRate2->xAxis->setTickStep(6);

    plotHeartRate2->yAxis->setRange(0,250);
    plotHeartRate2->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotHeartRate2->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotHeartRate2->xAxis->setLabel("时间");
    plotHeartRate2->yAxis->setLabel("心率");

    //2号床位的呼吸数据波形
    plotBreathing2 = new QCustomPlot();
    plotBreathing2->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotBreathing2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotBreathing2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotBreathing2->xAxis->setDateTimeFormat("mm:ss");
    plotBreathing2->xAxis->setAutoTickStep(false);
    plotBreathing2->xAxis->setTickStep(4);

    plotBreathing2->yAxis->setRange(0,250);
    plotBreathing2->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotBreathing2->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotBreathing2->xAxis->setLabel("时间");
    plotBreathing2->yAxis->setLabel("呼吸");

    //3号床位的心率数据波形
    plotHeartRate3 = new QCustomPlot();
    plotHeartRate3->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotHeartRate3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotHeartRate3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotHeartRate3->xAxis->setDateTimeFormat("mm:ss");
    plotHeartRate3->xAxis->setAutoTickStep(false);
    plotHeartRate3->xAxis->setTickStep(6);

    plotHeartRate3->yAxis->setRange(0,250);
    plotHeartRate3->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotHeartRate3->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotHeartRate3->xAxis->setLabel("时间");
    plotHeartRate3->yAxis->setLabel("心率");

    //3号床位的呼吸数据波形
    plotBreathing3 = new QCustomPlot();
    plotBreathing3->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotBreathing3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotBreathing3->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotBreathing3->xAxis->setDateTimeFormat("mm:ss");
    plotBreathing3->xAxis->setAutoTickStep(false);
    plotBreathing3->xAxis->setTickStep(4);

    plotBreathing3->yAxis->setRange(0,250);
    plotBreathing3->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotBreathing3->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotBreathing3->xAxis->setLabel("时间");
    plotBreathing3->yAxis->setLabel("呼吸");

    //4号床位的心率数据波形
    plotHeartRate4 = new QCustomPlot();
    plotHeartRate4->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotHeartRate4->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotHeartRate4->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotHeartRate4->xAxis->setDateTimeFormat("mm:ss");
    plotHeartRate4->xAxis->setAutoTickStep(false);
    plotHeartRate4->xAxis->setTickStep(6);

    plotHeartRate4->yAxis->setRange(0,250);
    plotHeartRate4->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotHeartRate4->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotHeartRate4->xAxis->setLabel("时间");
    plotHeartRate4->yAxis->setLabel("心率");

    //4号床位的呼吸数据波形
    plotBreathing4 = new QCustomPlot();
    plotBreathing4->addGraph(); // 添加一个图表
    // 设置关键点的显示效果
    plotBreathing4->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QPen(QColor(41,138,220), 2), QBrush(QColor(5,189,251)), 10));
    // 设置x轴显示时间
    plotBreathing4->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plotBreathing4->xAxis->setDateTimeFormat("mm:ss");
    plotBreathing4->xAxis->setAutoTickStep(false);
    plotBreathing4->xAxis->setTickStep(4);

    plotBreathing4->yAxis->setRange(0,250);
    plotBreathing4->yAxis->setAutoTickStep(false);//设置是否自动分配刻度间距
    plotBreathing4->yAxis->setTickStep(50);   //设置刻度间距
    // 设置轴的标题
    plotBreathing4->xAxis->setLabel("时间");
    plotBreathing4->yAxis->setLabel("呼吸");

    /*******************上面的信息是定义每一个床位的绘制心率和呼吸的容器**************************/

    //图表画图计时器初始化
    QTimer dataTimer1;
    connect(&dataTimer1, SIGNAL(timeout()), this, SLOT(realtimeDataSlotCom1(double,double)));
    dataTimer1.start(0); // Interval 0 means to refresh as fast as possible


    QTimer dataTimer2;
    connect(&dataTimer2, SIGNAL(timeout()), this, SLOT(realtimeDataSlotCom2(double,double)));
    dataTimer2.start(0); // Interval 0 means to refresh as fast as possible


    QTimer dataTimer3;
    connect(&dataTimer3, SIGNAL(timeout()), this, SLOT(realtimeDataSlotCom3(double,double)));
    dataTimer3.start(0); // Interval 0 means to refresh as fast as possible


    QTimer dataTimer4;
    connect(&dataTimer4, SIGNAL(timeout()), this, SLOT(realtimeDataSlotCom4(double,double)));
    dataTimer4.start(0); // Interval 0 means to refresh as fast as possible

    //每个串口的打开按钮
    OpenButton1 = new QPushButton(tr("打开串口"));
    OpenButton2 = new QPushButton(tr("打开串口"));
    OpenButton3 = new QPushButton(tr("打开串口"));
    OpenButton4 = new QPushButton(tr("打开串口"));

    //每个串口数据的清除按钮
    ClearRecvButton1 = new QPushButton(tr("清除接收"));
    ClearRecvButton2 = new QPushButton(tr("清除接收"));
    ClearRecvButton3 = new QPushButton(tr("清除接收"));
    ClearRecvButton4 = new QPushButton(tr("清除接收"));

    //设置“打开串口”按钮出发信号发生时的接收器以及接收响应函数
    connect(OpenButton1, QPushButton::clicked, this, OpenSerialCom1);
    connect(OpenButton2, QPushButton::clicked, this, OpenSerialCom2);
    connect(OpenButton3, QPushButton::clicked, this, OpenSerialCom3);
    connect(OpenButton4, QPushButton::clicked, this, OpenSerialCom4);

    //设置“清除数据”按钮出发信号发生时的接收器以及接收响应函数
    connect(ClearRecvButton1, QPushButton::clicked, this, ClearRecvCom1);   //清除显示区数据
    connect(ClearRecvButton2, QPushButton::clicked, this, ClearRecvCom2);   //清除显示区数据
    connect(ClearRecvButton3, QPushButton::clicked, this, ClearRecvCom3);   //清除显示区数据
    connect(ClearRecvButton4, QPushButton::clicked, this, ClearRecvCom4);   //清除显示区数据


    //复选框
    RTSBox = new QCheckBox(tr("RTS"));
    DTRBox = new QCheckBox(tr("DTR"));

    //绑定复选框信号
    connect(RTSBox, QCheckBox::stateChanged, this, RTSControl);
    connect(DTRBox, QCheckBox::stateChanged, this, DTRControl);

    //初始化左侧容器的布局
    leftLlayout = new QFormLayout();
    leftLlayout->addRow(BaudrateLabel, BaudrateBox);
    leftLlayout->addRow(StopbitsLabel, StopbitsBox);
    leftLlayout->addRow(DatabitsLabel, DatabitsBox);
    leftLlayout->addRow(ParityLabel, ParityBox);
    leftLlayout->addRow(RTSBox,DTRBox);
    leftLlayout->setMargin(10);             //控件与窗体的左右边距
    leftLlayout->setHorizontalSpacing(10);  //控件与控件之间的左右边距
    leftLlayout->setVerticalSpacing(30);    //控件与控件之间的上下边距

    paramGroup = new QGroupBox();
    paramGroup->setLayout(leftLlayout);
    paramGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /*******************下面的信息是定义左右两个QVBoxLayout容器*************************
    ********************每一个分别纵向放置5个QGridLayout容器，装载床位的信息**************/

    //定义左侧QVBoxLayout容器
    cvlayout = new QVBoxLayout();
    BedLayout1= new QGridLayout();
    BedLayout1->setVerticalSpacing(20);  //控件之间的纵向间距
    BedLayout1->addWidget(OpenButton1,0,0);   //0行0列
    BedLayout1->addWidget(COMLabel1,1,0);     //1行0列
    BedLayout1->addWidget(COMBox1,2,0);       //2行0列
    BedLayout1->addWidget(ClearRecvButton1,3,0);   //3行0列
    BedLayout1->addWidget(BedLabel1,0,1,5,1);      //0行1列，并占据5行1列的空间大小
    BedLayout1->addWidget(LightLabel1,0,2,5,1);      //0行1列，并占据5行1列的空间大小
    BedLayout1->addWidget(plotHeartRate1, 0,3,5,1);  //0行2列，并占据5行1列的空间大小
    BedLayout1->addWidget(HeartRateLabel1,0,4,5,1);  //0行3列，并占据5行1列的空间大小
    BedLayout1->addWidget(plotBreathing1, 0,5,5,1);  //0行4列，并占据5行1列的空间大小
    BedLayout1->addWidget(BreathingLabel1,0,6,5,1);  //0行5列，并占据5行1列的空间大小

    //设置每一列的所占空间的比例，同理可以设置每一行所占空间的比例
    BedLayout1->setColumnStretch(0, 2);
    BedLayout1->setColumnStretch(1, 2);
    BedLayout1->setColumnStretch(2, 1);
    BedLayout1->setColumnStretch(3, 6);
    BedLayout1->setColumnStretch(4, 2);
    BedLayout1->setColumnStretch(5, 6);
    BedLayout1->setColumnStretch(6, 2);
    cvlayout->addLayout(BedLayout1);      //往cvlayout内部添加BedLayout1

    BedLayout2= new QGridLayout();
    BedLayout2->setVerticalSpacing(20);
    BedLayout2->addWidget(OpenButton2,0,0);
    BedLayout2->addWidget(COMLabel2,1,0);
    BedLayout2->addWidget(COMBox2,2,0);
    BedLayout2->addWidget(ClearRecvButton2,3,0);
    BedLayout2->addWidget(BedLabel2,0,1,5,1);
    BedLayout2->addWidget(LightLabel2,0,2,5,1);
    BedLayout2->addWidget(plotHeartRate2, 0,3,5,1);
    BedLayout2->addWidget(HeartRateLabel2,0,4,5,1);
    BedLayout2->addWidget(plotBreathing2, 0,5,5,1);
    BedLayout2->addWidget(BreathingLabel2,0,6,5,1);

    BedLayout2->setColumnStretch(0, 2);
    BedLayout2->setColumnStretch(1, 2);
    BedLayout2->setColumnStretch(2, 1);
    BedLayout2->setColumnStretch(3, 6);
    BedLayout2->setColumnStretch(4, 2);
    BedLayout2->setColumnStretch(5, 6);
    BedLayout2->setColumnStretch(6, 2);
    cvlayout->addLayout(BedLayout2);   //往cvlayout内部添加BedLayout1

    BedLayout3= new QGridLayout();
    BedLayout3->setVerticalSpacing(20);
    BedLayout3->addWidget(OpenButton3,0,0);
    BedLayout3->addWidget(COMLabel3,1,0);
    BedLayout3->addWidget(COMBox3,2,0);
    BedLayout3->addWidget(ClearRecvButton3,3,0);
    BedLayout3->addWidget(BedLabel3,0,1,5,1);
    BedLayout3->addWidget(LightLabel3,0,2,5,1);
    BedLayout3->addWidget(plotHeartRate3, 0,3,5,1);
    BedLayout3->addWidget(HeartRateLabel3,0,4,5,1);
    BedLayout3->addWidget(plotBreathing3, 0,5,5,1);
    BedLayout3->addWidget(BreathingLabel3,0,6,5,1);

    BedLayout3->setColumnStretch(0, 2);
    BedLayout3->setColumnStretch(1, 2);
    BedLayout3->setColumnStretch(2, 1);
    BedLayout3->setColumnStretch(3, 6);
    BedLayout3->setColumnStretch(4, 2);
    BedLayout3->setColumnStretch(5, 6);
    BedLayout3->setColumnStretch(6, 2);
    cvlayout->addLayout(BedLayout3);   //往cvlayout内部添加BedLayout1

    BedLayout4= new QGridLayout();
    BedLayout4->setVerticalSpacing(20);
    BedLayout4->addWidget(OpenButton4,0,0);
    BedLayout4->addWidget(COMLabel4,1,0);
    BedLayout4->addWidget(COMBox4,2,0);
    BedLayout4->addWidget(ClearRecvButton4,3,0);
    BedLayout4->addWidget(BedLabel4,0,1,5,1);
    BedLayout4->addWidget(LightLabel4,0,2,5,1);
    BedLayout4->addWidget(plotHeartRate4, 0,3,5,1);
    BedLayout4->addWidget(HeartRateLabel4,0,4,5,1);
    BedLayout4->addWidget(plotBreathing4, 0,5,5,1);
    BedLayout4->addWidget(BreathingLabel4,0,6,5,1);

    BedLayout4->setColumnStretch(0, 2);
    BedLayout4->setColumnStretch(1, 2);
    BedLayout4->setColumnStretch(2, 1);
    BedLayout4->setColumnStretch(3, 6);
    BedLayout4->setColumnStretch(4, 2);
    BedLayout4->setColumnStretch(5, 6);
    BedLayout4->setColumnStretch(6, 2);
    cvlayout->addLayout(BedLayout4);   //往cvlayout内部添加BedLayout1

    //定义centralLayout容器，一共包含3个子容器
    centralLayout = new QGridLayout(this);

    centralLayout->addWidget(paramGroup, 0, 0, 1, 1);  //位置在0行0列，且占据1行1列
    centralLayout->addLayout(cvlayout, 0, 1, 1, 1);    //位置在0行1列，且占据1行1列

    centralLayout->setColumnMinimumWidth(0, 150);   //centralLayout第一列至少150像素
    centralLayout->setColumnMinimumWidth(1, 100);   //centralLayout第二列至少100像素

    //设置两个子容器在centralLayout占据列的比例
    centralLayout->setColumnStretch(0, 0);
    centralLayout->setColumnStretch(1, 1);

    setLayout(centralLayout);   //把centralLayout设置为主容器

    //设置主窗口的背景颜色
    QPalette palWidget;
    palWidget.setColor(QPalette::Background,QColor(192,253,123));

    setAutoFillBackground(true);
    setPalette(palWidget);

    //串口1的计时器初始化
    CheckTimer1 = new QTimer(this);
    CheckTimer1->start(1000);
    SendTimer1 = new QTimer(this);
    connect(CheckTimer1, QTimer::timeout, this, CheckSerialsCom1);

    //串口2的计时器初始化
    CheckTimer2 = new QTimer(this);
    CheckTimer2->start(1000);
    SendTimer2 = new QTimer(this);
    connect(CheckTimer2, QTimer::timeout, this, CheckSerialsCom2);

    //串口3的计时器初始化
    CheckTimer3 = new QTimer(this);
    CheckTimer3->start(1000);
    SendTimer3 = new QTimer(this);
    connect(CheckTimer3, QTimer::timeout, this, CheckSerialsCom3);

    //串口4的计时器初始化
    CheckTimer4 = new QTimer(this);
    CheckTimer4->start(1000);
    SendTimer4 = new QTimer(this);
    connect(CheckTimer4, QTimer::timeout, this, CheckSerialsCom4);

    //初始化串口列表
    CheckSerialsCom1();
    CheckSerialsCom2();
    CheckSerialsCom3();
    CheckSerialsCom4();


    //开始串口1进程
    serialControllerCom1 = new SerialController;
    serialControllerCom1->moveToThread(&SerialThrCom1);
    SerialThrCom1.start();

    //开始串口2进程
    serialControllerCom2 = new SerialController;
    serialControllerCom2->moveToThread(&SerialThrCom2);
    SerialThrCom2.start();

    //开始串口3进程
    serialControllerCom3 = new SerialController;
    serialControllerCom3->moveToThread(&SerialThrCom3);
    SerialThrCom3.start();

    //开始串口4进程
    serialControllerCom4 = new SerialController;
    serialControllerCom4->moveToThread(&SerialThrCom4);
    SerialThrCom4.start();

    //connect开关串口1控制信号 以及是否成功的返回信号
    connect(this, requestOpenCom1, serialControllerCom1, SerialController::openSerial);
    connect(this, requestCloseCom1, serialControllerCom1, SerialController::closeSerial);
    connect(serialControllerCom1, SerialController::openSuccess, this, serialOpenedCom1);  //绑定串口打开成功信号发出时的接收方以及响应函数
    connect(serialControllerCom1, SerialController::openFailed, this, serialNotOpenedCom1);  //绑定串口打开失败信号发出时的接收方以及响应函数
    connect(serialControllerCom1, SerialController::closeSuccess, this, serialClosedCom1);   //绑定串口关闭成功信号发出时的接收方以及响应函数

    //connect波特率等参数的控制信号
    connect(this, setBaudRate, serialControllerCom1, SerialController::getBaudrate);
    connect(this, setStopBits, serialControllerCom1, SerialController::getStopbits);
    connect(this, setDataBits, serialControllerCom1, SerialController::getDatabits);
    connect(this, setParity, serialControllerCom1, SerialController::getParity);
    connect(BaudrateBox, QComboBox::currentTextChanged, serialControllerCom1, SerialController::getBaudrate);
    connect(StopbitsBox, QComboBox::currentTextChanged, serialControllerCom1, SerialController::getStopbits);
    connect(DatabitsBox, QComboBox::currentTextChanged, serialControllerCom1, SerialController::getDatabits);
    connect(ParityBox, QComboBox::currentTextChanged, serialControllerCom1, SerialController::getParity);
    connect(this, changeRTS, serialControllerCom1, SerialController::contrloRTS);
    connect(this, changeDTR, serialControllerCom1, SerialController::controlDTR);

    connect(serialControllerCom1, SerialController::recvData, this, getRecvCom1); //绑定串口1发出数据的信号以及接收方的响应函数

    //connect开关串口2控制信号 以及是否成功的返回信号
    connect(this, requestOpenCom2, serialControllerCom2, SerialController::openSerial);
    connect(this, requestCloseCom2, serialControllerCom2, SerialController::closeSerial);
    connect(serialControllerCom2, SerialController::openSuccess, this, serialOpenedCom2);
    connect(serialControllerCom2, SerialController::openFailed, this, serialNotOpenedCom2);
    connect(serialControllerCom2, SerialController::closeSuccess, this, serialClosedCom2);

    //connect波特率等参数的控制信号
    connect(this, setBaudRate, serialControllerCom2, SerialController::getBaudrate);
    connect(this, setStopBits, serialControllerCom2, SerialController::getStopbits);
    connect(this, setDataBits, serialControllerCom2, SerialController::getDatabits);
    connect(this, setParity, serialControllerCom2, SerialController::getParity);
    connect(BaudrateBox, QComboBox::currentTextChanged, serialControllerCom2, SerialController::getBaudrate);
    connect(StopbitsBox, QComboBox::currentTextChanged, serialControllerCom2, SerialController::getStopbits);
    connect(DatabitsBox, QComboBox::currentTextChanged, serialControllerCom2, SerialController::getDatabits);
    connect(ParityBox, QComboBox::currentTextChanged, serialControllerCom2, SerialController::getParity);
    connect(this, changeRTS, serialControllerCom2, SerialController::contrloRTS);
    connect(this, changeDTR, serialControllerCom2, SerialController::controlDTR);

    connect(serialControllerCom2, SerialController::recvData, this, getRecvCom2);

    //connect开关串口3控制信号 以及是否成功的返回信号
    connect(this, requestOpenCom3, serialControllerCom3, SerialController::openSerial);
    connect(this, requestCloseCom3, serialControllerCom3, SerialController::closeSerial);
    connect(serialControllerCom3, SerialController::openSuccess, this, serialOpenedCom3);
    connect(serialControllerCom3, SerialController::openFailed, this, serialNotOpenedCom3);
    connect(serialControllerCom3, SerialController::closeSuccess, this, serialClosedCom3);

    //connect波特率等参数的控制信号
    connect(this, setBaudRate, serialControllerCom3, SerialController::getBaudrate);
    connect(this, setStopBits, serialControllerCom3, SerialController::getStopbits);
    connect(this, setDataBits, serialControllerCom3, SerialController::getDatabits);
    connect(this, setParity, serialControllerCom3, SerialController::getParity);
    connect(BaudrateBox, QComboBox::currentTextChanged, serialControllerCom3, SerialController::getBaudrate);
    connect(StopbitsBox, QComboBox::currentTextChanged, serialControllerCom3, SerialController::getStopbits);
    connect(DatabitsBox, QComboBox::currentTextChanged, serialControllerCom3, SerialController::getDatabits);
    connect(ParityBox, QComboBox::currentTextChanged, serialControllerCom3, SerialController::getParity);
    connect(this, changeRTS, serialControllerCom3, SerialController::contrloRTS);
    connect(this, changeDTR, serialControllerCom3, SerialController::controlDTR);

    connect(serialControllerCom3, SerialController::recvData, this, getRecvCom3);

    //connect开关串口4控制信号 以及是否成功的返回信号
    connect(this, requestOpenCom4, serialControllerCom4, SerialController::openSerial);
    connect(this, requestCloseCom4, serialControllerCom4, SerialController::closeSerial);
    connect(serialControllerCom4, SerialController::openSuccess, this, serialOpenedCom4);
    connect(serialControllerCom4, SerialController::openFailed, this, serialNotOpenedCom4);
    connect(serialControllerCom4, SerialController::closeSuccess, this, serialClosedCom4);

    //connect波特率等参数的控制信号
    connect(this, setBaudRate, serialControllerCom4, SerialController::getBaudrate);
    connect(this, setStopBits, serialControllerCom4, SerialController::getStopbits);
    connect(this, setDataBits, serialControllerCom4, SerialController::getDatabits);
    connect(this, setParity, serialControllerCom4, SerialController::getParity);
    connect(BaudrateBox, QComboBox::currentTextChanged, serialControllerCom4, SerialController::getBaudrate);
    connect(StopbitsBox, QComboBox::currentTextChanged, serialControllerCom4, SerialController::getStopbits);
    connect(DatabitsBox, QComboBox::currentTextChanged, serialControllerCom4, SerialController::getDatabits);
    connect(ParityBox, QComboBox::currentTextChanged, serialControllerCom4, SerialController::getParity);
    connect(this, changeRTS, serialControllerCom4, SerialController::contrloRTS);
    connect(this, changeDTR, serialControllerCom4, SerialController::controlDTR);

    connect(serialControllerCom4, SerialController::recvData, this, getRecvCom4);

}

ostream &os = cout;
/*****************************************************************************
 ***********************    这里比较重要： 检测串口    **************************
******************************************************************************/

void MainWidget::CheckSerialsCom1()
{
    //不断检查可用串口列表，并与当前列表进行比较，若发生变化则重新生成列表
    emit sendDateTime(QDateTime::currentDateTime().toString());//更新状态栏时间

    QList<QSerialPortInfo> SerialList1 = QSerialPortInfo::availablePorts();  //获取可用的串口号
    if (!SerialList1.isEmpty())      //获取的串口号不为空
    {
        QStringList TmpComList1, TmpPortNameList1, TmpDesList1;
        for (QSerialPortInfo serial : SerialList1)
        {
            TmpComList1 << serial.portName() + " " + serial.description();  //获得每一个可用串口的名称和描述
            TmpPortNameList1<< serial.portName();
            TmpDesList1 << serial.description();
        }
        if (COMList1 != TmpComList1)
        {
            //TODO: 可用串口发生改变却不为空时，保持串口打开状态，及其他细节的处理
            //只在串口发生变化时刷新ComboBox
            COMList1 = TmpComList1;
            PortNameList1 = TmpPortNameList1;
            DescList1 = TmpDesList1;
            COMBox1->setDisabled(false);
            COMBox1->clear();
            COMBox1->addItems(PortNameList1);
            OpenButton1->setDisabled(false);
            for (int i = 0; i < COMList1.count(); i++)   //为串口列表增加ToolTip
            {
                COMBox1->setItemData(i, DescList1[i], Qt::ToolTipRole);
                //os<<PortNameList1[i].toLatin1().data()<<endl;   //cout: USB-SERIAL CH340
            }
        }
    }
    else    //可用串口为空时发送关闭串口信号
    {
        COMBox1->clear();
        COMList1.clear();
        COMBox1->addItem(tr("(空)"));
        COMBox1->setDisabled(true);
        OpenButton1->setDisabled(true);
        emit CloseSerialCom1();
        isOpened1 = false;
    }
    //os<<PortNameList1[COMBox1->currentIndex()].toLatin1().data()<<endl;  //获取当前的串口号
    COMLabel1->setText(QString("串口号 : ")+PortNameList1[COMBox1->currentIndex()]);   //串口号显示标签中显示被选中的串口号
}

void MainWidget::CheckSerialsCom2()
{
    //不断检查可用串口列表，并与当前列表进行比较，若发生变化则重新生成列表
    emit sendDateTime(QDateTime::currentDateTime().toString());//更新状态栏时间

    QList<QSerialPortInfo> SerialList2 = QSerialPortInfo::availablePorts();
    if (!SerialList2.isEmpty())
    {
        QStringList TmpComList2, TmpPortNameList2, TmpDesList2;
        for (QSerialPortInfo serial : SerialList2)
        {
            TmpComList2 << serial.portName() + " " + serial.description();
            TmpPortNameList2<< serial.portName();
            TmpDesList2 << serial.description();
        }
        if (COMList2 != TmpComList2)
        {
            //TODO: 可用串口发生改变却不为空时，保持串口打开状态，及其他细节的处理
            //只在串口发生变化时刷新ComboBox
            COMList2 = TmpComList2;
            PortNameList2 = TmpPortNameList2;
            DescList2 = TmpDesList2;
            COMBox2->setDisabled(false);
            COMBox2->clear();
            COMBox2->addItems(PortNameList2);
            OpenButton2->setDisabled(false);
            for (int i = 0; i < COMList2.count(); i++)   //为串口列表增加ToolTip
            {
                COMBox2->setItemData(i, DescList2[i], Qt::ToolTipRole);
                //os<<PortNameList1[i].toLatin1().data()<<endl;   //cout: USB-SERIAL CH340
            }
        }
    }
    else    //可用串口为空时发送关闭串口信号
    {
        COMBox2->clear();
        COMList2.clear();
        COMBox2->addItem(tr("(空)"));
        COMBox2->setDisabled(true);
        OpenButton2->setDisabled(true);
        emit CloseSerialCom2();
        isOpened2 = false;
    }
    //os<<PortNameList2[COMBox2->currentIndex()].toLatin1().data()<<endl;  //获取当前的串口号
    COMLabel2->setText(QString("串口号 : ")+PortNameList2[COMBox2->currentIndex()]);
}

void MainWidget::CheckSerialsCom3()
{
    //不断检查可用串口列表，并与当前列表进行比较，若发生变化则重新生成列表
    emit sendDateTime(QDateTime::currentDateTime().toString());//更新状态栏时间

    QList<QSerialPortInfo> SerialList3 = QSerialPortInfo::availablePorts();
    if (!SerialList3.isEmpty())
    {
        QStringList TmpComList3, TmpPortNameList3, TmpDesList3;
        for (QSerialPortInfo serial : SerialList3)
        {
            TmpComList3 << serial.portName() + " " + serial.description();
            TmpPortNameList3<< serial.portName();
            TmpDesList3 << serial.description();
        }
        if (COMList3 != TmpComList3)
        {
            //TODO: 可用串口发生改变却不为空时，保持串口打开状态，及其他细节的处理
            //只在串口发生变化时刷新ComboBox
            COMList3 = TmpComList3;
            PortNameList3 = TmpPortNameList3;
            DescList3 = TmpDesList3;
            COMBox3->setDisabled(false);
            COMBox3->clear();
            COMBox3->addItems(PortNameList3);
            OpenButton3->setDisabled(false);
            for (int i = 0; i < COMList3.count(); i++)   //为串口列表增加ToolTip
            {
                COMBox3->setItemData(i, DescList3[i], Qt::ToolTipRole);
                //os<<PortNameList1[i].toLatin1().data()<<endl;   //cout: USB-SERIAL CH340
            }
        }
    }
    else    //可用串口为空时发送关闭串口信号
    {
        COMBox3->clear();
        COMList3.clear();
        COMBox3->addItem(tr("(空)"));
        COMBox3->setDisabled(true);
        OpenButton3->setDisabled(true);
        emit CloseSerialCom3();
        isOpened3 = false;
    }
    //os<<PortNameList1[COMBox1->currentIndex()].toLatin1().data()<<endl;  //获取当前的串口号
    COMLabel3->setText(QString("串口号 : ")+PortNameList3[COMBox3->currentIndex()]);
}

void MainWidget::CheckSerialsCom4()
{
    //不断检查可用串口列表，并与当前列表进行比较，若发生变化则重新生成列表
    emit sendDateTime(QDateTime::currentDateTime().toString());//更新状态栏时间

    QList<QSerialPortInfo> SerialList4 = QSerialPortInfo::availablePorts();
    if (!SerialList4.isEmpty())
    {
        QStringList TmpComList4, TmpPortNameList4, TmpDesList4;
        for (QSerialPortInfo serial : SerialList4)
        {
            TmpComList4 << serial.portName() + " " + serial.description();
            TmpPortNameList4<< serial.portName();
            TmpDesList4 << serial.description();
        }
        if (COMList4 != TmpComList4)
        {
            //TODO: 可用串口发生改变却不为空时，保持串口打开状态，及其他细节的处理
            //只在串口发生变化时刷新ComboBox
            COMList4 = TmpComList4;
            PortNameList4 = TmpPortNameList4;
            DescList4 = TmpDesList4;
            COMBox4->setDisabled(false);
            COMBox4->clear();
            COMBox4->addItems(PortNameList4);
            OpenButton4->setDisabled(false);
            for (int i = 0; i < COMList4.count(); i++)   //为串口列表增加ToolTip
            {
                COMBox4->setItemData(i, DescList4[i], Qt::ToolTipRole);
                //os<<PortNameList1[i].toLatin1().data()<<endl;   //cout: USB-SERIAL CH340
            }
        }
    }
    else    //可用串口为空时发送关闭串口信号
    {
        COMBox4->clear();
        COMList4.clear();
        COMBox4->addItem(tr("(空)"));
        COMBox4->setDisabled(true);
        OpenButton4->setDisabled(true);
        emit CloseSerialCom4();
        isOpened4 = false;
    }
    //os<<PortNameList1[COMBox1->currentIndex()].toLatin1().data()<<endl;  //获取当前的串口号
    COMLabel4->setText(QString("串口号 : ")+PortNameList4[COMBox4->currentIndex()]);
}

MainWidget::~MainWidget()
{
    SerialThrCom1.terminate();
    //SerialThrCom2.terminate();
    //SerialThrCom3.terminate();
    //SerialThrCom4.terminate();

    settings.setValue("Baud rate", BaudrateBox->currentIndex());
    settings.setValue("Stop bits", StopbitsBox->currentIndex());
    settings.setValue("Data bits", DatabitsBox->currentIndex());
    settings.setValue("Parity", ParityBox->currentIndex());
}

void MainWidget::RTSControl(int state)
{
    if(isOpened1||isOpened2||isOpened3||isOpened4)
    {
        if(state == 2)
            emit changeRTS(true);
        else if(state == 0)
            emit changeRTS(false);
    }
}

void MainWidget::DTRControl(int state)
{
    if(isOpened1||isOpened2||isOpened3||isOpened4)
    {
        if(state == 2)
            emit changeDTR(true);
        else if(state == 0)
            emit changeDTR(false);
    }
}

//串口1打开成功的信号发出时的接收响应函数
void MainWidget::serialOpenedCom1()
{
    isOpened1= true;   //串口1打开标志位为真
    //串口复选框显示当前Com口
    int portIndex = COMBox1->currentIndex();
    emit sendStatus(QString(PortNameList1[portIndex] + tr(" ") + DescList1[portIndex]));
    //串口关闭时无法设置RTS,DTR信号（但此时checkbox仍然是可操作的），串口打开时发送stateChanged信号来应用改变
    emit RTSBox->stateChanged(RTSBox->checkState());
    emit DTRBox->stateChanged(DTRBox->checkState());
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom1(true);
}

//串口1打开失败的信号发出时的接收响应函数
void MainWidget::serialNotOpenedCom1()
{
    //设置状态栏 并发出警告音
    //TODO: 此处应有更容易察觉的提示
    emit sendStatus(tr("串口打开失败"));
    QApplication::beep();
}

//串口1关闭成功的信号发出时的接收响应函数
void MainWidget::serialClosedCom1()
{
    isOpened1 = false;
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom1(false);
    emit sendStatus(tr("串口已关闭"));
}

//触发“打开串口”按钮后的调用函数： 主要获取当前可用串口1的相关信息
void MainWidget::OpenSerialCom1()
{
    QString portName = COMBox1->currentText();
    emit requestOpenCom1(portName);   //判断所选信号是否可用
    emit setBaudRate(BaudrateBox->currentText());
    emit setStopBits(StopbitsBox->currentText());
    emit setDataBits(DatabitsBox->currentText());
    emit setParity(ParityBox->currentText());
}

void MainWidget::CloseSerialCom1()
{
    emit requestCloseCom1();
}

void MainWidget::serialOpenedCom2()
{
    isOpened2= true;
    int portIndex = COMBox2->currentIndex();
    emit sendStatus(QString(PortNameList2[portIndex] + tr(" ") + DescList2[portIndex]));
    //串口关闭时无法设置RTS,DTR信号（但此时checkbox仍然是可操作的），串口打开时发送stateChanged信号来应用改变
    emit RTSBox->stateChanged(RTSBox->checkState());
    emit DTRBox->stateChanged(DTRBox->checkState());
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom2(true);
}

void MainWidget::serialNotOpenedCom2()
{
    //设置状态栏 并发出警告音
    //TODO: 此处应有更容易察觉的提示
    emit sendStatus(tr("串口打开失败"));
    QApplication::beep();
}

//当串口关闭后，标志位改变，并控制相关控件属性
void MainWidget::serialClosedCom2()
{
    isOpened2 = false;
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom2(false);
    emit sendStatus(tr("串口已关闭"));
}

//触发“打开串口”按钮后的调用函数： 主要获取当前可用串口2的相关信息
void MainWidget::OpenSerialCom2()
{
    QString portName = COMBox2->currentText();
    emit requestOpenCom2(portName);
    emit setBaudRate(BaudrateBox->currentText());
    emit setStopBits(StopbitsBox->currentText());
    emit setDataBits(DatabitsBox->currentText());
    emit setParity(ParityBox->currentText());
}

void MainWidget::CloseSerialCom2()
{
    emit requestCloseCom2();
}

void MainWidget::serialOpenedCom3()
{
    isOpened3= true;
    int portIndex = COMBox3->currentIndex();
    emit sendStatus(QString(PortNameList3[portIndex] + tr(" ") + DescList3[portIndex]));
    //串口关闭时无法设置RTS,DTR信号（但此时checkbox仍然是可操作的），串口打开时发送stateChanged信号来应用改变
    emit RTSBox->stateChanged(RTSBox->checkState());
    emit DTRBox->stateChanged(DTRBox->checkState());
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom3(true);
}

void MainWidget::serialNotOpenedCom3()
{
    //设置状态栏 并发出警告音
    //TODO: 此处应有更容易察觉的提示
    emit sendStatus(tr("串口打开失败"));
    QApplication::beep();
}

//当串口关闭后，标志位改变，并控制相关控件属性
void MainWidget::serialClosedCom3()
{
    isOpened3 = false;
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom3(false);
    emit sendStatus(tr("串口已关闭"));
}

//触发“打开串口”按钮后的调用函数： 主要获取当前可用串口3的相关信息
void MainWidget::OpenSerialCom3()
{
    QString portName = COMBox3->currentText();
    emit requestOpenCom3(portName);
    emit setBaudRate(BaudrateBox->currentText());
    emit setStopBits(StopbitsBox->currentText());
    emit setDataBits(DatabitsBox->currentText());
    emit setParity(ParityBox->currentText());
}

void MainWidget::CloseSerialCom3()
{
    emit requestCloseCom3();
}

void MainWidget::serialOpenedCom4()
{
    isOpened4= true;
    int portIndex = COMBox4->currentIndex();
    emit sendStatus(QString(PortNameList4[portIndex] + tr(" ") + DescList4[portIndex]));
    //串口关闭时无法设置RTS,DTR信号（但此时checkbox仍然是可操作的），串口打开时发送stateChanged信号来应用改变
    emit RTSBox->stateChanged(RTSBox->checkState());
    emit DTRBox->stateChanged(DTRBox->checkState());
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom4(true);
}

void MainWidget::serialNotOpenedCom4()
{
    //设置状态栏 并发出警告音
    //TODO: 此处应有更容易察觉的提示
    emit sendStatus(tr("串口打开失败"));
    QApplication::beep();
}

//当串口关闭后，标志位改变，并控制相关控件属性
void MainWidget::serialClosedCom4()
{
    isOpened1 = false;
    //相应控件可用性做出改变(setDisabled)
    ACtionAttachToSerialCom4(false);
    emit sendStatus(tr("串口已关闭"));
}

//触发“打开串口”按钮后的调用函数： 主要获取当前可用串口4的相关信息
void MainWidget::OpenSerialCom4()
{
    QString portName = COMBox4->currentText();
    emit requestOpenCom4(portName);
    emit setBaudRate(BaudrateBox->currentText());
    emit setStopBits(StopbitsBox->currentText());
    emit setDataBits(DatabitsBox->currentText());
    emit setParity(ParityBox->currentText());
}

void MainWidget::CloseSerialCom4()
{
    emit requestCloseCom4();
}

//获得串口1的心率和呼吸数据并在图表中绘图
deque<double> valueOfHeartRate1, valueOfBreath1;  //心率和呼吸的队列（主要在首尾进行插入和删除）
QByteArray recvTempCom1;
void MainWidget::getRecvCom1(QByteArray recv)
{
    recvTempCom1.append(recv);
    if(recvTempCom1.size()==43){
        //cout<<recvTempCom1.toHex().data()<<endl;
        QString str = QString(recvTempCom1.toHex());  //先把QByteArray全部转换为字符串，直接使用recv会出现乱码
        //cout<<recvTempCom1.toHex().data()<<endl;
        for(size_t i=2,j=22;i<22,j<42;){
            bool ok;
            //传输心率和呼吸的1Byte数据并进行绘制波形，mid(j,2)表示从下标j开始取两个字符，
            //toInt(&ok,16)后面的16表示十六进制字符串转十进制int，若转换成功，则ok为ture
            realtimeDataSlotCom1(str.mid(j,2).toInt(&ok,16),str.mid(i,2).toInt(&ok,16));
            //cout<<str.mid(j,2).toInt(&ok,16)<<endl;
            //cout<<"-------------------*****************-----------------------"<<endl;
            i+=2;
            j+=2;
        }
        recvTempCom1.clear();
    }
}


//获得串口2的心率和呼吸数据并在图表中绘图
deque<double> valueOfHeartRate2, valueOfBreath2;
QByteArray recvTempCom2;
void MainWidget::getRecvCom2(QByteArray recv)
{
    recvTempCom2.append(recv);
    if(recvTempCom2.size()==43){
        //cout<<recvTempCom1.toHex().data()<<endl;
        QString str = QString(recvTempCom2.toHex());  //先把QByteArray全部转换为字符串，直接使用recv会出现乱码
        //cout<<recvTempCom1.toHex().data()<<endl;
        for(size_t i=2,j=22;i<22,j<42;){
            bool ok;
            //传输心率和呼吸的1Byte数据并进行绘制波形，mid(j,2)表示从下标j开始取两个字符，
            //toInt(&ok,16)后面的16表示十六进制字符串转十进制int，若转换成功，则ok为ture
            realtimeDataSlotCom2(str.mid(j,2).toInt(&ok,16),str.mid(i,2).toInt(&ok,16));
            //cout<<str.mid(j,2).toInt(&ok,16)<<endl;
            //cout<<"-------------------*****************-----------------------"<<endl;
            i+=2;
            j+=2;
        }
        recvTempCom2.clear();
    }
}

//获得串口3的心率和呼吸数据并在图表中绘图
deque<double> valueOfHeartRate3, valueOfBreath3;

QByteArray recvTempCom3;
void MainWidget::getRecvCom3(QByteArray recv)
{
    recvTempCom3.append(recv);
    if(recvTempCom3.size()==43){
        //cout<<recvTempCom1.toHex().data()<<endl;
        QString str = QString(recvTempCom3.toHex());  //先把QByteArray全部转换为字符串，直接使用recv会出现乱码
        //cout<<recvTempCom1.toHex().data()<<endl;
        for(size_t i=2,j=22;i<22,j<42;){
            bool ok;
            //传输心率和呼吸的1Byte数据并进行绘制波形，mid(j,2)表示从下标j开始取两个字符，
            //toInt(&ok,16)后面的16表示十六进制字符串转十进制int，若转换成功，则ok为ture
            realtimeDataSlotCom3(str.mid(j,2).toInt(&ok,16),str.mid(i,2).toInt(&ok,16));
            //cout<<str.mid(j,2).toInt(&ok,16)<<endl;
            //cout<<"-------------------*****************-----------------------"<<endl;
            i+=2;
            j+=2;
        }
        recvTempCom3.clear();
    }
}


//获得串口4的心率和呼吸数据并在图表中绘图
deque<double> valueOfHeartRate4, valueOfBreath4;

QByteArray recvTempCom4;
void MainWidget::getRecvCom4(QByteArray recv)
{
    recvTempCom4.append(recv);
    if(recvTempCom4.size()==43){
        //cout<<recvTempCom1.toHex().data()<<endl;
        QString str = QString(recvTempCom4.toHex());  //先把QByteArray全部转换为字符串，直接使用recv会出现乱码
        //cout<<recvTempCom1.toHex().data()<<endl;
        for(size_t i=2,j=22;i<22,j<42;){
            bool ok;
            //传输心率和呼吸的1Byte数据并进行绘制波形，mid(j,2)表示从下标j开始取两个字符，
            //toInt(&ok,16)后面的16表示十六进制字符串转十进制int，若转换成功，则ok为ture
            realtimeDataSlotCom4(str.mid(j,2).toInt(&ok,16),str.mid(i,2).toInt(&ok,16));
            //cout<<str.mid(j,2).toInt(&ok,16)<<endl;
            //cout<<"-------------------*****************-----------------------"<<endl;
            i+=2;
            j+=2;
        }
        recvTempCom4.clear();
    }
}

//16进制字符串的string转换成16进制对应的内容
QString MainWidget::HexStringToString(QString hexstr)
{
    //将内容为16进制字符串的string转换成16进制对应的内容
    QStringList list = hexstr.split(' ');
    QByteArray bit;
    foreach (QString str, list)
    {
        if (!str.isEmpty())
            bit.append(str.toInt(nullptr, 16));
    }
    return QString::fromLocal8Bit(bit);
}


//根据串口1的开关状态控制一些控件的属性
void MainWidget::ACtionAttachToSerialCom1(bool set)
{
    //根据串口开关状态决定一些控件的可用性
    if(set)
    {
        OpenButton1->setText(tr("关闭串口"));
        COMBox1->setDisabled(true);
        disconnect(OpenButton1, QPushButton::clicked, this, OpenSerialCom1);  //取消“打开串口”按钮与OpenSerialCom1的绑定
        connect(OpenButton1, QPushButton::clicked, this, CloseSerialCom1);    //开启“打开串口”按钮与CloseSerialCom1的绑定
    }
    else
    {
        OpenButton1->setText(tr("打开串口"));
        COMBox1->setDisabled(false);
        disconnect(OpenButton1, QPushButton::clicked, this, CloseSerialCom1);
        connect(OpenButton1, QPushButton::clicked, this, OpenSerialCom1);
    }
}


//根据串口2的开关状态控制一些控件的属性
void MainWidget::ACtionAttachToSerialCom2(bool set)
{
    //根据串口开关状态决定一些控件的可用性
    if(set)
    {
        OpenButton2->setText(tr("关闭串口"));
        COMBox2->setDisabled(true);
        disconnect(OpenButton2, QPushButton::clicked, this, OpenSerialCom2);
        connect(OpenButton2, QPushButton::clicked, this, CloseSerialCom2);
    }
    else
    {
        OpenButton2->setText(tr("打开串口"));
        COMBox2->setDisabled(false);
        disconnect(OpenButton2, QPushButton::clicked, this, CloseSerialCom2);
        connect(OpenButton2, QPushButton::clicked, this, OpenSerialCom2);
    }
}


//根据串口3的开关状态控制一些控件的属性
void MainWidget::ACtionAttachToSerialCom3(bool set)
{
    //根据串口开关状态决定一些控件的可用性
    if(set)
    {
        OpenButton3->setText(tr("关闭串口"));
        COMBox3->setDisabled(true);
        disconnect(OpenButton3, QPushButton::clicked, this, OpenSerialCom3);
        connect(OpenButton3, QPushButton::clicked, this, CloseSerialCom3);
    }
    else
    {
        OpenButton3->setText(tr("打开串口"));
        COMBox3->setDisabled(false);
        disconnect(OpenButton3, QPushButton::clicked, this, CloseSerialCom3);
        connect(OpenButton3, QPushButton::clicked, this, OpenSerialCom3);
    }
}







//根据串口1的开关状态控制一些控件的属性
void MainWidget::ACtionAttachToSerialCom4(bool set)
{
    //根据串口开关状态决定一些控件的可用性
    if(set)
    {
        OpenButton4->setText(tr("关闭串口"));
        COMBox4->setDisabled(true);
        disconnect(OpenButton4, QPushButton::clicked, this, OpenSerialCom4);
        connect(OpenButton4, QPushButton::clicked, this, CloseSerialCom4);
    }
    else
    {
        OpenButton4->setText(tr("打开串口"));
        COMBox4->setDisabled(false);
        disconnect(OpenButton4, QPushButton::clicked, this, CloseSerialCom4);
        connect(OpenButton4, QPushButton::clicked, this, OpenSerialCom4);
    }
}



//负责接收一个字节心率和呼吸的数据，并且在相应位置显示
//形参：对应的一个心率和呼吸的数据
void MainWidget::realtimeDataSlotCom1(double valueHeartRate,double valueBreath){

    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;  //key为当前时间
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    plotHeartRate1->graph(0)->addData(key, valueHeartRate);//添加心率数据到曲线1
    plotBreathing1->graph(0)->addData(key,  valueBreath);  //添加呼吸数据到曲线2
    //删除n秒之前的数据。这里的n要和下面设置横坐标宽度的n配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    plotHeartRate1->graph(0)->removeDataBefore(key-20);
    plotBreathing1->graph(0)->removeDataBefore(key-16);
    //自动设定graph(0)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    plotHeartRate1->graph(0)->rescaleValueAxis(true);
    plotBreathing1->graph(0)->rescaleValueAxis(true);

    //这里的n，是指横坐标时间宽度为n秒，如果想要横坐标显示更多的时间
    //就把n调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-n)中的8也要改成60，否则曲线显示不完整。
    plotHeartRate1->xAxis->setRange(key+0.08, 20, Qt::AlignRight);//设定x轴的范围
    plotHeartRate1->replot();

    plotBreathing1->xAxis->setRange(key+0.08, 16, Qt::AlignRight);//设定x轴的范围
    plotBreathing1->replot();
    //cout<<valueBreath<<endl;
    valueOfHeartRate1.push_back(valueHeartRate);   //将该心率数据压入到队列
    valueOfBreath1.push_back(valueBreath);         //将该呼吸数据压入到队列

    //说明已经读取了60s的数据，现在可以进行提取有效心率数
    vector<int> maxValueIndex,minValueIndex;   //定义存放心率队列数据的极值vector
    if(valueOfHeartRate1.size()==729&&valueOfBreath1.size()==729){   //心率数据计数到一分钟的729个数据
        int heartRateCount = 0;    //中间过程的临时心率次数
        int heartRatendCount=0;   //最终计算的心率次数
        int breathEndCount=0;      //最终计算的呼吸次数
        //for_each(valueOfHeartRate.cbegin(),valueOfHeartRate.cend(),[&os](const double value){ cout<<value<<" "<<endl; });
        findPeaks(valueOfHeartRate1,maxValueIndex,minValueIndex);  //查找729个心率数据的极值并压入对应的vector中
        //cout<<maxValueIndex.size()<<endl;
        //cout<<minValueIndex.size()<<endl;

        for(size_t i = 0; i < min(maxValueIndex.size(),minValueIndex.size());++i){
            //cout<<valueOfHeartRate1[maxValueIndex[i]]<<"  "<<valueOfHeartRate1[minValueIndex[i]]<<endl;
            if(valueOfHeartRate1[maxValueIndex[i]]-valueOfHeartRate1[minValueIndex[i]] > 60)  //相邻极大值和极小值的阈值差及判断标准
                heartRateCount ++;  //加1
        }
        //for_each(maxValueIndex.cbegin(),maxValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        //for_each(minValueIndex.cbegin(),minValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        heartRatendCount = (heartRateCount/2-20);   //原带算法未知，此值根据实际情况调整
        breathEndCount = findPulse(valueOfBreath1);  //计算呼吸数据的有效数量
        //cout<<"Com1 : "<<endCount<<endl;
        //cout<<breathEndCount<<endl;
        valueOfHeartRate1.erase(valueOfHeartRate1.begin(),valueOfHeartRate1.begin()+10);  //每相隔10个数据进行一次1分钟的有效心率数统计(所以要清除掉队列的最前面的10个数据)
        valueOfBreath1.erase(valueOfBreath1.begin(),valueOfBreath1.begin()+10);
        //判断检测到的有效心率数是否符合要求，并更新显示标签

        std::cout<<xinlv1<<endl;
        std::cout<<huxi1<<endl;

        if(heartRatendCount < xinlv1 &&breathEndCount < huxi1){
            HeartRateLabel1->setText(QString("    心率\n\n     无"));  //检测到不合理心率
            BreathingLabel1->setText(QString("    呼吸\n\n     无"));  //检测不到合理呼吸
            BedLabel1->setText(tr("   1号床位\n\n   : 离床"));
            LightLabel1->setStyleSheet("background-color:red"); //底色

            //  下面的程序为往数据库对应该串口的信息表中插入时间、呼吸以及心率数据
            if(isOpenDataBase){
                string   time = getTime();
                string   date = getDate();
                cout << time << endl;
                QSqlQuery query(db); //查询表并输出，测试能否正常操作数据库
                query.exec("select * from 异常信息_info where 雷达1_呼吸 is NULL");   //查询雷达1_呼吸是否为NULL
                if(query.numRowsAffected()>0){
                    query.prepare(" UPDATE top(1) 异常信息_info set 雷达1=1,雷达1_日期=:date,雷达1_时间=:time,雷达1_心率=:xinlv1,雷达1_呼吸=:huxi1,雷达1_是否在床 = 'NO' where 雷达1_呼吸 IS NULL");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv1",xinlv1);
                    query.bindValue(":huxi1",huxi1);
                    query.exec();
                }
                else{
                    query.prepare("insert into 异常信息_info(雷达1,雷达1_日期,雷达1_时间,雷达1_心率,雷达1_呼吸,雷达1_是否在床) VALUES(1,:date,:time,:xinlv1,:huxi1,'NO')");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv1",xinlv1);
                    query.bindValue(":huxi1",huxi1);
                    query.exec();
                }

                /*
                while(query.next())
                {
                    qDebug()<<query.value(0).toInt() <<query.value(1).toString() <<query.value(2).toInt();
                }

                QDialog *mainDialog = new QDialog;          //打开新的对话框
                QTableView *tableView = new QTableView;     //打开新的表
                QHBoxLayout *layout = new QHBoxLayout;      //添加布局
                layout->addWidget(tableView);
                mainDialog->setLayout(layout);
                QSqlQueryModel *model = new QSqlQueryModel;//新数据库模型
                model->setQuery(QObject::tr("select * from yichang2_info"));//确定选择哪个表
                tableView->setModel(model);//适应打开的表
                mainDialog->adjustSize();//适应标的大小
                */
            }

        }
        else if(heartRatendCount > xinlv1 &&breathEndCount > huxi1){
            HeartRateLabel1->setText(QString("    心率\n\n     ")+QString::number(heartRatendCount,10));  //更新心率显示标签
            BreathingLabel1->setText(QString("    呼吸\n\n     ")+QString::number(breathEndCount,10));  //检测不到合理呼吸
            BedLabel1->setText(tr("   1号床位\n\n   : 在床"));
            LightLabel1->setStyleSheet("background-color:white"); //底色
        }
    }
}

//负责接收一个字节心率和呼吸的数据，并且在相应位置显示
//形参：对应的一个心率和呼吸的数据
void MainWidget::realtimeDataSlotCom2(double valueHeartRate,double valueBreath){
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;  //key为当前时间
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    plotHeartRate2->graph(0)->addData(key, valueHeartRate);//添加心率数据到曲线1
    plotBreathing2->graph(0)->addData(key,  valueBreath);  //添加呼吸数据到曲线2
    //删除n秒之前的数据。这里的n要和下面设置横坐标宽度的n配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    plotHeartRate2->graph(0)->removeDataBefore(key-20);
    plotBreathing2->graph(0)->removeDataBefore(key-16);
    //自动设定graph(0)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    plotHeartRate2->graph(0)->rescaleValueAxis(true);
    plotBreathing2->graph(0)->rescaleValueAxis(true);

    //这里的n，是指横坐标时间宽度为n秒，如果想要横坐标显示更多的时间
    //就把n调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-n)中的8也要改成60，否则曲线显示不完整。
    plotHeartRate2->xAxis->setRange(key+0.08, 20, Qt::AlignRight);//设定x轴的范围
    plotHeartRate2->replot();

    plotBreathing2->xAxis->setRange(key+0.08, 16, Qt::AlignRight);//设定x轴的范围
    plotBreathing2->replot();

    valueOfHeartRate2.push_back(valueHeartRate);   //将该心率数据压入到队列
    valueOfBreath2.push_back(valueBreath);         //将该呼吸数据压入到队列

    //说明已经读取了60s的数据，现在可以进行提取有效心率数
    vector<int> maxValueIndex,minValueIndex;   //定义存放心率队列数据的极值vector
    if(valueOfHeartRate2.size()==729&&valueOfBreath2.size()==729){   //心率数据计数到一分钟的729个数据
        int heartRateCount = 0;    //中间过程的临时心率次数
        int heartRatendCount=0;   //最终计算的心率次数
        int breathEndCount=0;      //最终计算的呼吸次数
        //for_each(valueOfHeartRate.cbegin(),valueOfHeartRate.cend(),[&os](const double value){ cout<<value<<" "<<endl; });
        findPeaks(valueOfHeartRate2,maxValueIndex,minValueIndex);  //查找729个心率数据的极值并压入对应的vector中
        //cout<<maxValueIndex.size()<<endl;
        //cout<<minValueIndex.size()<<endl;

        for(size_t i = 0; i < min(maxValueIndex.size(),minValueIndex.size());++i){
            //cout<<valueOfHeartRate1[maxValueIndex[i]]<<"  "<<valueOfHeartRate1[minValueIndex[i]]<<endl;
            if(valueOfHeartRate2[maxValueIndex[i]]-valueOfHeartRate2[minValueIndex[i]] > 60)  //相邻极大值和极小值的阈值差及判断标准
                heartRateCount ++;  //加1
        }
        //for_each(maxValueIndex.cbegin(),maxValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        //for_each(minValueIndex.cbegin(),minValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        heartRatendCount = (heartRateCount/2-20);   //原带算法未知，此值根据实际情况调整
        breathEndCount = findPulse(valueOfBreath2);  //计算呼吸数据的有效数量
        //cout<<"Com1 : "<<endCount<<endl;
        //cout<<breathEndCount<<endl;
        valueOfHeartRate2.erase(valueOfHeartRate2.begin(),valueOfHeartRate2.begin()+10);  //每相隔10个数据进行一次1分钟的有效心率数统计(所以要清除掉队列的最前面的10个数据)
        valueOfBreath2.erase(valueOfBreath2.begin(),valueOfBreath2.begin()+10);
        //判断检测到的有效心率数是否符合要求，并更新显示标签
        if(heartRatendCount < xinlv2 &&breathEndCount < huxi2 ){
            HeartRateLabel2->setText(QString("    心率\n\n     无"));  //检测到不合理心率
            BreathingLabel2->setText(QString("    呼吸\n\n     无"));  //检测不到合理呼吸
            BedLabel2->setText(tr("   2号床位\n\n   : 离床"));
            LightLabel2->setStyleSheet("background-color:red"); //底色

            //  下面的程序为往数据库对应该串口的信息表中插入时间、呼吸以及心率数据

            if(isOpenDataBase){
                string   time = getTime();
                string   date = getDate();
                cout << time << endl;
                QSqlQuery query(db); //查询表并输出，测试能否正常操作数据库
                query.exec("select * from 异常信息_info where 雷达2_呼吸 is NULL");   //查询雷达1_呼吸是否为NULL
                if(query.numRowsAffected()>0){
                    query.prepare(" UPDATE top(1) 异常信息_info set 雷达2=2,雷达2_日期=:date,雷达2_时间=:time,雷达2_心率=:xinlv2,雷达2_呼吸=:huxi2,雷达2_是否在床 = 'NO' where 雷达2_呼吸 IS NULL");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv2",xinlv2);
                    query.bindValue(":huxi2",huxi2);
                    query.exec();
                }
                else{
                    query.prepare("insert into 异常信息_info(雷达2,雷达2_日期,雷达2_时间,雷达2_心率,雷达2_呼吸,雷达2_是否在床) VALUES(2,:date,:time,:xinlv2,:huxi2,'NO')");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv2",xinlv2);
                    query.bindValue(":huxi2",huxi2);
                    query.exec();
                }

                /*
                while(query.next())
                {
                    qDebug()<<query.value(0).toInt() <<query.value(1).toString() <<query.value(2).toInt();
                }
                QDialog *mainDialog = new QDialog;          //打开新的对话框
                QTableView *tableView = new QTableView;     //打开新的表
                QHBoxLayout *layout = new QHBoxLayout;      //添加布局
                layout->addWidget(tableView);
                mainDialog->setLayout(layout);
                QSqlQueryModel *model = new QSqlQueryModel;//新数据库模型
                model->setQuery(QObject::tr("select * from yichang2_info"));//确定选择哪个表
                tableView->setModel(model);//适应打开的表
                mainDialog->adjustSize();//适应标的大小*/

            }
        }
        else if(heartRatendCount >xinlv2&&breathEndCount > huxi2){
            HeartRateLabel2->setText(QString("    心率\n\n     ")+QString::number(heartRatendCount,10));  //更新心率显示标签
            BreathingLabel2->setText(QString("    呼吸\n\n     ")+QString::number(breathEndCount,10));  //检测不到合理呼吸
            BedLabel2->setText(tr("   2号床位\n\n   : 在床"));
            LightLabel2->setStyleSheet("background-color:white"); //底色
        }
    }
}





//负责接收一个字节心率和呼吸的数据，并且在相应位置显示
//形参：对应的一个心率和呼吸的数据
void MainWidget::realtimeDataSlotCom3(double valueHeartRate,double valueBreath){

    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;  //key为当前时间
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    plotHeartRate3->graph(0)->addData(key, valueHeartRate);//添加心率数据到曲线1
    plotBreathing3->graph(0)->addData(key,  valueBreath);  //添加呼吸数据到曲线2
    //删除n秒之前的数据。这里的n要和下面设置横坐标宽度的n配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    plotHeartRate3->graph(0)->removeDataBefore(key-20);
    plotBreathing3->graph(0)->removeDataBefore(key-16);
    //自动设定graph(0)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    plotHeartRate3->graph(0)->rescaleValueAxis(true);
    plotBreathing3->graph(0)->rescaleValueAxis(true);

    //这里的n，是指横坐标时间宽度为n秒，如果想要横坐标显示更多的时间
    //就把n调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-n)中的8也要改成60，否则曲线显示不完整。
    plotHeartRate3->xAxis->setRange(key+0.08, 20, Qt::AlignRight);//设定x轴的范围
    plotHeartRate3->replot();

    plotBreathing3->xAxis->setRange(key+0.08, 16, Qt::AlignRight);//设定x轴的范围
    plotBreathing3->replot();

    valueOfHeartRate3.push_back(valueHeartRate);   //将该心率数据压入到队列
    valueOfBreath3.push_back(valueBreath);         //将该呼吸数据压入到队列

    //说明已经读取了60s的数据，现在可以进行提取有效心率数
    vector<int> maxValueIndex,minValueIndex;   //定义存放心率队列数据的极值vector
    if(valueOfHeartRate3.size()==729&&valueOfBreath3.size()==729){   //心率数据计数到一分钟的729个数据
        int heartRateCount = 0;    //中间过程的临时心率次数
        int heartRatendCount=0;   //最终计算的心率次数
        int breathEndCount=0;      //最终计算的呼吸次数
        //for_each(valueOfHeartRate.cbegin(),valueOfHeartRate.cend(),[&os](const double value){ cout<<value<<" "<<endl; });
        findPeaks(valueOfHeartRate3,maxValueIndex,minValueIndex);  //查找729个心率数据的极值并压入对应的vector中
        //cout<<maxValueIndex.size()<<endl;
        //cout<<minValueIndex.size()<<endl;

        for(size_t i = 0; i < min(maxValueIndex.size(),minValueIndex.size());++i){
            //cout<<valueOfHeartRate1[maxValueIndex[i]]<<"  "<<valueOfHeartRate1[minValueIndex[i]]<<endl;
            if(valueOfHeartRate3[maxValueIndex[i]]-valueOfHeartRate3[minValueIndex[i]] > 60)  //相邻极大值和极小值的阈值差及判断标准
                heartRateCount ++;  //加1
        }
        //for_each(maxValueIndex.cbegin(),maxValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        //for_each(minValueIndex.cbegin(),minValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        heartRatendCount = (heartRateCount/2-20);   //原带算法未知，此值根据实际情况调整
        breathEndCount = findPulse(valueOfBreath3);  //计算呼吸数据的有效数量
        //cout<<"Com1 : "<<endCount<<endl;
        //cout<<breathEndCount<<endl;
        valueOfHeartRate3.erase(valueOfHeartRate3.begin(),valueOfHeartRate3.begin()+10);  //每相隔10个数据进行一次1分钟的有效心率数统计(所以要清除掉队列的最前面的10个数据)
        valueOfBreath3.erase(valueOfBreath3.begin(),valueOfBreath3.begin()+10);
        //判断检测到的有效心率数是否符合要求，并更新显示标签
        if(heartRatendCount < xinlv3 &&breathEndCount < huxi3 ){
            HeartRateLabel3->setText(QString("    心率\n\n     无"));  //检测到不合理心率
            BreathingLabel3->setText(QString("    呼吸\n\n     无"));  //检测不到合理呼吸
            BedLabel3->setText(tr("   3号床位\n\n   : 离床"));
            LightLabel3->setStyleSheet("background-color:red"); //底色

            //  下面的程序为往数据库对应该串口的信息表中插入时间、呼吸以及心率数据

            if(isOpenDataBase){
                string   date = getDate();
                string   time = getTime();
                cout << time << endl;
                QSqlQuery query(db); //查询表并输出，测试能否正常操作数据库
                query.exec("select * from 异常信息_info where 雷达3_呼吸 is NULL");   //查询雷达1_呼吸是否为NULL
                if(query.numRowsAffected()>0){

                    query.prepare(" UPDATE top(1) 异常信息_info set 雷达3=3,雷达3_日期=:date,雷达3_时间=:time,雷达3_心率=:xinlv3,雷达3_呼吸=:huxi3,雷达3_是否在床 = 'NO' where 雷达3_呼吸 IS NULL");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv3",xinlv3);
                    query.bindValue(":huxi3",huxi3);
                    query.exec();
                }
                else{
                    query.prepare("insert into 异常信息_info(雷达3,雷达3_日期,雷达3_时间,雷达3_心率,雷达3_呼吸,雷达3_是否在床) VALUES(3,:date,:time,:xinlv3,:huxi3,'NO')");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv3",xinlv3);
                    query.bindValue(":huxi3",huxi3);
                    query.exec();
                }
                /*
                while(query.next())
                {
                    qDebug()<<query.value(0).toInt() <<query.value(1).toString() <<query.value(2).toInt();
                }

                QDialog *mainDialog = new QDialog;          //打开新的对话框
                QTableView *tableView = new QTableView;     //打开新的表
                QHBoxLayout *layout = new QHBoxLayout;      //添加布局
                layout->addWidget(tableView);
                mainDialog->setLayout(layout);
                QSqlQueryModel *model = new QSqlQueryModel;//新数据库模型
                model->setQuery(QObject::tr("select * from yichang2_info"));//确定选择哪个表
                tableView->setModel(model);//适应打开的表
                mainDialog->adjustSize();//适应标的大小
                */
            }

        }
        else if(heartRatendCount > xinlv3 &&breathEndCount > huxi3){
            HeartRateLabel3->setText(QString("    心率\n\n     ")+QString::number(heartRatendCount,10));  //更新心率显示标签
            BreathingLabel3->setText(QString("    呼吸\n\n     ")+QString::number(breathEndCount,10));  //检测不到合理呼吸
            BedLabel3->setText(tr("   3号床位\n\n   : 在床"));
            LightLabel3->setStyleSheet("background-color:white"); //底色
        }
    }
}


//负责接收一个字节心率和呼吸的数据，并且在相应位置显示
//形参：对应的一个心率和呼吸的数据
void MainWidget::realtimeDataSlotCom4(double valueHeartRate,double valueBreath){
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;  //key为当前时间
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    plotHeartRate4->graph(0)->addData(key, valueHeartRate);//添加心率数据到曲线1
    plotBreathing4->graph(0)->addData(key,  valueBreath);  //添加呼吸数据到曲线2
    //删除n秒之前的数据。这里的n要和下面设置横坐标宽度的n配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    plotHeartRate4->graph(0)->removeDataBefore(key-20);
    plotBreathing4->graph(0)->removeDataBefore(key-16);
    //自动设定graph(0)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    plotHeartRate4->graph(0)->rescaleValueAxis(true);
    plotBreathing4->graph(0)->rescaleValueAxis(true);

    //这里的n，是指横坐标时间宽度为n秒，如果想要横坐标显示更多的时间
    //就把n调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-n)中的8也要改成60，否则曲线显示不完整。
    plotHeartRate4->xAxis->setRange(key+0.08, 20, Qt::AlignRight);//设定x轴的范围
    plotHeartRate4->replot();

    plotBreathing4->xAxis->setRange(key+0.08, 16, Qt::AlignRight);//设定x轴的范围
    plotBreathing4->replot();

    valueOfHeartRate4.push_back(valueHeartRate);   //将该心率数据压入到队列
    valueOfBreath4.push_back(valueBreath);         //将该呼吸数据压入到队列

    //说明已经读取了60s的数据，现在可以进行提取有效心率数
    vector<int> maxValueIndex,minValueIndex;   //定义存放心率队列数据的极值vector
    if(valueOfHeartRate4.size()==729&&valueOfBreath4.size()==729){   //心率数据计数到一分钟的729个数据
        int heartRateCount = 0;    //中间过程的临时心率次数
        int heartRatendCount=0;   //最终计算的心率次数
        int breathEndCount=0;      //最终计算的呼吸次数
        //for_each(valueOfHeartRate.cbegin(),valueOfHeartRate.cend(),[&os](const double value){ cout<<value<<" "<<endl; });
        findPeaks(valueOfHeartRate4,maxValueIndex,minValueIndex);  //查找729个心率数据的极值并压入对应的vector中
        //cout<<maxValueIndex.size()<<endl;
        //cout<<minValueIndex.size()<<endl;

        for(size_t i = 0; i < min(maxValueIndex.size(),minValueIndex.size());++i){
            //cout<<valueOfHeartRate1[maxValueIndex[i]]<<"  "<<valueOfHeartRate1[minValueIndex[i]]<<endl;
            if(valueOfHeartRate4[maxValueIndex[i]]-valueOfHeartRate4[minValueIndex[i]] > 60)  //相邻极大值和极小值的阈值差及判断标准
                heartRateCount ++;  //加1
        }
        //for_each(maxValueIndex.cbegin(),maxValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        //for_each(minValueIndex.cbegin(),minValueIndex.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
        heartRatendCount = (heartRateCount/2-20);   //原带算法未知，此值根据实际情况调整
        breathEndCount = findPulse(valueOfBreath4);  //计算呼吸数据的有效数量
        //cout<<"Com1 : "<<endCount<<endl;
        //cout<<breathEndCount<<endl;
        valueOfHeartRate4.erase(valueOfHeartRate4.begin(),valueOfHeartRate4.begin()+10);  //每相隔10个数据进行一次1分钟的有效心率数统计(所以要清除掉队列的最前面的10个数据)
        valueOfBreath4.erase(valueOfBreath4.begin(),valueOfBreath4.begin()+10);
        //判断检测到的有效心率数是否符合要求，并更新显示标签
        if(heartRatendCount < xinlv4 &&breathEndCount < huxi4 ){
            HeartRateLabel4->setText(QString("    心率\n\n     无"));  //检测到不合理心率
            BreathingLabel4->setText(QString("    呼吸\n\n     无"));  //检测不到合理呼吸
            BedLabel4->setText(tr("   4号床位\n\n   : 离床"));
            LightLabel4->setStyleSheet("background-color:red"); //底色

            //  下面的程序为往数据库对应该串口的信息表中插入时间、呼吸以及心率数据

            if(isOpenDataBase){
                string   date = getDate();
                string   time = getTime();
                cout << time << endl;
                QSqlQuery query(db); //查询表并输出，测试能否正常操作数据库
                query.exec("select * from 异常信息_info where 雷达4_呼吸 is NULL");   //查询雷达1_呼吸是否为NULL
                if(query.numRowsAffected()>0){
                    query.prepare(" UPDATE top(1) 异常信息_info set 雷达4=4,雷达4_日期=:date,雷达4_时间=:time,雷达4_心率=:xinlv4,雷达4_呼吸=:huxi4,雷达4_是否在床 = 'NO' where 雷达4_呼吸 IS NULL");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv4",xinlv4);
                    query.bindValue(":huxi4",huxi4);
                    query.exec();
                }
                else{
                    query.prepare("insert into 异常信息_info(雷达4,雷达4_日期,雷达4_时间,雷达4_心率,雷达4_呼吸,雷达4_是否在床) VALUES(4,:date,:time,:xinlv4,:huxi4,'NO')");
                    query.bindValue(":date",date.c_str());
                    query.bindValue(":time",time.c_str());
                    query.bindValue(":xinlv4",xinlv4);
                    query.bindValue(":huxi4",huxi4);
                    query.exec();
                }
                /*
                while(query.next())
                {
                    qDebug()<<query.value(0).toInt() <<query.value(1).toString() <<query.value(2).toInt();
                }

                QDialog *mainDialog = new QDialog;          //打开新的对话框
                QTableView *tableView = new QTableView;     //打开新的表
                QHBoxLayout *layout = new QHBoxLayout;      //添加布局
                layout->addWidget(tableView);
                mainDialog->setLayout(layout);
                QSqlQueryModel *model = new QSqlQueryModel;//新数据库模型
                model->setQuery(QObject::tr("select * from yichang2_info"));//确定选择哪个表
                tableView->setModel(model);//适应打开的表
                mainDialog->adjustSize();//适应标的大小
                */
            }
        }
        else if(heartRatendCount > xinlv4 &&breathEndCount > huxi4 ){
            HeartRateLabel4->setText(QString("    心率\n\n     ")+QString::number(heartRatendCount,10));  //更新心率显示标签
            BreathingLabel4->setText(QString("    呼吸\n\n     ")+QString::number(breathEndCount,10));  //检测不到合理呼吸
            BedLabel4->setText(tr("   4号床位\n\n   : 在床"));
            LightLabel4->setStyleSheet("background-color:white"); //底色
        }
    }
}

//触发“清除数据”按钮后的响应函数：清除串口1显示区数据
void MainWidget::ClearRecvCom1()
{

    plotHeartRate1->graph(0)->clearData();  //清除心率图表数据
    plotBreathing1->graph(0)->clearData();  //清除呼吸图表数据
    BedLabel1->setText(tr("   1号床位\n\n   : 离床"));
    HeartRateLabel1->setText(tr("    心率\n\n     无"));
    BreathingLabel1->setText(tr("    呼吸\n\n     无"));
    valueOfHeartRate1.erase(valueOfHeartRate1.begin(),valueOfHeartRate1.end());  //之前的数据清零，重新开始采集数据并计算
    valueOfBreath1.erase(valueOfBreath1.begin(),valueOfBreath1.end());   //之前的呼吸数据清零
}

//触发“清除数据”按钮后的响应函数：清除串口2显示区数据
void MainWidget::ClearRecvCom2()
{

    plotHeartRate2->graph(0)->clearData();
    plotBreathing2->graph(0)->clearData();
    BedLabel2->setText(tr("   2号床位\n\n   : 离床"));
    HeartRateLabel2->setText(tr("    心率\n\n     无"));
    BreathingLabel2->setText(tr("    呼吸\n\n     无"));
    valueOfHeartRate2.erase(valueOfHeartRate2.begin(),valueOfHeartRate2.end());  //之前的数据清零，重新开始采集数据并计算
    valueOfBreath2.erase(valueOfBreath2.begin(),valueOfBreath2.end());   //之前的呼吸数据清零
}
//触发“清除数据”按钮后的响应函数：清除串口3显示区数据
void MainWidget::ClearRecvCom3()
{

    plotHeartRate3->graph(0)->clearData();
    plotBreathing3->graph(0)->clearData();
    BedLabel3->setText(tr("   3号床位\n\n   : 离床"));
    HeartRateLabel3->setText(tr("    心率\n\n     无"));
    BreathingLabel3->setText(tr("    呼吸\n\n     无"));
    valueOfHeartRate3.erase(valueOfHeartRate3.begin(),valueOfHeartRate3.end());  //之前的数据清零，重新开始采集数据并计算
    valueOfBreath3.erase(valueOfBreath3.begin(),valueOfBreath3.end());   //之前的呼吸数据清零
}

//触发“清除数据”按钮后的响应函数：清除串口4显示区数据
void MainWidget::ClearRecvCom4()
{

    plotHeartRate4->graph(0)->clearData();
    plotBreathing4->graph(0)->clearData();
    BedLabel4->setText(tr("   4号床位\n\n   : 离床"));
    HeartRateLabel4->setText(tr("    心率\n\n     无"));
    BreathingLabel4->setText(tr("    呼吸\n\n     无"));
    valueOfHeartRate4.erase(valueOfHeartRate4.begin(),valueOfHeartRate4.end());  //之前的数据清零，重新开始采集数据并计算
    valueOfBreath4.erase(valueOfBreath4.begin(),valueOfBreath4.end());   //之前的呼吸数据清零
}

//查找心率数据队列的极大值和极小值以及他们的坐标
void MainWidget::findPeaks(const deque<double> Peaks, vector<int> &maxValueIndex, vector<int> &minValueIndex){
    vector<int> sign;   //记录数据大小之间关系的标志
    /* 相邻值做差： 小于0，赋-1,大于0，赋1,等于0，赋0 */
    for(size_t i = 1;i < Peaks.size();++i){
        int diff = Peaks[i]- Peaks[i-1];
        if(diff>0)
            sign.push_back(1);
        else if(diff<0)
            sign.push_back(-1);
        else
            sign.push_back(0);
    }
    //for_each(sign.cbegin(),sign.cend(),[&os](const int value){ cout<<value<<" "<<endl; });
    //cout<<sign.size()<<endl;
    //再对sign相邻位做差
    //保存极大值和极小值的位置
    for(size_t j = 1;j <sign.size();++j){
        int diff = sign[j] - sign[j-1];
        if(diff<0)
        {
           maxValueIndex.push_back(j);
        }
        else if(diff>0)
        {
           minValueIndex.push_back(j);
        }
    }
}

//查找呼吸数据队列的脉冲插值大于阈值的数量
double MainWidget::findPulse(const deque<double> Impulse){
    double endCount = 0;
    for(size_t i =0;i<Impulse.size()-1;i++){



        if(Impulse.at(i)-Impulse.at(i+1)>50)
            endCount++;
    }
    return endCount;
}
