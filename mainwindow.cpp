#include "mainwindow.h"
#include "fibheap.h"
#include "ui_mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initial_ui();
    ui->label->installEventFilter(this);
    ui->menuBar->installEventFilter(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(qobject_cast<QLabel*>(obj)==ui->label &&event->type() == QEvent::MouseMove)
    {
        return false;
    }

    return false;

}

void MainWindow::initial_ui()
{
    this->resize( QSize( 2000, 2000 ));
    //---设置当前窗体对鼠标追踪，默认为false，false表示不追踪
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->label->setMouseTracking(true);
    ui->label->setScaledContents(true);



    //---显示鼠标移动时的实时位置
    statusLabel = new QLabel();
    statusLabel->setText("Real_time Position:");
    statusLabel->setFixedWidth(300);

    //---显示鼠标位置
    mousePointLabel = new QLabel();
    mousePointLabel->setText("");
    mousePointLabel->setFixedWidth(300);

    //---在状态栏增加控件
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(mousePointLabel);


    ui->statusBar->showMessage("Click File->Open to begin");
}



QPoint MainWindow::toRelativePos(QMouseEvent *e)
{
    QPoint labelPoint = ui->label->pos();
    QPoint relativePos;
    relativePos.setX(e->x()-labelPoint.x());
    relativePos.setY(e->y()-labelPoint.y()-widthUpperEdge);
    return relativePos;
}

bool MainWindow::judgeMouseInPic(QMouseEvent *e)
{
    QPoint relativePos=toRelativePos(e);
    QSize labelSize= ui->label->size();
    if (relativePos.x()>=0 && relativePos.x()<labelSize.width())
        if(relativePos.y()>=0 && relativePos.y()<labelSize.height())
            return true;
    return false;
}

