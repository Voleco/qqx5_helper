#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(  QWidget *parent ) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setObjectName("MainWindow");
    this->setWindowTitle("personal assignment 2");
    threshold1=threshold2=0;

    block_size = 48;
    block_num = 6;
    edge_size = 8;
//    imageViewer = new QLabel();
//    imageViewer->setBackgroundRole(QPalette::Base);
//    imageViewer->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
//    imageViewer->setScaledContents(true);


//    mainImageArea = new QScrollArea();
//    mainImageArea->setMinimumWidth(300);
//    mainImageArea->setBackgroundRole(QPalette::Dark);
//    mainImageArea->setWidget(imageViewer);

//    setCentralWidget(mainImageArea);


    openImage = new QImage();
    changedImage = QImage();
//    openImageGray = new QImage();

    //setupToolBar();

    mousePressAction = 0;

    setupMenuBar();
    setupDockWidgets();

    statusBar()->showMessage(tr("Status Bar"));
    this->resize(900,450);

    timerJZ = new QTimer(this);
    connect(timerJZ, SIGNAL(timeout()), this, SLOT(pressAKey()));

    timerCT = new QTimer(this);
    connect(timerCT, SIGNAL(timeout()), this, SLOT(doHelperCT1()));

    raise_process_priority();
    context = interception_create_context();


}

MainWindow::~MainWindow()
{
    interception_destroy_context(context);
    delete ui;
}

void MainWindow::setupMenuBar()
{

    QMenu *menu = menuBar()->addMenu(tr("&File"));

    QAction *action = menu->addAction(tr("new..."));
    connect(action, SIGNAL(triggered()), this, SLOT(newFile()));
    action->setDisabled(true);

    action = menu->addAction(tr("open..."));
    connect(action, SIGNAL(triggered()), this, SLOT(openFile()));

    action = menu->addAction(tr("save..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
    action->setDisabled(true);

    action = menu->addAction(tr("save as..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    action = menu->addAction(tr("load std notes image"));
    connect(action, SIGNAL(triggered()), this, SLOT(loadStandardNoteImages()));
\
    action = menu->addAction(tr("get keyboard"));
    connect(action, SIGNAL(triggered()), this, SLOT(getKeyBoard()));

    menu->addSeparator();

    menu->addAction(tr("&exit"), this, SLOT(close()));

    //menu for X5
    QMenu *x5Menu = menuBar()->addMenu(tr("for x5"));

    action = x5Menu->addAction(tr("capture x5 window "));
    connect(action, SIGNAL(triggered()), this, SLOT(doCaptureX5Window()));

    action = x5Menu->addAction(tr("find note area "));
    connect(action, SIGNAL(toggled(bool)), this, SLOT(doFindNoteArea(bool)));
    action->setCheckable(true);

    action = x5Menu->addAction(tr("edge detetion"));
    connect(action,SIGNAL(triggered()),this,SLOT(doEdgeDetection()));

    action = x5Menu->addAction(tr("trim"));
    connect(action,SIGNAL(triggered()),this,SLOT(doTrim()));

    action = x5Menu->addAction(tr("ostu "));
    connect(action, SIGNAL(triggered()), this, SLOT(ostu()));

    action = x5Menu->addAction(tr("find note num"));
    connect(action, SIGNAL(triggered()), this, SLOT(doFindNoteNum()));

    action = x5Menu->addAction(tr("mark notes begin end"));
    connect(action, SIGNAL(triggered()), this, SLOT(doMarkNoteBeginEnd()));

    action = x5Menu->addAction(tr("decode the note sequence"));
    connect(action, SIGNAL(triggered()), this, SLOT(doDecodeNotes()));


    x5Menu->addSeparator();
    action = x5Menu->addAction(tr("test sending key event"));
    connect(action, SIGNAL(triggered()), this, SLOT(doTestSendKeyEvent()));

    action = x5Menu->addAction(tr("test sending key event"));
    connect(action, SIGNAL(triggered()), this, SLOT(doStopSendKeyEvent()));
    //menu for binary image opertions
    QMenu *ChangeImageMenu = menuBar()->addMenu(tr(" Change image "));

    action = ChangeImageMenu->addAction(tr("to origin"));
    connect(action, SIGNAL(triggered()), this, SLOT(changeImageToOrigin()));

    //
    QMenu *x5HelperMenu = menuBar()->addMenu(tr(" x5 helper "));

    action = x5HelperMenu->addAction(tr("start help CT1"));
    connect(action, SIGNAL(triggered()), this, SLOT(doStartHelperCT1()));

    action = x5HelperMenu->addAction(tr("stop help CT1"));
    connect(action, SIGNAL(triggered()), this, SLOT(doStopHelperCT1()));

    x5HelperMenu->addSeparator();

    action = x5HelperMenu->addAction(tr("one key fill"));
    connect(action, SIGNAL(triggered()), this, SLOT(doOneKeyFill()));


    dockWidgetMenu = menuBar()->addMenu(tr("&Views"));
}

void MainWindow::setDockOptions()
{
//    DockOptions opts;
//    QList<QAction*> actions = mainWindowMenu->actions();

//    if (actions.at(0)->isChecked())
//        opts |= AnimatedDocks;
//    if (actions.at(1)->isChecked())
//        opts |= AllowNestedDocks;
//    if (actions.at(2)->isChecked())
//        opts |= AllowTabbedDocks;
//    if (actions.at(3)->isChecked())
//        opts |= ForceTabbedDocks;
//    if (actions.at(4)->isChecked())
//        opts |= VerticalTabs;

//    QMainWindow::setDockOptions(opts);
}

void MainWindow::setupDockWidgets()
{
    //qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    mapper = new QSignalMapper(this);
    //connect(mapper, SIGNAL(mapped(int)), this, SLOT(setCorner(int)));

    createDockWidgetAction = new QAction(tr("Add View..."), this);
    connect(createDockWidgetAction, SIGNAL(triggered()), this, SLOT(createDockWidget()));
    destroyDockWidgetMenu = new QMenu(tr("Close View"), this);
    destroyDockWidgetMenu->setEnabled(true);
    connect(destroyDockWidgetMenu, SIGNAL(triggered(QAction*)), this, SLOT(destroyDockWidget(QAction*)));

    dockWidgetMenu->addSeparator();
    dockWidgetMenu->addAction(createDockWidgetAction);
    dockWidgetMenu->addMenu(destroyDockWidgetMenu);

    static const struct Set {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
    } sets [] = {
        //{ "Blue Histogram", 0, Qt::RightDockWidgetArea },
        //{ "Red Histogram", 0, Qt::RightDockWidgetArea },
        //{ "Green Histogram", 0, Qt::RightDockWidgetArea },
        { "HistoGram", 0, Qt::RightDockWidgetArea },
        { "Threshold", 0, Qt::RightDockWidgetArea }
    };
    const int setCount = sizeof(sets) / sizeof(Set);


    for (int i = 0; i < setCount; ++i) {
        QDockWidget *qdw = new QDockWidget(tr(sets[i].name));

        addDockWidget(sets[i].area, qdw);

        destroyDockWidgetMenu->addAction(tr(sets[i].name));
        allDockWidgets.append(qdw);
    }

    allDockWidgets.first()->setMinimumWidth(256);
    allDockWidgets.first()->setMinimumHeight(300);

    histgramViewer = new QLabel(allDockWidgets.first());

    allDockWidgets[setCount-1]->setMinimumHeight(128);
    //allDockWidgets[setCount-1]->resize();

    //QHBoxLayout* hlo1 = new QHBoxLayout(allDockWidgets[3]);
    //allDockWidgets[4]->setLayout(hlo1);

    spinBox = new QSpinBox(allDockWidgets[setCount-1]);
    slider = new QSlider(Qt::Horizontal,allDockWidgets[setCount-1]);
    spinBox->setGeometry(200,20,50,25);
    spinBox->setRange(0,255);
    slider->setGeometry(0,20,200,20);
    slider->setRange(0,255);
    connect(spinBox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),spinBox,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(threshold1Changed(int)));

    spinBox2 = new QSpinBox(allDockWidgets[setCount-1]);
    slider2 = new QSlider(Qt::Horizontal,allDockWidgets[setCount-1]);
    spinBox2->setGeometry(200,50,50,25);
    spinBox2->setRange(0,255);
    slider2->setGeometry(0,50,200,20);
    slider2->setRange(0,255);
    connect(spinBox2,SIGNAL(valueChanged(int)),slider2,SLOT(setValue(int)));
    connect(slider2,SIGNAL(valueChanged(int)),spinBox2,SLOT(setValue(int)));
    connect(slider2,SIGNAL(valueChanged(int)),this,SLOT(threshold2Changed(int)));
    //connect(spinBox,SIGNAL(valueChanged(int)),this,SLOT(thresholdChanged(int)));
    //hlo1->addWidget(spinBox);
    //hlo1->addWidget(slider);
    //allDockWidgets[4]->show();
    //QVBoxLayout* vlo = new QVBoxLayout(allDockWidgets[4]);
    //vlo->addLayout(hlo1);

    //QPushButton *pb = new QPushButton(allDockWidgets[4]);
    //pb->setGeometry(300,70,50,30);


   // destroyDockWidgetMenu->addAction(new QAction(dialog.objectName(), this));
}

class CreateDockWidgetDialog : public QDialog
{
public:
    CreateDockWidgetDialog(QWidget *parent = 0);

    QString objectName() const;
    Qt::DockWidgetArea location() const;

private:
    QLineEdit *m_objectName;
    QComboBox *m_location;
};

CreateDockWidgetDialog::CreateDockWidgetDialog(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(new QLabel(tr("Object name:")), 0, 0);
    m_objectName = new QLineEdit;
    layout->addWidget(m_objectName, 0, 1);

    layout->addWidget(new QLabel(tr("Location:")), 1, 0);
    m_location = new QComboBox;
    m_location->setEditable(false);
    m_location->addItem(tr("Top"));
    m_location->addItem(tr("Left"));
    m_location->addItem(tr("Right"));
    m_location->addItem(tr("Bottom"));
    m_location->addItem(tr("Restore"));
    layout->addWidget(m_location, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    layout->addLayout(buttonLayout, 2, 0, 1, 2);
    buttonLayout->addStretch();

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonLayout->addWidget(cancelButton);
    QPushButton *okButton = new QPushButton(tr("Ok"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(okButton);

    okButton->setDefault(true);
}

QString CreateDockWidgetDialog::objectName() const
{
    return m_objectName->text();
}

Qt::DockWidgetArea CreateDockWidgetDialog::location() const
{
    switch (m_location->currentIndex()) {
        case 0: return Qt::TopDockWidgetArea;
        case 1: return Qt::LeftDockWidgetArea;
        case 2: return Qt::RightDockWidgetArea;
        case 3: return Qt::BottomDockWidgetArea;
        default:
            break;
    }
    return Qt::NoDockWidgetArea;
}

void MainWindow::createDockWidget()
{
    CreateDockWidgetDialog dialog(this);
    int ret = dialog.exec();
    if (ret == QDialog::Rejected)
        return;

    QDockWidget *dw = new QDockWidget;
    dw->setObjectName(dialog.objectName());
    dw->setWindowTitle(dialog.objectName());
    dw->setWidget(new QTextEdit);

    Qt::DockWidgetArea area = dialog.location();
    switch (area) {
        case Qt::LeftDockWidgetArea:
        case Qt::RightDockWidgetArea:
        case Qt::TopDockWidgetArea:
        case Qt::BottomDockWidgetArea:
            addDockWidget(area, dw);
            break;
        default:
            if (!restoreDockWidget(dw)) {
                QMessageBox::warning(this, QString(), tr("Failed to restore dock widget"));
                delete dw;
                return;
            }
            break;
    }

    allDockWidgets.append(dw);
    destroyDockWidgetMenu->setEnabled(true);
    destroyDockWidgetMenu->addAction(new QAction(dialog.objectName(), this));
}

void MainWindow::destroyDockWidget(QAction *action)
{
    int index = destroyDockWidgetMenu->actions().indexOf(action);
    delete allDockWidgets.takeAt(index);
    destroyDockWidgetMenu->removeAction(action);
    action->deleteLater();

    if (destroyDockWidgetMenu->isEmpty())
        destroyDockWidgetMenu->setEnabled(false);
}

void MainWindow::newFile()
{

}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this,"Choose File",QDir::currentPath(),"Images (*.bmp *.png *.xpm *.jpg)");
    if(!openImage)
        delete openImage;
    openImage = new QImage(file);
    //srcPicture.setData();
    //openImage->save("c:\a.jpg");

    changedImage = QImage(*openImage);


    this->setWindowTitle(file);
    updateImage();
    updateHistogram();


//    allColors.clear();
//    numberOfPixelInAColor.clear();
//    one_x.clear();
//    one_y.clear();

//    validColors.clear();
//    numberOfPixelLT2500.clear();
//    pixel_x.clear();
//    pixel_y.clear();
}

void MainWindow::saveFile()
{

}

void MainWindow::saveFileAs()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save as"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
    changedImage.save(fileName);

}

void MainWindow::threshold1Changed(int threshold)
{
    threshold1 = threshold;
    changedImage = QImage(*openImage);
    for(int j=0;j<changedImage.height();j++)
        for(int i=0;i<changedImage.width();i++)
        {
            if(qGray(changedImage.pixel(i,j))>threshold2)
                changedImage.setPixel(QPoint(i,j),MYBLACK);
            else if(qGray(changedImage.pixel(i,j))<threshold1)
                changedImage.setPixel(QPoint(i,j),MYBLACK);
            else
                changedImage.setPixel(QPoint(i,j),MYWHITE);
        }

    updateImage();
    updateHistogram();
}

void MainWindow::threshold2Changed(int threshold)
{
    threshold2 = threshold;

    changedImage = QImage(*openImage);
    for(int j=0;j<changedImage.height();j++)
        for(int i=0;i<changedImage.width();i++)
        {
            if(qGray(changedImage.pixel(i,j))>threshold2)
                changedImage.setPixel(QPoint(i,j),MYBLACK);
            else if(qGray(changedImage.pixel(i,j))<threshold1)
                changedImage.setPixel(QPoint(i,j),MYBLACK);
            else
                changedImage.setPixel(QPoint(i,j),MYWHITE);
        }

    updateImage();
    updateHistogram();
}

void MainWindow::updateImage()
{

    ui->ImageViewer->setPixmap(QPixmap::fromImage(changedImage));
    ui->ImageViewer->adjustSize();
    ui->ImageViewer->show();


    //paintHistogram();

}

void MainWindow::updateHistogram()
{
    QImage* src= new QImage(changedImage);
             int grayValue;
             int hist[256]={0};
             for(int j=0;j<src->height();j++)
             {
                       for(int i=0;i<src->width();i++)
                       {
                           grayValue = qGray(src->pixel(i,j));
                           hist[grayValue]++;
                       }
             }
             int max=0;
             for(int i=0;i<256;i++)
             {
                 if(hist[i]>max)
                     max=hist[i];
             }
             QImage* dst = new QImage(256,300,QImage::Format_RGB32);
             dst->fill(MYWHITE);
             double factor=(double)280.0/max;

             for(int i=0;i<dst->width();i++)
                 for(int j=dst->height()-1;j>=0;j--)
                 {
                     if(j > dst->height()- factor * hist[i])
                         dst->setPixel(QPoint(i,j),MYBLACK);
                 }

        histgramViewer->setPixmap(QPixmap::fromImage(*dst));
        histgramViewer->adjustSize();
        histgramViewer->show();
}

void MainWindow::changeImageToOrigin()
{
    changedImage = QImage(*openImage);
    updateImage();
    updateHistogram();
}

void MainWindow::doCaptureX5Window()
{

    LPCWSTR pszThreadName = convertCharArrayToLPCWSTR("QQìÅÎè");

    QPixmap originalPixmap = QPixmap::grabWindow((WId)FindWindowW(NULL, pszThreadName));
    QImage tmp = originalPixmap.toImage();
/*
    int actualWidth = tmp.width();
    int actualHeight = tmp.height();
    //remove the black margin
    while(actualWidth>0)
    {
        if(IsBlack(tmp.pixel(actualWidth-1,0)))
            actualWidth--;
        else
            break;
    }
    while(actualHeight>0)
    {
        if(IsBlack(tmp.pixel(0,actualHeight-1)))
            actualHeight--;
        else
            break;
    }
    qDebug()<<"width"<<actualWidth<<"height"<<actualHeight;
*/
    changedImage = tmp.copy(0,0,1024,768);
    updateImage();
    updateHistogram();
}

void MainWindow::doFindNoteArea(bool checked)
{
    if(checked)
        mousePressAction = 1;
    else
        mousePressAction = 0;
}

void MainWindow::doTrim()
{
    //Mark(changedImage,458,498,360,685);
    QImage res = Clip(changedImage,458,498,360,685);
    changedImage = QImage(res);
    if(!openImage)
        delete openImage;
    openImage = new QImage(changedImage);
    updateImage();
    updateHistogram();
}

void MainWindow::drawCircle(QImage& src,int x,int y,int r,int color)
{
    int startx,starty,endx,endy;
    if(x-r<0)
        startx = 0;
    else
        startx = x-r;
    if(y-r<0)
        starty = 0;
    else
        starty = y-r;
    if(x+r >= src.width())
        endx = src.width();
    else
        endx = x+r;
    if(y+r >= src.height())
        endy = src.height();
    else
        endy = y+r;

    for(int j=starty;j<endy;j++)
        for(int i=startx;i<endx;i++)
        {
            if((y-j)*(y-j)+(x-i)*(x-i)<r*r)
                src.setPixel(QPoint(i,j),color);
        }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    switch (mousePressAction) {
    case 0:
            qDebug()<<"x"<<e->x();
            qDebug()<<"y"<<e->y();
        break;
    case 1:
        if(e->x()<changedImage.width()&&e->y()<changedImage.height())
        {
            std::string msg;
            msg += "R: ";
            msg += std::to_string(qRed(changedImage.pixel(e->x(),e->y())));
            msg += " G: ";
            msg += std::to_string(qGreen(changedImage.pixel(e->x(),e->y())));
            msg += " B: ";
            msg += std::to_string(qBlue(changedImage.pixel(e->x(),e->y())));
//            qDebug()<<QString::fromStdString(msg);
            ui->statusBar->showMessage(QString::fromStdString(msg));

        }
        break;
    default:
        break;
    }
//    qDebug()<<"x"<<e->x();
//    qDebug()<<"y"<<e->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    switch (mousePressAction) {
    case 0:
//            qDebug()<<"x"<<e->x();
//            qDebug()<<"y"<<e->y();
        break;
    case 1:
        if(e->x()<changedImage.width()&&e->y()<changedImage.height())
        {
            std::string msg;
            msg += "R: ";
            msg += std::to_string(qRed(changedImage.pixel(e->x(),e->y())));
            msg += " G: ";
            msg += std::to_string(qGreen(changedImage.pixel(e->x(),e->y())));
            msg += " B: ";
            msg += std::to_string(qBlue(changedImage.pixel(e->x(),e->y())));
//            qDebug()<<QString::fromStdString(msg);
            ui->statusBar->showMessage(QString::fromStdString(msg));
        }
        break;
    default:
        break;
    }
//    qDebug()<<"x"<<e->x();
//    qDebug()<<"y"<<e->y();
}

void MainWindow::doEdgeDetection()
{
    MorphologicalEdgeDetectionByWTT(changedImage);
    updateImage();
    updateHistogram();

}

void MainWindow::ostu()
{
    int thr = ostu_tres(changedImage);
    QImageToBinary(changedImage,thr);

    spinBox->setValue(thr);
    spinBox2->setValue(255);
    updateImage();
    updateHistogram();
}


void MainWindow::doFindNoteNum()
{
    std::vector<int> bgs;
    int num = GetNoteNumAndBeginFromBinaryImage(changedImage,bgs);
    qDebug()<<num;
}

void MainWindow::doMarkNoteBeginEnd()
{
    std::vector<QImage> notes;
    MarkNodeBeginEndFromBinaryImage(changedImage,notes);
    QString filename;
    std::string strfileName;
    for(unsigned int i=0;i<notes.size();i++)
    {
        strfileName = "D:/tests/" + std::to_string(i) + ".bmp";
        filename = QString::fromStdString(strfileName);
        notes[i].save(filename);
    }

    updateImage();
    updateHistogram();
}

void MainWindow::loadStandardNoteImages()
{
    QImage stdNoteLeft;
    QImage stdNoteRight;
    QImage stdNoteUp;
    QImage stdNoteDown;
    QString prefix = QDir::currentPath()+"/stdnotes/";
    stdNoteLeft = QImage(prefix+"left.bmp");
    stdNoteRight = QImage(prefix+"right.bmp");
    stdNoteUp = QImage(prefix+"up.bmp");
    stdNoteDown = QImage(prefix+"down.bmp");

    //changedImage = QImage(stdNoteLeft);
    stdNotes.push_back(stdNoteRight);
    stdNotes.push_back(stdNoteUp);
    stdNotes.push_back(stdNoteLeft);
    stdNotes.push_back(stdNoteDown);
}

void MainWindow::doDecodeNotes()
{
    std::vector<NoteType> noteSeq;
    GetNodeSequenceFromBinaryImage(changedImage, stdNotes, noteSeq);
    for(unsigned int i=0;i<noteSeq.size();i++)
        qDebug()<<noteSeq[i];

}

void MainWindow::doTestSendKeyEvent()
{
nextNote = NOTE_RIGHT;
    timerJZ->start(1000);
}

void MainWindow::pressAKey()
{
    //qDebug()<<"pressakey called";
    // Simulate a key press
    switch (nextNote) {
    case NOTE_RIGHT:
      //  qDebug()<<"right";
        keybd_event(VK_RIGHT, 0,  0, 0);
        Sleep(20);
        keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
        break;
    case NOTE_UP:
        //qDebug()<<"up";
        keybd_event(VK_UP, 0,  0, 0);
        Sleep(20);
        keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
        break;
    case NOTE_LEFT:
        //qDebug()<<"left";
        keybd_event(VK_LEFT, 0,  0, 0);
        Sleep(20);
        keybd_event(VK_LEFT, 0,  KEYEVENTF_KEYUP, 0);
        break;
    case NOTE_DOWN:
        //qDebug()<<"down";
        keybd_event(VK_DOWN, 0,  0, 0);
        Sleep(20);
        keybd_event(VK_DOWN, 0,  KEYEVENTF_KEYUP, 0);
        break;
    default:
        break;
    }

}

void MainWindow::doStopSendKeyEvent()
{
    timerJZ->stop();
}


void MainWindow::doStartHelperCT1()
{
    timerCT->start(500);
}

void MainWindow::doStopHelperCT1()
{
    timerCT->stop();
}

void MainWindow::doHelperCT1()
{

    noteSeqBuf.resize(0);
    std::vector<NoteType> noteSeq;

    srand (time(NULL));

    //capture window and take a snapshot
    LPCWSTR pszThreadName = convertCharArrayToLPCWSTR("QQìÅÎè");

    QPixmap originalPixmap = QPixmap::grabWindow((WId)FindWindowW(NULL, pszThreadName));
    QImage snapshot = originalPixmap.toImage().copy(0,0,1024,768);

    //clip the snapshot for note zone
    QImage notezone = Clip(snapshot,458,498,360,685);

    //use ostu algorithm to turn note zone into binary image
    int thr = ostu_tres(notezone);
    QImageToBinary(notezone,thr);

    changedImage = QImage(notezone);
    updateImage();
    updateHistogram();
    //decode the note sequence from the binary image
    GetNodeSequenceFromBinaryImage(notezone, stdNotes, noteSeq);

    if(noteSeq.size()>0 && noteSeq!=noteSeqBuf)
    {
        noteSeqBuf = noteSeq;

        for (unsigned int i = 0; i < noteSeqBuf.size(); i++)
        {
            PressKey(context,device,stroke,noteSeqBuf[i]);
            Sleep(50);
            ReleaseKey(context,device,stroke,noteSeqBuf[i]);
            Sleep(50);
        }
    }



}

void MainWindow::doOneKeyFill()
{
    InterceptionContext context;
    InterceptionDevice device;
    InterceptionKeyStroke stroke;

    raise_process_priority();
    context = interception_create_context();

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

    if (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0)
    {
        if (stroke.code == SCANCODE_TAB && stroke.state == INTERCEPTION_KEY_DOWN)
        {
            for (unsigned int i = 0; i < noteSeqBuf.size(); i++)
            {
                PressKey(context,device,stroke,noteSeqBuf[i]);
                Sleep(50);
                ReleaseKey(context,device,stroke,noteSeqBuf[i]);
                Sleep(50);
            }
        }

        //else if (stroke.code == SCANCODE_ESC)
        //    break;
        else
            interception_send(context, device, (InterceptionStroke *)&stroke, 1);

    }


    interception_destroy_context(context);
}

void MainWindow::getKeyBoard()
{
    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

    if (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0)
        interception_send(context, device, (InterceptionStroke *)&stroke, 1);
}
