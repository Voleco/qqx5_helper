
#include <QtGui>
//#include <QtGui/QApplication>
#include<QTextCodec>
#include "mainwindow.h"



int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QApplication a(argc, argv);
    MainWindow mainWin;

    mainWin.show();

    return a.exec();
}
