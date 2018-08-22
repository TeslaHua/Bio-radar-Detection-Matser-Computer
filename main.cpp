#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setWindowIcon(QIcon(QString(":/pic/icon/icon.png")));
    MainWindow w;
    w.show();

    return a.exec();
}


/*/int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    OpenDatabase();

    QTableView *tableView = new QTableView;//打开新的表
    QHBoxLayout *layout = new QHBoxLayout;//添加布局
    layout->addWidget(tableView);
    mainDialog->setLayout(layout);
    //QSqlQueryModel *model = new QSqlQueryModel;//新数据库模型
    //model->setQuery(QObject::tr("select * from yichang2_info"));//确定选择哪个表
    //tableView->setModel('数据库连接成功');//适应打开的表
  // if(!OpenDatabase())
   //    return 1;
    w.show();
   return a.exec();
}*/
