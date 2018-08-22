#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QGridLayout>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QSerialPortInfo>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QThread>
#include <QDateTime>
#include <QByteArray>
#include <QSettings>
#include <QFormLayout>
#include <QGroupBox>
#include "qcustomplot.h"
#include "serialcontroller.h"
#include <deque>
#include <vector>

#include <QDialog>
#include <QSqlDatabase>					//头文件
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QString>
#include <QTextCodec>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSql>
#include <QTableView>
#include <QSqlQueryModel>
#include <QHBoxLayout>
#include <QtGui>
#include <QSqlQuery>
#include <string>
#include <time.h>


#include<iostream>
using namespace std;

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  MainWidget(QWidget *parent = 0);
  //每一个串口号的检测
  void CheckSerialsCom1();
  void CheckSerialsCom2();
  void CheckSerialsCom3();
  void CheckSerialsCom4();
  ~MainWidget();

signals:
  //每一个串口的询问打开和关闭
  void requestOpenCom1(QString portName);
  void requestCloseCom1();

  void requestOpenCom2(QString portName);
  void requestCloseCom2();

  void requestOpenCom3(QString portName);
  void requestCloseCom3();

  void requestOpenCom4(QString portName);
  void requestCloseCom4();

  //设置串口的公有波特率等信息
  void setBaudRate(QString baudrate);
  void setStopBits(QString stopbits);
  void setDataBits(QString databits);
  void setParity(QString parity);
  void sendStatus(QString status);     //更新statusbar
  void sendDateTime(QString datetime); //更新stastatusbar中的时间
  void changeRTS(bool set);
  void changeDTR(bool set);


public slots:
  void serialOpenedCom1();           //串口1打开成功
  void serialNotOpenedCom1();        //串口1打开失败
  void serialClosedCom1();           //串口1关闭
  void OpenSerialCom1();             //打开串口1
  void CloseSerialCom1();            //关闭串口1

  void serialOpenedCom2();           //串口2打开成功
  void serialNotOpenedCom2();        //串口2打开失败
  void serialClosedCom2();           //串口2关闭
  void OpenSerialCom2();             //打开串口2
  void CloseSerialCom2();            //关闭串口2

  void serialOpenedCom3();           //串口3打开成功
  void serialNotOpenedCom3();        //串口3打开失败
  void serialClosedCom3();           //串口3关闭
  void OpenSerialCom3();             //打开串口3
  void CloseSerialCom3();            //关闭串口3

  void serialOpenedCom4();           //串口4打开成功
  void serialNotOpenedCom4();        //串口4打开失败
  void serialClosedCom4();           //串口4关闭
  void OpenSerialCom4();             //打开串口4
  void CloseSerialCom4();            //关闭串口4

  void ClearRecvCom1();              //清除串口1的数据
  void ClearRecvCom2();              //清除串口2的数据
  void ClearRecvCom3();              //清除串口3的数据
  void ClearRecvCom4();              //清除串口4的数据

  void getRecvCom1(QByteArray recv); //串口1接受数据
  void getRecvCom2(QByteArray recv); //串口2接受数据
  void getRecvCom3(QByteArray recv); //串口3接受数据
  void getRecvCom4(QByteArray recv); //串口4接受数据

  void RTSControl(int state);
  void DTRControl(int state);

  //接收雷达数据以及画图显示
  void realtimeDataSlotCom1(double valueHeartRate,double valueBreath);  //用来绘制心率和呼吸数据波形
  void realtimeDataSlotCom2(double valueHeartRate,double valueBreath);  //用来绘制心率和呼吸数据波形
  void realtimeDataSlotCom3(double valueHeartRate,double valueBreath);  //用来绘制心率和呼吸数据波形
  void realtimeDataSlotCom4(double valueHeartRate,double valueBreath);  //用来绘制心率和呼吸数据波形

  //查找队列中的极小值和极大值
  void findPeaks(const deque<double> Peaks, vector<int> &maxValueIndex, vector<int> &miniValueIndex);   //查找极大值和极小值以及他们的坐标
  double findPulse(const deque<double> Impulse);
private:
  //串口和波特率等信息的复选框
  QComboBox *COMBox1,*COMBox2,*COMBox3,*COMBox4,
            *BaudrateBox, *StopbitsBox, *DatabitsBox, *ParityBox;

  QStringList BaudrateList, StopbitsList, DatabitsList, ParityList;
  QStringList COMList1, PortNameList1, DescList1,
              COMList2, PortNameList2, DescList2,
              COMList3, PortNameList3, DescList3,
              COMList4, PortNameList4, DescList4;

  QLabel *COMLabel1,*COMLabel2,*COMLabel3,*COMLabel4,
         *BaudrateLabel, *StopbitsLabel, *DatabitsLabel, *ParityLabel;

  QPushButton *OpenButton1,*OpenButton2,*OpenButton3,*OpenButton4,
              *ClearRecvButton1,*ClearRecvButton2,*ClearRecvButton3,*ClearRecvButton4;

  QGridLayout *centralLayout;  //主容器
  QVBoxLayout *cvlayout;       //右侧容器
  QFormLayout *leftLlayout;    //左侧容器

  //以下的都是关于装载每个床位信息和心率、呼吸波形的容器
  QGridLayout *BedLayout1,*BedLayout2,*BedLayout3,*BedLayout4,*BedLayout5,*BedLayout6,
  *BedLayout7,*BedLayout8,*BedLayout9,*BedLayout10;

  QGroupBox *paramGroup;
  QCheckBox *RTSBox, *DTRBox;
  //定时器变量
  QTimer *CheckTimer1, *SendTimer1,*CheckTimer2, *SendTimer2,*CheckTimer3, *SendTimer3,*CheckTimer4, *SendTimer4;
  //分配4个串口类的对象
  SerialController *serialControllerCom1,*serialControllerCom2,*serialControllerCom3,*serialControllerCom4;
  //为每个串口分配一个线程
  QThread SerialThrCom1,SerialThrCom2,SerialThrCom3,SerialThrCom4;
  QSettings settings;

  //以下的都是关于床位信息的标签
  QLabel *BedLabel1,*LightLabel1,*HeartRateLabel1,*BreathingLabel1;
  QLabel *BedLabel2,*LightLabel2,*HeartRateLabel2,*BreathingLabel2;
  QLabel *BedLabel3,*LightLabel3,*HeartRateLabel3,*BreathingLabel3;
  QLabel *BedLabel4,*LightLabel4,*HeartRateLabel4,*BreathingLabel4;
  QLabel *BedLabel5,*LightLabel5,*HeartRateLabel5,*BreathingLabel5;
  QLabel *BedLabel6,*LightLabel6,*HeartRateLabel6,*BreathingLabel6;
  QLabel *BedLabel7,*LightLabel7,*HeartRateLabel7,*BreathingLabel7;
  QLabel *BedLabel8,*LightLabel8,*HeartRateLabel8,*BreathingLabel8;
  QLabel *BedLabel9,*LightLabel9,*HeartRateLabel9,*BreathingLabel9;
  QLabel *BedLabel10,*LightLabel10,*HeartRateLabel10,*BreathingLabel10;

  //以下的都是关于绘制每个床位心率、呼吸波形的容器
  QCustomPlot* plotHeartRate1,*plotBreathing1;
  QCustomPlot* plotHeartRate2,*plotBreathing2;
  QCustomPlot* plotHeartRate3,*plotBreathing3;
  QCustomPlot* plotHeartRate4,*plotBreathing4;
  QCustomPlot* plotHeartRate5,*plotBreathing5;
  QCustomPlot* plotHeartRate6,*plotBreathing6;
  QCustomPlot* plotHeartRate7,*plotBreathing7;
  QCustomPlot* plotHeartRate8,*plotBreathing8;
  QCustomPlot* plotHeartRate9,*plotBreathing9;
  QCustomPlot* plotHeartRate10,*plotBreathing10;

  //每个串口是否打开的标志位
  bool isOpened1 = false;
  bool isOpened2 = false;
  bool isOpened3 = false;
  bool isOpened4 = false;

  bool isOpenDataBase = false;   //判断是否成功打开数据库

  QSqlDatabase db;     //定义一个数据库对象


  QString HexStringToString(QString hexstr); //解码16进制字符串

  void ACtionAttachToSerialCom1(bool set);   //控制串口1的相关控件操作
  void ACtionAttachToSerialCom2(bool set);   //控制串口2的相关控件操作
  void ACtionAttachToSerialCom3(bool set);   //控制串口3的相关控件操作
  void ACtionAttachToSerialCom4(bool set);   //控制串口4的相关控件操作

  bool OpenDatabase();   //确认是否打开串口
  string getTime();      //获取当前的时间
  string getDate();

};

#endif // MAINWIDGET_H
