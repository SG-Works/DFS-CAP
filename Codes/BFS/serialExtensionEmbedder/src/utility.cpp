#include"super.h"

MTRand dRand(time(0));

inline void displayVec(bool* q,int n)
{
	for(int i=0;i<n;i++)
		cout<<q[i]<<" ";
	cout << endl;
}
double myRand(double low,double high)
{
	double diff = high - low;
	double temp = dRand();
	return (low + diff*temp);
}
int myRandInt(int low,int high)
{
	double temp = myRand(low,high+1);
	int ret = (int) temp;
	if(ret > high)
		return high;
	else return ret;
}


void displayQ(deque< vector <int> > Q)
{
	for(int i=0;i<Q.size();i++)
	{	
		for(int j=0;j<Q[i].size();j++)
			cout << Q[i][j] << " ";
		cout << endl;
	}
	cout << "Number of serial xtensions = " << Q.size() << endl;
}

void displayTransClose(fstream& fout,pattern* p)
{
	for(int i = 0;i<p->size;i++)
		fout<<p->events[i]<<",";
	fout << ":";
	for(int i = 0;i<p->size;i++)
		for(int j=0;j<p->size;j++)
			if(p->adjMat[i][j] == 1)
				fout<<i+1<<">"<<j+1<<",";
	fout<<endl;
}
