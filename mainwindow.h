#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QWidget>
#include <QDesktopWidget>
#include <QSignalMapper>
#include <QDockWidget>
#include <QTextEdit>
#include <QScrollArea>
#include <QLabel>
#include <Qlayout>
#include <QSpinBox>
#include <QSlider>

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <qdebug.h>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <QTime>

#include <QMouseEvent>

#include <string>

#include <windows.h>
#include <windef.h>
#include <winuser.h>
#include "imageprocessing.h"
#include "myutil.h"





namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //MainWindow(   QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSignalMapper *mapper;
    QMenu *dockWidgetMenu;
    //QMenu *mainWindowMenu;
    QList<QDockWidget*> allDockWidgets;
    QAction *createDockWidgetAction;
    QMenu *destroyDockWidgetMenu;

    //origin image
    QImage *openImage;
    //editted image
    QImage changedImage;
    //origin image in gray scale
    QImage *openImageGray;

    QLabel *histgramViewer;

    int threshold1,threshold2;
    QSpinBox *spinBox,*spinBox2;
    QSlider *slider,*slider2;


    int up,down,left,right;

    //local o-w o+8*w, mean, variance
    double umin,umax,variancemin,variancemax;


    int maxBluePointX,maxBluePointY;
    int noteL,noteR,noteU,noteD;


    int block_size;
    int block_num;
    int edge_size;


    int mousePressAction;

private:

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    //draw a circle whose center is (x,y), radius =r
    void drawCircle(QImage& src,int x,int y,int r,int color=MYWHITE);

    void updateHistogram();
    void updateImage();

    void setupMenuBar();
    void setupDockWidgets();

private slots:
    void setDockOptions();

    void createDockWidget();
    void destroyDockWidget(QAction *action);


    void changeImageToOrigin();


    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();


    void doCaptureX5Window();
    void doFindNoteArea(bool checked);
    void doEdgeDetection();
    void doTrim();
    void ostu();
    void doFindNoteNum();
    void doMarkNoteBeginEnd();

    void threshold1Changed(int threshold);
    void threshold2Changed(int threshold);

};

#endif // MAINWINDOW_H
