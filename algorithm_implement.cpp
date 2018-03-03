//***************************************************************************
// algorithm_implement.CPP
//
// implement the main algorithm to find the best path
//***************************************************************************

#include "fibheap.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
//image.at<Vec3b>(row,col)[channel]


void MainWindow::init_ForChangeMode()
{
    stage=1;
    startFlag=false;

    backgroundImage=image.clone();
    QImage img = cvMat2QImage(backgroundImage);
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->resize(ui->label->pixmap()->size());

    init_mainVec();
    initComputeCost();
    ui->statusBar->showMessage("Click Start");
}
void MainWindow::init_ForDrawAgain()
{
    NNode *temp;
    for(int i=1;i<image.rows-1;i++)
    {
        for(int j=1;j<image.cols-1;j++)
        {
            temp = &mainVec[i][j];
            temp->totalCost=INF;
            temp->state=INITIAL;
            temp->prevNode=NULL;
            temp->preNode_index_link=0;
        }
    }
}
void MainWindow::init_mainVec()
{
    mainVec.clear();
    while (!seedNNodes.empty())
        seedNNodes.pop();
    while(!HistoryImage.empty())
        HistoryImage.pop();

    for(int i=0;i<image.rows;i++)
    {
        vector<NNode>temp_mainVec;
        for(int j=0;j<image.cols;j++)
        {
            NNode tempNode;
            tempNode.row=i;
            tempNode.column=j;
            tempNode.position=i*image.cols+j;
            tempNode.state = INITIAL;
            tempNode.prevNode = NULL;
            tempNode.totalCost = INF;
            if(i == 0 || j == 0 || i == image.rows - 1 || j ==image.cols - 1)
            {
                for(int k = 0 ;k < 9;k++)
                {
                    tempNode.average_linkCost[k]=INF;
                    tempNode.linkCost[k] = INF;
                }

            }
            temp_mainVec.push_back(tempNode);
        }
        mainVec.push_back(temp_mainVec);
    }
}

void MainWindow::initComputeCost(void)
{
    double maxD=-INF*1.0;
    for(int i=1;i<(image.rows-1);i++)
    {
        for(int j=1;j<(image.cols-1);j++)
        {

            double Dlink[8];
            double average_Dlink[8];

            double Dlink_temp;
            for(int k=0;k<3;k++) //k means channels
            {
                //link0
                Dlink_temp=(image.at<Vec3b>(i,j-1)[k]+image.at<Vec3b>(i+1,j-1)[k]-image.at<Vec3b>(i,j+1)[k]-image.at<Vec3b>(i+1,j+1)[k])*1.0/4.0;
                Dlink[0]+=pow(Dlink_temp,2);
                average_Dlink[0]+=abs(Dlink_temp);
                //link2
                Dlink_temp=(image.at<Vec3b>(i-1,j-1)[k]+image.at<Vec3b>(i-1,j)[k]-image.at<Vec3b>(i+1,j-1)[k]-image.at<Vec3b>(i+1,j)[k])*1.0/4.0;
                Dlink[2]+=pow(Dlink_temp,2);
                average_Dlink[2]+=abs(Dlink_temp);
                //link4
                Dlink_temp=(image.at<Vec3b>(i-1,j+1)[k]+image.at<Vec3b>(i,j+1)[k]-image.at<Vec3b>(i-1,j-1)[k]-image.at<Vec3b>(i,j-1)[k])*1.0/4.0;
                Dlink[4]+=pow(Dlink_temp,2);
                average_Dlink[4]+=abs(Dlink_temp);
                //link6
                Dlink_temp=(image.at<Vec3b>(i+1,j)[k]+image.at<Vec3b>(i+1,j+1)[k]-image.at<Vec3b>(i-1,j)[k]-image.at<Vec3b>(i-1,j+1)[k])*1.0/4.0;
                Dlink[6]+=pow(Dlink_temp,2);
                average_Dlink[6]+=abs(Dlink_temp);
                //link1
                Dlink_temp=(image.at<Vec3b>(i+1,j)[k]-image.at<Vec3b>(i,j-1)[k])*1.0/sqrt(2.0);
                Dlink[1] +=pow(Dlink_temp,2);
                average_Dlink[1]+=abs(Dlink_temp);
                //link3
                Dlink_temp=(image.at<Vec3b>(i-1,j)[k]-image.at<Vec3b>(i,j-1)[k])*1.0/sqrt(2.0);
                Dlink[3] +=pow(Dlink_temp,2);
                average_Dlink[3]+=abs(Dlink_temp);
                //link5
                Dlink_temp=(image.at<Vec3b>(i,j+1)[k]-image.at<Vec3b>(i-1,j)[k])*1.0/sqrt(2.0);
                Dlink[5] +=pow(Dlink_temp,2);
                average_Dlink[5]+=abs(Dlink_temp);
                //link7
                Dlink_temp=(image.at<Vec3b>(i+1,j)[k]-image.at<Vec3b>(i,j+1)[k])*1.0/sqrt(2.0);
                Dlink[7] +=pow(Dlink_temp,2);
                average_Dlink[7]+=abs(Dlink_temp);

            }

            for(int linkNum=0;linkNum<8;linkNum++)
            {
                Dlink[linkNum] = sqrt(Dlink[linkNum]*1.0/3.0);
                maxD=max(maxD,Dlink[linkNum]);
                average_Dlink[linkNum] /=3.0;
                mainVec[i][j].linkCost[linkNum]=Dlink[linkNum];
            }

        }
    }
    for(int i=1;i<(image.rows-1);i++)
    {
        for(int j=1;j<(image.cols-1);j++)
        {
            for(int linkNum=0;linkNum<8;linkNum++)
            {
                if (linkNum%2==0)
                    mainVec[i][j].linkCost[linkNum]=(maxD-mainVec[i][j].linkCost[linkNum])*1.0*debugMode_PixelRatio;
                else
                    mainVec[i][j].linkCost[linkNum]=(maxD-mainVec[i][j].linkCost[linkNum])*sqrt(2.0)*debugMode_PixelRatio;
            }
        }
    }
}

void MainWindow::genPathTree()
{
    //begin
    if(hasPathTree)
        return;
    init_ForDrawAgain();

    NNode *q;
    NNode *r;
    NNode *treeRoot;
    int index_link;
    vector<NNode*> neighbors;
    vector<int>indexLinkCost;
    FibHeap *theHeap = new FibHeap;

    PathTreeNNodes.clear();

    NNode seedNNode=seedNNodes.top();
    treeRoot=&mainVec[seedNNode.row][seedNNode.column];
    treeRoot->totalCost=0;
    theHeap->Insert(treeRoot);
    while(theHeap->GetNumNodes()!=0)
    {
        q = (NNode*) theHeap->ExtractMin();
        q->state=EXPANDED;

        PathTreeNNodes.push_back(mainVec[q->row][q->column]);

        //bound point
        if(judgeBound(q->row,q->column))
            continue;
        neighbors.clear();
        indexLinkCost.clear();
        if(1) //find all neighbors
        {
            if(!(q->row==0 && q->column==0)) //left_upper
            {
                neighbors.push_back(&mainVec[(q->row-1)][q->column-1]);
                indexLinkCost.push_back(3);
            }

            if(!(q->row==0)) //center upper
            {
                neighbors.push_back(&mainVec[(q->row-1)][q->column]);
                indexLinkCost.push_back(2);
            }
            if(!(q->row==0 && q->column==(image.cols-1)))//right_upper
            {
                neighbors.push_back(&mainVec[(q->row-1)][+q->column+1]);
                indexLinkCost.push_back(1);
            }
            if(!(q->column==0)) //center left
            {
                neighbors.push_back(&mainVec[(q->row)][q->column-1]);
                indexLinkCost.push_back(4);
            }
            if(!(q->column==(image.cols-1))) //center right
            {
                neighbors.push_back(&mainVec[(q->row)][q->column-1]);
                indexLinkCost.push_back(0);
            }
            if(!(q->row==(image.rows-1) && q->column==0)) //left_down
            {
                neighbors.push_back(&mainVec[(q->row+1)][q->column-1]);
                indexLinkCost.push_back(5);
            }
            if(!(q->row==(image.rows-1))) //center down
            {
                neighbors.push_back(&mainVec[(q->row+1)][q->column]);
                indexLinkCost.push_back(6);
            }
            if(!(q->row==(image.rows-1)&& q->column==(image.cols-1)))//right_down
            {
                neighbors.push_back(&mainVec[(q->row+1)][q->column+1]);
                indexLinkCost.push_back(7);
            }
        }
        for(int r_index=0;r_index<(int)neighbors.size();r_index++)
        {
            r=neighbors[r_index];
            index_link=indexLinkCost[r_index];
            if(r->state!=EXPANDED)
            {
                if(r->state==INITIAL)
                {
                    r->prevNode=q;
                    r->preNode_index_link=index_link;
                    r->totalCost=q->totalCost+mainVec[q->row][q->column].linkCost[index_link];
                    r->state=ACTIVE;
                    theHeap->Insert(r);
                }
                else if(r->state==ACTIVE)
                {
                    if(r->totalCost>(q->totalCost+mainVec[q->row][q->column].linkCost[index_link]))
                    {
                        r->prevNode=q;
                        r->preNode_index_link=index_link;
                        r->totalCost=q->totalCost+mainVec[q->row][q->column].linkCost[index_link];
                    }
                }
            }
        }

    }
    hasPathTree=true;

}
