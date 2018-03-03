#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stack>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<QString>
#include<QFileDialog>
#include<QLabel>
#include<QGraphicsScene>
#include<QGraphicsView>
#include <QStatusBar>
#include <QMouseEvent>
#include <QTextCodec>
#include <QDebug>
#include<qdebug.h>
#include <QAction>
#include <QPushButton>
#include <QCursor>
#include <QKeySequence>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QObject>
#include <queue>
#include <unistd.h>
//#include <syswait.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>
#include "fibheap.h"

#define widthUpperEdge 14
#define PointRad 2

#define INITIAL 1
#define ACTIVE 2
#define EXPANDED 3
#define MASKED 4
#define MASKING 5
#define INF 100000
#define debugMode_PixelRatio 2
using namespace cv;
class NNode : public FibHeapNode
{

public:
    double linkCost[8];
    double average_linkCost[8];
    int state;
    float totalCost;
    NNode *prevNode;
    int column, row;
    int position;
    int preNode_index_link;

      NNode() : FibHeapNode() { totalCost = 0;};

      virtual void operator =(FibHeapNode& RHS);
      virtual int  operator ==(FibHeapNode& RHS);
      virtual int  operator <(FibHeapNode& RHS);

      virtual void operator =(float NewKeyVal);
      virtual void Print();
      float GetKeyValue() { return totalCost;};
      void SetKeyValue(float inkey) { totalCost = inkey; };
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage cvMat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);

    void initial_ui();// initial the ui
    void mousePressEvent(QMouseEvent *e);		//--鼠标按下事件
    void mouseMoveEvent(QMouseEvent *e);	//--鼠标移动事件

    bool judgeMouseInPic(QMouseEvent *e);
    QPoint toRelativePos(QMouseEvent *e);   //return the mouse' s relative position

    void drawPoint(Mat &Pic,Point CirCenter);
    void initComputeCost(void);
    void showCostGraphAndPixelNode();
    void showPathTree();
    void init_mainVec();
    void drawPath(NNode curNNode);
    void genPathTree();
    bool judgeBound(int row,int column);
    void showMask(NNode center);
    bool judgeMaskBoundOrBound(NNode* tempNNode);

    void init_ForDrawAgain();
    void init_ForChangeMode();
protected:
    bool eventFilter(QObject *obj, QEvent *e);
private slots:
    void action_showMask();


    void on_actionOpen_triggered();

    void on_actionImage_Only_triggered();

    void on_actionImage_with_Contour_triggered();

    void on_actionPixel_Node_triggered();

    void on_actionCost_Graph_triggered();

    void on_actionPath_Tree_triggered();

    void on_actionMin_Path_triggered();

    void on_actionStart_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionUndo_triggered();

    void on_actionfinish_Drawing_triggered();

    void on_actionSave_Contour_triggered();

    void on_actionSave_Mask_triggered();

    void on_actionClose_triggered();

private:
    int stage=0;  // if loadPic, change to 1; if isReady, change to 2. if is finished,change to 3;
    bool wantSaveMask=false;
    bool finishDraw=false;
    bool hasMaskPic=false;
    bool isReady=false;
    bool hasZoomed=false;
    bool loadPic=false;
    bool startFlag=false; //represent for the start of drawing
    int Mode=6;
    bool hasPathTree=false;
    //1 is Pixel mode, 2 is image with Contour,3 is Pixel Node
    // 4 is cost graph, 5 is path tree, 6 is min Path
    Ui::MainWindow *ui;
    Mat image;
    Mat tempImage;
    Mat backgroundImage;
    Mat pathTreeImage;
    Mat finalMaskImage;
    QLabel *statusLabel;				//---显示鼠标移动时的实时位置
    QLabel *mousePointLabel;		//---显示鼠标click位置


    vector<vector<NNode>>mainVec;
    stack<Mat>HistoryImage;
    stack<NNode>seedNNodes;
    vector<NNode>PathTreeNNodes;
};

#endif // MAINWINDOW_H
