#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<deque>
#include<algorithm>
#include"mtrand.h"
using namespace std;

struct parameters
{
	double simTime;
	int numNeu;
	double rho,p,eta;
};

class pattern
{
public:
	int size;
	vector<int> events;
	vector< vector<bool> > adjMat;
	inline void display()
	{
		for(int i=0;i<size;i++)
			cout <<events[i]<<" ";
		cout << "\t: ";
		for(int i=0;i<size;i++)
		{
			for(int j=0;j<size;j++)
				if(adjMat[i][j])
					cout<<events[i]<<"->"<<events[j]<<" ";
		}
		cout << endl;

	}
	pattern(int);
	~pattern();
};
void loadPattern(pattern* p,string lines,int size);
void getSerialXtensions(pattern*,deque< vector<int> >&,int);
void generateStream(vector<deque< vector<int> > >Q,parameters param);
void displayTransClose(fstream& fout,pattern* p);
inline void displayVec(bool*,int);
double myRand(double,double);
int myRandInt(int,int);
void displayQ(deque< vector< int > >);
