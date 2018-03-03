//***************************************************************************
// NNode.CPP
//
// implement the function and reloaded operator of class NNode in this .cpp file.
//***************************************************************************

#include "fibheap.h"
#include "mainwindow.h"
using namespace std;

void NNode::Print()
{
     FibHeapNode::Print();
     cout << totalCost;
}

void NNode::operator =(float NewKeyVal)
{
    NNode Temp;

     Temp.totalCost = totalCost = NewKeyVal;
     FHN_Assign(Temp);
}

void NNode::operator =(FibHeapNode& RHS)
{
     FHN_Assign(RHS);
     totalCost = ((NNode&) RHS).totalCost;
}

int  NNode::operator ==(FibHeapNode& RHS)
{
     if (FHN_Cmp(RHS)) return 0;
     return totalCost == ((NNode&) RHS).totalCost ? 1 : 0;
}

int  NNode::operator <(FibHeapNode& RHS)
{
int X;

     if ((X=FHN_Cmp(RHS)) != 0)
      return X < 0 ? 1 : 0;

     return totalCost < ((NNode&) RHS).totalCost ? 1 : 0;
};

/*test usage
    NNode *Min;
    NNode *A;
    NNode temp;
    FibHeap *theHeap = NULL;
    int Max=20;
   // Setup for the Fibonacci heap

     if ((theHeap = new FibHeap) == NULL || (A = new NNode[Max+1]) == NULL)
     {
     cout << "Memory allocation failed-- ABORTING.\n";
         exit(-1);
     }

     for (int i=0;i<Max;i++)
         A[i].SetKeyValue ((float)i);
     for (int i=Max-1;i>=0;i--)
         theHeap->Insert(&A[i]);
     //A[5].SetKeyValue((float)100);

     temp=A[5];
     temp.totalCost=A[5].totalCost-100;
     theHeap->DecreaseKey(&A[5],temp);

     cout << "before #nodes = " << theHeap->GetNumNodes() << endl;

     for (int i=0;i<Max;i++) {
         if ((Min = (NNode*) theHeap->ExtractMin ()) != NULL) {
            float key = Min->totalCost;
            cout << key << ' ';
         }
     }
     cout << endl;

     for (int i=0;i<Max;i++)
         cout << A[i].GetKeyValue() << ' ';

     cout << "after #nodes = " << theHeap->GetNumNodes() << endl;


     delete theHeap;
*/
