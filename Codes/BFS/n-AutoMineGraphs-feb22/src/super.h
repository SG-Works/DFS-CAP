#include<iostream>
#include<deque>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>
//#include "lapack++/lapack++.h"
using namespace std;

class readfile
{
	int* buffer;
	double* timebuffer;
	double timetemp;
	int pos;
	ifstream fptr;
	void buffin();
	void reset();
public:
	readfile(char *);
	~readfile();
	int getnextevent(double&);
	int getnextevent();
	double getTime();
	void pushBackOne();	
};

class freqEpisodes
{
  bool LmaxFlag;
	int tresh; 
	int maxSiz,maxLength,maxWidth;
	int maxEvents;
	int NoTimeTicks;
	double expTime,fudgeFac,entTresh,rho;
	int episodeType;
	char filename[100];
    string outfile;
	class alpha;
	class automaton;
	class beta
	{
	public:
		alpha* ptrAlpha;
		automaton* ptrAuto;
		//bool* q;
		int j;
		beta *pPrev,*pNext;
		beta(int len);
		~beta();
	};
	class automaton
	{
	public:
		bool* q;
		bool* w;
		double t1;
		bool* flag;
		bool** trackMat;
		beta** betaAddress;
		automaton* pNext;
		void deleteMat(int len);
		void updateMat(int len,bool* q, int j);
		void copyMat(int len, bool **mat);
		automaton(int len);
		~automaton();
	};
	class alpha
	{
	public:
		bool flag;
		bool *initWaits;
		long int iFrequency;
		int* paiEvents;
		int** ppEntropyMatrix;
		bool** ppEventMatrix;
		automaton* automataList;
		double expiryTime;
		alpha* pstBlockStart;
		//adding two more variables which are calculated thresholds.
		int FreqThresholdStatistical;
		int blocStart;
		alpha* pNext;		 
		alpha* pPrev; //addition for doubly linked.
		double calculate_entropy(int len);
		double entropy;
		void initialize(int);
		void deleteMat(int);
		void display(int);
		void display(fstream&,int);
		void transredMatDisp(fstream& out,int len);
		alpha(int);
		~alpha();
	};
	class stEpisodeGel
	{
	public:
		int iEpisodeListLength;
		int iEventLength;
		alpha* ptrAlpha;
		alpha* pstTail;
		void filter();
		void ent_filter(double);
		stEpisodeGel();
	};
	class dummyEpisode
	{
	public:
		int* paiEvents;
		bool** ppEventMatrix;
		dummyEpisode(int);
		void deleteMat(int);
		~dummyEpisode();
	};
	vector <stEpisodeGel> GelHeads;
	// Utlity functions
	void transred(bool**,int);
	int maxDegree(bool**,int);
	int lengthMaxPath(bool**,int);
	int numMaxPath(bool**,int);
	int pathFinder2(int,bool**,int*,int);
	int pathFinder(int,bool**,int*,int);
	int compare_containment(bool**,bool**,int);
	int compare_event(int*,int*,int);
	bool chkEvent(int*,int,int);
	void initW(bool*,bool**,int);
	void relinkDelBeta(beta*&,beta*);
	void addAutoAtHead(alpha*, automaton*);
	void addBetaAtHead(beta*&, beta*);
	bool compareQ(bool*,bool*,int);
	void assignVec(bool*,bool*,int);
	void assignQNull(bool*,int);
	bool checkQNull(bool*,int);
	bool checkQallOne(bool*,int);
	bool checkVecSame(bool*,bool*,int);
	void addMatrix(int**,bool**,int);
public:
    int addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,const int iFrequency);
	alpha* existSubEpisode(alpha*,int*,bool**,int);
	void generatePossibleCandidates(alpha*,alpha*,alpha**,int);
	alpha* catEpisodes(alpha*,alpha*,int);
	void cleanupWaits(beta**);
	void cleanupAutoList(alpha*,int);
	void initCandidates(stEpisodeGel&);
	void generateCandidates(stEpisodeGel,stEpisodeGel&);
	void findFreqCandids(stEpisodeGel&);
	void findFreqEpisodes();
	void displayFreqEpisodes(char*);
	void displayLevelWise(fstream&,int);
	void parser(string);
	void loadCandidate(alpha*,int,string);
	void filter();
	void filter(int);
	freqEpisodes(double expTime,int tresh,double entTresh,int maxLn1,int maxWid1,double fugeFac1,int maxSiz,int maxEvents,char*,string,int,bool, int NoTimeTicks, double rho);
};

void errorAbort(string);
void errorEcho(string);
void displayVec(bool*,int);
double ComputeSignificanceThreshold(int *alphag, bool **alphae, bool single, int M, int NoTimeTicks, double rho, int size); 
