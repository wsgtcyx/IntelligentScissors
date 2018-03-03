//***************************************************************************
// cv_process.CPP
//
// implement the cv algorithms
//***************************************************************************

#include "fibheap.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

//image.at<Vec3b>(row,col)[channel]
//Point(x,y) is using (x,y) as (column,row)


void MainWindow::showMask(NNode center)
{
    Mat maskImage = Mat(image.rows,image.cols,image.type(),Scalar(0,0,0));
    vector<vector<NNode>>tempMainVec;
    tempMainVec.assign(mainVec.begin(),mainVec.end());
    queue<NNode*>maskNodes;

    maskNodes.push(&tempMainVec[center.row][center.column]);
    cout<<"push root"<<endl;
    while(!maskNodes.empty())
    {
        NNode *q=maskNodes.front();
        maskNodes.pop();

        maskImage.at<Vec3b>(q->row,q->column)=image.at<Vec3b>(q->row,q->column);
        tempMainVec[q->row][q->column].state=MASKED;
        //cout<<"Masked "<<q->row<<" "<<q->column<<endl;

        for(int i=-1;i<2;i++)
        {
            for(int j=-1;j<2;j++)
            {
                if(i==0&&j==0)
                    continue;
                NNode *r=&tempMainVec[q->row+i][q->column+j];
                if((!judgeMaskBoundOrBound(r))&&(r->state!=MASKED)&&(r->state!=MASKING))
                {
                    maskNodes.push(r);
                    r->state=MASKING;
                }

            }
        }
    }
    finalMaskImage=maskImage.clone();
    imwrite("/Users/andy/Desktop/mask.png", finalMaskImage);
    ui->statusBar->showMessage("success save the mask picture");

}

bool MainWindow::judgeMaskBoundOrBound(NNode* tempNNode)
{
    if(judgeBound(tempNNode->row,tempNNode->column))
        return true;
    int accumulate=0;
    for(int i=-1;i<2;i++)
    {
        for(int j=-1;j<2;j++)
        {
            if(i==0&&j==0)
                continue;
            NNode *r=&mainVec[tempNNode->row+i][tempNNode->column+j];
            if(backgroundImage.at<Vec3b>(r->row,r->column)==Vec3b(0,0,255))
                accumulate +=1;
            if(backgroundImage.at<Vec3b>(r->row,r->column)==Vec3b(0,255,255))
                accumulate +=1;
        }
    }
    if(accumulate>=6)
        return true;
    else
        return false;
}

void MainWindow::drawPath(NNode curNNode)
{
    Mat PathTreePic=backgroundImage.clone();
    NNode *curNode = &mainVec[curNNode.row][curNNode.column];
    NNode seedNNode=seedNNodes.top();
    circle(PathTreePic,Point(seedNNode.column,seedNNode.row),2*PointRad,Scalar(0,255,255),-1);
    circle(PathTreePic,Point(curNode->column,curNode->row),2*PointRad,Scalar(0,255,255),-1);
    while(!(curNode->row==seedNNode.row&&curNode->column==seedNNode.column))
    {
        if(curNode->prevNode==NULL)
            break;
        drawPoint(PathTreePic,Point(curNode->column,curNode->row));
        curNode = curNode->prevNode;
    }

    QImage imgTemp = cvMat2QImage(PathTreePic);
    ui->label->setPixmap(QPixmap::fromImage(imgTemp));
    tempImage=PathTreePic;
}

void MainWindow::action_showMask()
{
    Mat mask=backgroundImage.clone();

    Point center(image.cols/2,image.rows/2);
    floodFill(mask, center, 255, NULL, cvScalarAll(0), cvScalarAll(0), CV_FLOODFILL_FIXED_RANGE);
    Mat img2;
    backgroundImage.copyTo(img2,mask);
    imshow("mask",mask);
    imshow("img2",img2);
    waitKey();

}

void MainWindow::drawPoint(Mat &Pic,Point CirCenter)
{
    circle(Pic,CirCenter,PointRad,Scalar(0,0,255),-1);
}

void MainWindow::showPathTree()
{
    if((Mode==5)&&stage==2&&startFlag)
    {
        Mat pathTreeImage = Mat(3*image.rows,3*image.cols,image.type(),Scalar(0,0,0));
        int gap=PathTreeNNodes.size()/255;

        for(int i=0;i<(int)PathTreeNNodes.size();i++)
        {
            Vec3b pixel;
            pixel[0]=0;
            pixel[1]=i/gap;
            pixel[2]=i/gap;
            pathTreeImage.at<Vec3b>(3*PathTreeNNodes[i].row+1,3*PathTreeNNodes[i].column+1)=pixel;

            int direction=1;
            NNode *preNode;

            for(int k=0;k<2;k++)
            {
                if(k==0)
                    preNode=&PathTreeNNodes[i];
                else
                {
                    preNode=PathTreeNNodes[i].prevNode;
                    direction = direction*(-1);
                }
                if (preNode==NULL)
                    continue;

                switch(PathTreeNNodes[i].preNode_index_link)
                {
                case 1:{
                    pathTreeImage.at<Vec3b>(3*preNode->row-1*direction,3*preNode->column+1*direction)=pixel;
                    break;
                }
                case 2:{
                    pathTreeImage.at<Vec3b>(3*preNode->row-1*direction,3*preNode->column)=pixel;
                    break;
                }
                case 3:{
                    pathTreeImage.at<Vec3b>(3*preNode->row-1*direction,3*preNode->column-1*direction)=pixel;
                    break;
                }
                case 4:{
                    pathTreeImage.at<Vec3b>(3*preNode->row,3*preNode->column-1*direction)=pixel;
                    break;
                }
                case 5:{
                    pathTreeImage.at<Vec3b>(3*preNode->row+1*direction,3*preNode->column-1*direction)=pixel;
                    break;
                }
                case 6:{
                    pathTreeImage.at<Vec3b>(3*preNode->row+1*direction,3*preNode->column)=pixel;
                    break;
                }
                case 7:{
                    pathTreeImage.at<Vec3b>(3*preNode->row+1*direction,3*preNode->column+1*direction)=pixel;
                    break;
                }
                case 0:{
                    pathTreeImage.at<Vec3b>(3*preNode->row,3*preNode->column+1*direction)=pixel;
                    break;
                }

                }
            }

            if(i%(PathTreeNNodes.size()/20)==0||i==(int)(PathTreeNNodes.size()-1))
            {
//                QImage imgTemp = cvMat2QImage(pathTreeImage);
//                ui->label2->setPixmap(QPixmap::fromImage(imgTemp));
//                ui->label2->resize(ui->label2->pixmap()->size());
                namedWindow("path tree",CV_WINDOW_AUTOSIZE);
                imshow("path tree",pathTreeImage);
                waitKey(200);
            }
        }

    }


}
void MainWindow::showCostGraphAndPixelNode()
{
    Mat CostGraph=Mat(3*image.rows,3*image.cols,image.type(),Scalar(0,0,0));

    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            for(int k=0;k<3;k++)
            {
                CostGraph.at<Vec3b>(3*i+1,3*j+1)[k]=image.at<Vec3b>(i,j)[k];

                if(Mode==4)
                {
                    CostGraph.at<Vec3b>(3*i,3*j)[k]=mainVec[i][j].linkCost[3];
                    CostGraph.at<Vec3b>(3*i,3*j+1)[k]=mainVec[i][j].linkCost[2];
                    CostGraph.at<Vec3b>(3*i,3*j+2)[k]=mainVec[i][j].linkCost[1];
                    CostGraph.at<Vec3b>(3*i+1,3*j)[k]=mainVec[i][j].linkCost[4];
                    CostGraph.at<Vec3b>(3*i+1,3*j+2)[k]=mainVec[i][j].linkCost[0];
                    CostGraph.at<Vec3b>(3*i+2,3*j)[k]=mainVec[i][j].linkCost[5];
                    CostGraph.at<Vec3b>(3*i+2,3*j+1)[k]=mainVec[i][j].linkCost[6];
                    CostGraph.at<Vec3b>(3*i+2,3*j+2)[k]=mainVec[i][j].linkCost[7];
                }
            }
        }
    }

    QImage img = cvMat2QImage(CostGraph);
    ui->label2->setPixmap(QPixmap::fromImage(img));
    ui->label2->resize(ui->label2->pixmap()->size());


}
bool MainWindow::judgeBound(int row,int column)
{
    if(row == 0 || column == 0 || row == image.rows - 1 || column ==image.cols - 1)
        return true;
    return false;
}
QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else// if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }

}

Mat MainWindow::QImage2cvMat(QImage image)
{
    cv::Mat mat;

    switch(image.format())
    {
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    default:
        break;
    }
    return mat;
}
