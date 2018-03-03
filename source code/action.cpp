#include "fibheap.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;



void MainWindow::on_actionSave_Contour_triggered()
{
    if(stage==3)
    {
        imwrite("/Users/andy/Desktop/contour.png", backgroundImage);
        ui->statusBar->showMessage("success save the contour picture");
    }
}

void MainWindow::on_actionSave_Mask_triggered()
{
    if(stage==3)
    {
        wantSaveMask=true;
        ui->statusBar->showMessage("Click the center of Mask!");
    }

}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}


void MainWindow::on_actionStart_triggered()
{
    if(stage==3&&Mode==2)
    {
        Mat withContour=HistoryImage.top().clone();

        QImage img = cvMat2QImage(withContour);
        ui->label2->setPixmap(QPixmap::fromImage(img));
        ui->label2->resize(ui->label2->pixmap()->size());

        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->resize(ui->label->pixmap()->size());
        ui->statusBar->showMessage("you can click finish Draw");
    }
    if(stage==1)
    {
        stage=2;
        if(Mode==1)
        {
            QImage img = cvMat2QImage(image);
            ui->label2->setPixmap(QPixmap::fromImage(img));
            ui->label2->resize(ui->label2->pixmap()->size());
        }
        else if(Mode==3||Mode==4)
        {
            showCostGraphAndPixelNode();
        }
        else if(Mode==2)
        {
            Mat withContour=HistoryImage.top().clone();

            QImage img = cvMat2QImage(withContour);
            ui->label2->setPixmap(QPixmap::fromImage(img));
            ui->label2->resize(ui->label2->pixmap()->size());

            ui->label->setPixmap(QPixmap::fromImage(img));
            ui->label->resize(ui->label->pixmap()->size());
            ui->statusBar->showMessage("you can click finish Draw");

        }
        else if(Mode==5)
        {
            ui->statusBar->showMessage("Click the root of tree");
        }

    }
}


void MainWindow::on_actionfinish_Drawing_triggered()
{
    if((Mode==6&&(stage==2)&&startFlag)||(Mode==2))
    {
        stage=3;
        startFlag=false;
        ui->statusBar->showMessage("Finsh draw,Click Save or File->Close");
    }

}
void MainWindow::on_actionUndo_triggered()
{
    if(Mode==6&&(stage==2)&&startFlag)
    {
        HistoryImage.pop();
        Mat tempMat=HistoryImage.top();
        backgroundImage=tempMat.clone();

        seedNNodes.pop();
        if(seedNNodes.empty())
        {
            startFlag=false;
        }
        hasPathTree=false;

        QImage img = cvMat2QImage(backgroundImage);
        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->resize(ui->label->pixmap()->size());


        ui->statusBar->showMessage("Undo success");

    }
}
void MainWindow::on_actionPixel_Node_triggered()
{
    if(stage>0)
    {
        Mode=3; 
        init_ForChangeMode();
    }
}

void MainWindow::on_actionCost_Graph_triggered()
{
    if(stage>0)
    {
        Mode=4;
        init_ForChangeMode();
    }
}

void MainWindow::on_actionPath_Tree_triggered()
{
    if(stage>0)
    {
        Mode=5;
        init_ForChangeMode();
    }
}

void MainWindow::on_actionMin_Path_triggered()
{
    if(stage>0)
    {
        Mode=6;
        init_ForChangeMode();
    }
}

void MainWindow::on_actionImage_with_Contour_triggered()
{
    if(stage>0)
    {
        Mode=2;
        //init_ForChangeMode();
    }
}
void MainWindow::on_actionImage_Only_triggered()
{
    if(stage>0)
    {
        Mode=1;
        init_ForChangeMode();
    }
}
void MainWindow::on_actionOpen_triggered()
{
    loadPic=true;
    isReady=false;
    startFlag=false;

    stage=1;  // load picture done.

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
    image = cv::imread(fileName.toLatin1().data());
//    image = cv::imread("/Users/andy/Desktop/picture/ferry.bmp");
    backgroundImage=image.clone();
    QImage img = cvMat2QImage(backgroundImage);
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());


    init_mainVec();
    initComputeCost();
    ui->statusBar->showMessage("load Pic success! you can use zoom in(out) now");
}

//--鼠标移动事件
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(stage==3)
        return;
    if(hasZoomed)
    {
        backgroundImage=image.clone();
        QImage img = cvMat2QImage(backgroundImage);
        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->resize(ui->label->pixmap()->size());

        init_mainVec();
        initComputeCost();
        while(!HistoryImage.empty())
            HistoryImage.pop();
        //HistoryImage.push(backgroundImage);
        hasZoomed=false;
        startFlag=false;

    }
    QPoint relativePos=toRelativePos(e);
    if(judgeMouseInPic(e)&&!judgeBound(relativePos.y(),relativePos.x()))
    {
        QString str = "("+QString :: number(relativePos.x()) +", "+QString::number(relativePos.y())+")";
        statusLabel->setText("Real_time Position:"+str);
        if(stage==2)
        {
            if(Mode==1||Mode==3||Mode==4||Mode==5||Mode==2)
                return;

            NNode curNNode;
            curNNode.row=relativePos.y();
            curNNode.column=relativePos.x();
            ui->statusBar->showMessage("Click undo or finishDraw");

            if(startFlag==true)
            {
                genPathTree();
                drawPath(curNNode);
            }
        }
    }
    else
    {
        QString str = "out of Picture";
        statusLabel->setText("Real_time Position:"+str);
        if(stage==2)
        {
            QImage img = cvMat2QImage(backgroundImage);
            ui->label->setPixmap(QPixmap::fromImage(img));
            ui->label->resize(ui->label->pixmap()->size());
        }

    }

}


//--鼠标按下事件
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(judgeMouseInPic(e))
    {
        QPoint relativePos=toRelativePos(e);
        QString str = "("+QString :: number(relativePos.x()) +", "+QString::number(relativePos.y())+")";
        mousePointLabel->setText("Click Position:"+str);

        if(stage==3&&wantSaveMask)
        {
            NNode temp;
            temp.row=relativePos.y();
            temp.column=relativePos.x();
            showMask(temp);
            return;
        }
        if(stage==2)
        {
            if(Mode==1 || Mode==3||Mode==4||Mode==2)
                return;
            //new click point
            NNode temp;
            temp.row=relativePos.y();
            temp.column=relativePos.x();
            seedNNodes.push(temp);

            init_ForDrawAgain();
            if(startFlag==false)
            {
                HistoryImage.push(image);
                backgroundImage=image.clone();
                startFlag=true;
                ui->statusBar->showMessage("the first seed point chosen");
            }
            else
            {
                if(Mode==6)
                {
                    ui->statusBar->showMessage("the following seed point chosen");
                    HistoryImage.push(tempImage);

                    backgroundImage=tempImage.clone();

                }

            }

            hasPathTree=false;
            genPathTree();
            if(Mode==5)
                showPathTree();
        }

    }
    else
    {
        QString str = "out of Picture";
        mousePointLabel->setText("Click Position:"+str);
    }

}

void MainWindow::on_actionZoom_in_triggered()
{
    if(stage==1)
    {
        float fx = 1.5;
        float fy = 1.5;
        Mat enlarge;
        cv::resize(image, enlarge, Size(), fx, fy, INTER_CUBIC);
        image=enlarge.clone();
        QImage img = cvMat2QImage(image);
        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->resize(ui->label->pixmap()->size());
        hasZoomed=true;
        ui->statusBar->showMessage("success zoom in!");
        return;
    }
    ui->statusBar->showMessage("cannot zoom in now");

}

void MainWindow::on_actionZoom_out_triggered()
{
    if(stage==1)
    {
        float fx = 0.667;
        float fy = 0.667;
        Mat enlarge;
        cv::resize(image, enlarge, Size(), fx, fy, INTER_CUBIC);
        image=enlarge.clone();
        QImage img = cvMat2QImage(image);
        ui->label->setPixmap(QPixmap::fromImage(img));
        ui->label->resize(ui->label->pixmap()->size());
        hasZoomed=true;
        ui->statusBar->showMessage("success zoom out");
        return ;
    }
    ui->statusBar->showMessage("cannot zoom out now");
}

