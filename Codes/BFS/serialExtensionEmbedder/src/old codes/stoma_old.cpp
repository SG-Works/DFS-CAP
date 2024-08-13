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

MTRand dRand(time(0));

using namespace std;
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
void generateStream(vector<deque< vector<int> > >Q,double,double,int,double);
#if 1
pattern::pattern(int n)
{
	size = n;
	for(int i=0;i<n;i++)
	{
		vector<bool> temp;
		adjMat.push_back(temp);
		events.push_back(0);
		for(int j=0;j<n;j++)
			adjMat[i].push_back(0);
	}
}
#endif
pattern::~pattern()
{
	for(int i=0;i<size;i++)
		adjMat[i].clear();
	adjMat.clear();
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


int main(int argc,char* argv[])
{
	double rho, eta;
	int numNeu;
	double simTime;
	if(argc != 5)
	{
		cout << "usage :: serialEmbedd.exe <rho> <eta> <number of neurons> <simulation time> " << endl;
		exit(0);
	}
	rho = atof(argv[1]);
	eta = atof(argv[2]);
	numNeu = atoi(argv[3]);
	simTime = atof(argv[4]);
	srand(time(0));
	int nop =0;
	fstream pf;
	pf.open("patterns",ios::in);
	vector<deque<vector<int> > > Q;
	pf >> nop;
	for(int n=0;n<nop;n++)
	{
		deque< vector<int> > t;
		int siz;
		pf >> siz;
		pattern* p = new pattern(siz);
		string line;
		pf>>line;
		loadPattern(p,line,siz);
		getSerialXtensions(p,t,siz);
//		displayQ(t);
		Q.push_back(t);
		delete p;
		cout << "pattern-"<<n+1<< " loaded."<<endl;
	}	
	generateStream(Q,rho,eta,numNeu,simTime);
}

void getSerialXtensions(pattern* p,deque< vector<int> > &Q,int size)
{
	//p->display();
	vector<int> init;
	Q.push_back(init);
	for(int pass=0;pass<size;pass++)
	{
		int len = Q.size();
		//cout << len << endl;
		for(int k=0;k<len;k++)
		{
			vector<int> temp = Q[0];
			Q.pop_front();
			bool q[size],w[size];
			for(int i=0;i<size;i++)
			{	
				q[i] = false;w[i]=false;
			}
			for(int i=0;i<size;i++)
			{
				for(int j=0;j<temp.size();j++)
				{
					if(p->events[i] == temp[j])
					{
						q[i] = true;
						break;
					}
				}
			}
			//we have now got q
			//cout<<"q = ";displayVec(q,size);
			for(int i=0;i<size;i++)
			{	
				if(q[i] == true) 
				{
					w[i] = false;
					continue;
				}
				bool flag = true;
				for(int j=0;j<size;j++)
				{
					if(p->adjMat[j][i] == 1 && q[j] == 0)
					{
						flag = false; break;
					}
				}
				w[i] = flag;
			}
			// now we have got w
			//cout<<"w = ";displayVec(w,size);
			for(int i=0;i<size;i++)
			{
				if(w[i] == true)
				{
					Q.push_back(temp);
					Q.back().push_back(p->events[i]);
				}
			}
		}
	}
	
}


#define MAXLEN 100
void loadPattern(pattern* p,string lines,int size)
{
	char* line = new char[lines.size()+1];
	strcpy(line,lines.c_str());
	int iTemp,jTemp=0,kTemp;
	char strEventTemp[26];
	int i=0,j=0, iEvent = 0;
	int loc_i=-1,loc_j=-1;
	int colonloc;
	colonloc = (int)lines.find_first_of(":");
	for(iTemp=0,jTemp=0;iTemp<colonloc;iTemp++)
	{
		if(line[iTemp] == ',')
		{	strEventTemp[jTemp] = '\0';jTemp=0; p->events[iEvent] = atoi(strEventTemp); iEvent++; }
		else if(isdigit(line[iTemp]))
		{	strEventTemp[jTemp] = line[iTemp]; jTemp++; }
		else if(!isspace(line[iTemp]))
		{	printf("1485 ::ERROR ::: INvalid Char in the Input\n"); exit(0); }
	}
	strEventTemp[jTemp] = '\0';
	p->events[iEvent] = atoi(strEventTemp); iEvent++;
	if(iEvent != size) {	printf("1489 :: ERROR in number of Events\n"); }
	//printf("Event Array : ");
	//for(iTemp=0;iTemp<size;iTemp++) 
	//	printf("%d ",p->events[iTemp]);
	//printf("\n");
	for(iTemp=colonloc+1,jTemp=0;iTemp<lines.size();iTemp++)
	{	
		if(line[iTemp] == '>')
		{	strEventTemp[jTemp] = '\0'; i = atoi(strEventTemp); jTemp = 0;}
		else if(line[iTemp] == ','||line[iTemp]=='\n')
		{
			strEventTemp[jTemp] = '\0';
			j = atoi(strEventTemp);
			for(jTemp=0;jTemp < size;jTemp++)
			{
				if(p->events[jTemp] == i)
				{	loc_i = jTemp; break; }					
			}
			if(jTemp == size) { printf("1503 :: ERROR : RHS and LHS doesnt MAtch\n"); }
			for(jTemp=0;jTemp<size;jTemp++)
			{
				if(p->events[jTemp] == j)
				{ 	loc_j = jTemp; break; }
			}
			if(jTemp == size) { printf("1509 :: ERROR : RHS and LHS doesnt MAtch\n"); }
			p->adjMat[loc_i][loc_j] = 1;
			jTemp = 0;
		}
		else if(isdigit(line[iTemp]))
		{ strEventTemp[jTemp] = line[iTemp];jTemp++;}
		else if(isspace(line[iTemp] == ' '))
		//	jTemp = 0;
		;
		else
		{	 printf("1518 :: ERROR ::: Invalid Character in Candidate Representation.\n"); exit(0); }
	}
	for(iTemp=0;iTemp<size;iTemp++)
		for(jTemp=0;jTemp<size;jTemp++)
			if(p->adjMat[iTemp][jTemp] == 1)
				for(kTemp=0;kTemp<size;kTemp++)
					if(p->adjMat[jTemp][kTemp] == 1)
						p->adjMat[iTemp][kTemp] = 1;
}



void generateStream(vector<deque<vector<int> > > Q,double rho,double eta, int numNeu,double simTime)
{
	double deltaTee = .001;
	int serial =0;
	int noS=0;
	int len=0;
	bool mode =false;
	int count =0;
	int episode =0;
	int epicnt =0;
	cout<<"Number of Embedded patterns = "<<Q.size()<<endl;
	fstream fout;
	fout.open("stream.txt",ios::out);
#if 1
	for(double i=0;i<=simTime;i=i+deltaTee)
	{
		if(mode == false && myRand(0,1) > rho) 
		{
			epicnt++;
			mode = true;
			episode = myRandInt(0,Q.size()-1);
			len = Q[episode][0].size();
			noS = Q[episode].size();
			serial = myRandInt(0,noS-1);
			count = 0;
		}
		if(mode == true)
		{
			if(myRand(0,1) > eta)
			{
				fout<<Q[episode][serial][count]<<","<<i<<endl;
				count++;
				if(count == len) {count =0;mode =false;}		
			}
			else
			        fout<<myRandInt(1,numNeu)<<","<<i<<endl;
		}
		else
		{
			fout<<myRandInt(1,numNeu)<<","<<i<<endl;
		}
		
		
	}
	cout << "NUmber of embeddings = "<<epicnt<<endl;
#endif
	fout.close();
}
