#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<cmath>
using namespace std;
inline void errorAbort(string s) {cout<<s<<endl;exit(0);}
inline void errorEcho(string s) {cout << s<< endl;}

inline void displayVec(bool* v,int len)
{
	for(int i=0;i<len;i++)
		cout << v[i];
	cout << endl;
}
#define BUFSIZE 10000
#define MAX_LONGINT 10000000
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
};
readfile::~readfile()
{
	fptr.close();
	delete []buffer;
	delete []timebuffer;
}
void readfile::reset()
{
	for(int i=0;i<BUFSIZE;i++)
	{	buffer[i]=-1;timebuffer[i]=0;}
}
readfile::readfile(char* filename)
{
	fptr.open(filename,ios::in);
	if(!fptr) errorAbort("ERROR :: Input file not read properly ");
	
	pos = BUFSIZE;
	buffer = new int[BUFSIZE];
	timebuffer = new double[BUFSIZE];
}
void readfile::buffin()
{
	reset();
	for(int i=0;i<BUFSIZE;i++)
		if(fptr)
		{	
			char dummy;
			fptr >> buffer[i];
			fptr >> dummy;
			fptr >> timebuffer[i];
		}
		else break;
	pos = 0;
}
int readfile::getnextevent(double &time)
{	
	if(pos == BUFSIZE)
		buffin();
	timetemp = timebuffer[pos];
	time = timebuffer[pos];
	return buffer[pos++];
}
int readfile::getnextevent()
{	
	if(pos == BUFSIZE)
		buffin();
	timetemp = timebuffer[pos];
	return buffer[pos++];
}
double readfile::getTime()
{
	return timetemp;
}
class freqEpisodes
{
	int tresh; int maxSiz,maxDeg;int maxEvents;double expTime,fudgeFac,entTresh;
	char filename[100];
	class alpha;
	class beta
	{
	public:
		alpha* ptrAlpha;
		bool* q;
		//bool* w;
		int j;
		beta *pPrev,*pNext;
		beta(int len);
		~beta();
	};
	class automaton
	{
	public:
		bool* q;
		double t1;
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
		long int iFrequency;
		int* paiEvents;
		int** ppEntropyMatrix;
		bool** ppEventMatrix;
		//bool* initWaits;
		automaton* automataList;
		double expiryTime;
		alpha* pstBlockStart;
		int blocStart;
		alpha* pNext;
		//double entropy;		 
		double calculate_entropy(int len);
		void initialize(int);
		void deleteMat(int);
		inline void display(int len)
		{
			
			for(int i=0;i<len;i++)
					cout << paiEvents[i]<<" ";
			cout << "\t: ";
			cout << iFrequency<<"\t: ";
			cout << calculate_entropy(len) <<"\t:  ";  
			//cout << pstBlockStart<<"\t: ";
			for(int i=0;i<len;i++)
			{
				for(int j=0;j<len;j++)
					if(ppEventMatrix[i][j])
						cout<<paiEvents[i]<<"->"<<paiEvents[j]<<" ";
			}
			cout << endl;
		}
		inline void display(fstream& out,int len)
		{
			//transred(ppEventMatrix,len);
			//out << iFrequency<<"\t: ";
			//out << entropy <<"\t:  "; 
			
			for(int i=0;i<len;i++)
					out << paiEvents[i]<<" ";
			out << "\t: ";
			out << iFrequency<<"\t: ";
			out << calculate_entropy(len) <<"\t:  "; 
			out << pstBlockStart<<"\t: ";
			//out << entropy<<"\t";
			for(int i=0;i<len;i++)
			{
				for(int j=0;j<len;j++)
					if(ppEventMatrix[i][j])
						out<<paiEvents[i]<<"->"<<paiEvents[j]<<" ";
			}
			out << endl;
			//for(int i=0;i<len;i++)
			//{
			//	for(int j=0;j<len;j++)
			//			out<<paiEvents[i]<<"->"<<paiEvents[j]<<" ("<<ppEntropyMatrix[i][j]<<") ";
			//}
			//out << endl;
		}
#if 0
		int maxDegree(int len)
		{
			int max = 0;
			for(int i=0;i<len;i++)
			{
				int count = 0;
				for(int j=0;j<len;j++)
				{
					if(ppEventMatrix[i][j] == 1) count++;
				}
				if(count > max) max = count;
			}
			for(int i=0;i<len;i++)
			{
				int count = 0;
				for(int j=0;j<len;j++)
				{
					if(ppEventMatrix[j][i] == 1) count++;
				}
				if(count > max) max = count;
			}
			return max;
		}
#endif
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
	//Utility Functions
	void transred(bool **mat , int len)
	{	
		int i,j,k;
		int **copy;
		copy = (int**) malloc(sizeof(int) * len);
		for(i = 0 ; i < len ; i++)
			copy[i] = (int*) malloc(sizeof(int) * len);
		for(i = 0 ; i < len ; i++)
			for(j = 0 ; j < len ; j++)
				copy[i][j] = mat[i][j];
		for(i = 0 ; i < len ; i++)
		{
			for(j = 0 ; j < len; j++)
			{
				if(mat[i][j])
				{
					for(k = 0 ; k < j ; k++)
						if(copy[i][k] && copy[k][j]) 
							mat[i][j] = 0;
					for(k = j+1 ; k < len ; k++)
						if(copy[i][k] && copy[k][j]) 
							mat[i][j] = 0;
				}
				
			}
		}
		for(i=0;i<len;i++)
			free(copy[i]);
		free(copy);
	}
	int maxDegree(bool** mat,int len)
	{
		bool** copy;
		copy = (bool**) malloc(sizeof(bool*) * len);
		for(int i = 0;i<len;i++)
			copy[i] = (bool*) malloc(sizeof(bool)*len);
		for(int i=0;i<len;i++)
			for(int j = 0;j<len;j++)
				copy[i][j] = mat[i][j];
		transred(copy,len);
		int max = 0;
		for(int i=0;i<len;i++)
		{
			int count = 0;
			for(int j=0;j<len;j++)
			{
				if(copy[i][j] == 1) count++;
			}
			if(count > max) max = count;
		}
		for(int i=0;i<len;i++)
		{
			int count = 0;
			for(int j=0;j<len;j++)
			{
				if(copy[j][i] == 1) count++;
			}
			if(count > max) max = count;
		}
		for(int i = 0;i<len;i++)
			free(copy[i]);
		free(copy);
		return max;
	}
	inline int compare_containment(bool **a , bool **b, int eventLength)
	{
		int i,j;
		int flag = 0;
		for(i = 0 ; i < eventLength ; i++)
		{
			for(j=0;j<eventLength;j++)
			{
				if(a[i][j] == 1 && b[i][j] == 0)
				{
					if(flag == -1) {flag = 2; break;}
					else { flag = 1; }
				}
				if(a[i][j] == 0 && b[i][j] == 1)
				{
					if(flag == 1) {flag = 2; break;}
					else { flag = -1 ;}
				}
			}
			if(flag == 2) {flag =0; break;}
		} 
		return flag;
	
	}
	inline int compare_event(int *a , int *b , int eventLength)
	{
		int flag = 1;int i = 0;
		for(i=0;i<eventLength;i++,a++,b++)
		{
			if(*a != *b) {flag = 0 ; break;}
		}
		return flag;
	
	}

	inline bool chkEvent(int *array, int event,int len)
	{
		bool flag = false;
		for(int i = 0;i<len;i++)
			if(array[i] == event) {flag = true; break;}
		return flag;
	}
	inline void initW(bool* w,bool ** adjMat,int len)
	{
		for(int i=0;i<len;i++)
		{
			bool flag = true;
			for(int j=0;j<len;j++)
				if(adjMat[j][i] == true) {flag = false;break;}
			if(flag == true) w[i] = true;
			else w[i] = false;
		}
	}
	inline void relinkDelBeta(beta*&head,beta* ptrBeta)
	{
		if(ptrBeta->pPrev == NULL) head = ptrBeta->pNext;
		else ptrBeta->pPrev->pNext = ptrBeta->pNext;
		if(ptrBeta->pNext) ptrBeta->pNext->pPrev = ptrBeta->pPrev;
	}
	inline void addAutoAtHead(alpha* ptrAlpha, automaton* node)
	{
		node->pNext = ptrAlpha->automataList;
		ptrAlpha->automataList = node;
	}
	inline void addBetaAtHead(beta*& head, beta* node)
	{
		node->pNext = head;
		head = node;
		if(node->pNext) node->pNext->pPrev = node;
	}
	inline bool compareQ(bool* q1,bool* q2,int len)
	{
		bool flag = true;
		for(int i=0;i<len;i++)
			if(q1[i] != q2[i]) {flag = false; break;}
		return flag;
	}
	inline void assignVec(bool* source, bool* dest,int len)
	{
		for(int i=0;i<len;i++)
			dest[i] = source[i];
	}
	inline void assignQNull(bool* dest,int len)
	{
		for(int i=0;i<len;i++)
			dest[i] = 0;
	}
	inline bool checkQNull(bool* q,int len)
	{
		bool flag = true;
		for(int i=0;i<len;i++)
			if(q[i] != 0) { flag = false; break;}
		return flag;
	}
	inline bool checkQallOne(bool* q,int len)
	{
		bool flag = true;
		for(int i=0;i<len;i++)
			if(q[i] != 1) { flag = false; break;}
		return flag;
	}
	inline void addMatrix(int** entMat,bool** trackMat,int len)
	{
		for(int i=0;i<len;i++)
			for(int j=0;j<len;j++)
				entMat[i][j] = entMat[i][j] + trackMat[i][j];
	}
	// utility functions -- end
public:
        int addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,const int iFrequency);
	int existSubEpisode(alpha*,int*,bool**,int);
	void generatePossibleCandidates(alpha*,alpha*,alpha**,int);
	alpha* catEpisodes(alpha*,alpha*,int);
	void cleanupWaits(beta**);
	void initCandidates(stEpisodeGel&);
	void generateCandidates(stEpisodeGel,stEpisodeGel&,int);
	void findFreqCandids(stEpisodeGel&);
	void findFreqEpisodes();
	void displayFreqEpisodes(char*);
	void parser(string,int);
	void filter();
	void filter(int);
	freqEpisodes(double expTime,int tresh,double entTresh,int maxDeg1,double fugeFac1,int maxSiz,int maxEvents,char*);
};
//CONSTRUCTORS & Destructors -- begin
freqEpisodes::freqEpisodes(double expTime1,int tresh1,double entTresh1,int maxDeg1,double fudgeFac1,int maxSiz1,int maxEvents1, char* file)	
{
	expTime = expTime1;
	//cout << "Expiry Time = "<<expTime;
	tresh = tresh1;
	entTresh = entTresh1;
	maxDeg = maxDeg1;
	maxSiz = maxSiz1;
	maxEvents = maxEvents1;
	fudgeFac = fudgeFac1;
	char* f = filename;
	while(*f++ = *file++);
}
freqEpisodes::stEpisodeGel::stEpisodeGel()
{
	iEventLength = 0;
	iEpisodeListLength = 0;
	ptrAlpha = NULL;
	pstTail = NULL;
}
//alpha --cons
freqEpisodes::alpha::alpha(int len)
{
	flag = false;
	iFrequency = 0;
	paiEvents = new int[len];
	ppEventMatrix = new bool*[len];
	ppEntropyMatrix = new int*[len];
	//initWaits = new bool[len];
	for(int i=0;i<len;i++)
	{paiEvents[i] =0 ; ppEventMatrix[i] = new bool[len]; ppEntropyMatrix[i] = new int[len];}
	pstBlockStart = NULL;
	automataList = NULL;
	expiryTime = 0.0;
	pNext = NULL;
	blocStart = 0;
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			{ppEventMatrix[i][j] =0;ppEntropyMatrix[i][j] = 0;}
}
#if 0
void freqEpisodes::alpha::initialize(int len)
{
	for(int i = 0;i<len;i++)
	{
		bool flag = true;
		for(int j=0;j<len;j++)
			if(ppEventMatrix[j][i] == 1) {flag = false;break;}
		if(flag) initWaits[i] = true;
	}
}
#endif
//alpha -- des
void freqEpisodes::alpha::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		{delete []ppEventMatrix[i]; delete []ppEntropyMatrix[i];}
}
freqEpisodes::alpha::~alpha()
{
	delete []paiEvents;
	delete []ppEventMatrix;
	delete []ppEntropyMatrix;
}
double freqEpisodes::alpha::calculate_entropy(int len)
{
	int** entMat = ppEntropyMatrix;
	bool** adjMat = ppEventMatrix;
	int iTemp,jTemp;
	double prob;
	double ent = 1.0 ,entTemp = 0.0;
	//*avgent = 0;
	for(iTemp = 0 ; iTemp < len ; iTemp++)
	{
		for(jTemp = iTemp + 1 ; jTemp < len ; jTemp++)
		{	
			if(adjMat[iTemp][jTemp] == 0 && adjMat[jTemp][iTemp] == 0 )
			{
				
				prob = ((double)entMat[iTemp][jTemp])/(entMat[iTemp][jTemp] + entMat[jTemp][iTemp]);
				if(prob != 0 && prob != 1)
				{
				entTemp = (prob * log(1/prob) + (1-prob) * log(1/(1 - prob)))/log(2);
				ent = (ent < entTemp) ? ent : entTemp ;				
				}
				else
				{
					return 0.0;			 
				}
			}
						
		}
	}
	return ent;
}
//dummyEpisode -- cons
freqEpisodes::dummyEpisode::dummyEpisode(int len)
{
	paiEvents = new int[len];
	ppEventMatrix = new bool*[len];
	for(int i=0;i<len;i++)
		{paiEvents[i] = 0 ;ppEventMatrix[i] = new bool[len];}
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			ppEventMatrix[i][j] =0;
			
}
void freqEpisodes::dummyEpisode::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		delete []ppEventMatrix[i];
}
freqEpisodes::dummyEpisode::~dummyEpisode()
{
	delete []paiEvents;
	delete []ppEventMatrix;
}
//beta -- cons
freqEpisodes::beta::beta(int len)
{
	ptrAlpha = NULL;
	q = new bool[len];
	//w = new bool[len];
	for(int i=0;i<len;i++)
	{ q[i] = 0;}
}
//beta -- des
freqEpisodes::beta::~beta()
{
	delete []q;
	//delete []w;
}
//beta -- end
//automaton -- cons
freqEpisodes::automaton::automaton(int len)
{
	pNext = NULL;
	t1 = 0.0;
	q = new bool[len];
	betaAddress = new beta*[len];
	trackMat = new bool*[len];
	for(int i=0;i<len;i++)
		trackMat[i] = new bool[len];
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			trackMat[i][j] = false;
	for(int i=0;i<len;i++)
	{ q[i] = 0; betaAddress[i] = NULL;}
}
//automaton -- des
freqEpisodes::automaton::~automaton()
{
	delete []q;
	delete []betaAddress;
	//delete []trackMat;
	//leftout
}

void freqEpisodes::automaton::deleteMat(int len)
{
	for(int i=0;i<len;i++)
		delete []trackMat[i];
	delete []trackMat;
}
void freqEpisodes::automaton::updateMat(int len,bool* q,int j)
{
	for(int i =0;i<len;i++)
		if(q[i] == false)
			trackMat[j][i] = true;
}
void freqEpisodes::automaton::copyMat(int len,bool** mat)
{
	for(int i=0;i<len;i++)
		for(int j=0;j<len;j++)
			trackMat[i][j] = mat[i][j];
}
//automaton -- end
//CONSTRUCTORS -- end
void freqEpisodes::findFreqEpisodes(void)
{
	stEpisodeGel oneCandid;
	oneCandid.iEventLength = 1;
	initCandidates(oneCandid);
	GelHeads.push_back(oneCandid);
	findFreqCandids(GelHeads[0]);
	for(int i=1;i<maxSiz;i++)
	{
		stEpisodeGel tempGelHead;
		tempGelHead.iEventLength = i+1;
		cout << "Generating "<<i+1<<"-Node Candidates..."<<endl;
		generateCandidates(GelHeads[i-1],tempGelHead,maxDeg);
		GelHeads.push_back(tempGelHead);
		findFreqCandids(GelHeads[i]);
		//filter(i);
	}
	
}
void freqEpisodes::initCandidates(stEpisodeGel& GelHead)
{
	alpha* ptrPrev = NULL;
	for(int i=0;i<maxEvents;i++)
	{
		alpha *ptrTemp = new alpha(1);	if(ptrTemp == NULL) errorAbort("in initCandidates :: out of memory");
		ptrTemp->paiEvents[0] = i+1;
		ptrTemp->ppEventMatrix[0][0] = 0;
		if(ptrPrev != NULL) {ptrPrev->pNext = ptrTemp;ptrTemp->pstBlockStart = GelHead.ptrAlpha;}
		else {GelHead.ptrAlpha = ptrTemp;ptrTemp->pstBlockStart = ptrTemp;}
		ptrPrev = ptrTemp;
	}
	GelHead.iEpisodeListLength = maxEvents;
	return;
}

void freqEpisodes::findFreqCandids(stEpisodeGel& GelHead)
{
	cout<<"Number of "<<GelHead.iEventLength<<"-node candidates being counted = "<<GelHead.iEpisodeListLength<<endl;
	cout << "Frequency treshhold = " << tresh << endl;
	readfile evntStream(filename);
	beta* waitsList[maxEvents];
	int eLen = GelHead.iEventLength;
	for(int i=0;i<maxEvents;i++)
		waitsList[i] = NULL;
	// initialization - only for paralle episodes - betas added in all the waits lists.
	for(alpha* ptrIter1 = GelHead.ptrAlpha;ptrIter1 != NULL;ptrIter1=ptrIter1->pNext)
	{
		ptrIter1->automataList = NULL;
		ptrIter1->iFrequency = 0;
		for(int i=0;i<eLen;i++)
		{
			beta* ptrBetaTemp = new beta(eLen);
			ptrBetaTemp->ptrAlpha = ptrIter1;
			for(int j=0;j<eLen;j++)
			{ ptrBetaTemp->q[j] = 0 ;}
			ptrBetaTemp->j = i;
			ptrBetaTemp->pPrev = NULL;
			addBetaAtHead(waitsList[ptrIter1->paiEvents[i] - 1] , ptrBetaTemp);
		}	
	}
	int Event;
	while((Event = evntStream.getnextevent()) != -1)
	{
		if(Event > maxEvents) errorAbort("Wrong Event");
		double time = evntStream.getTime();
		//setting all the flags to zero - why ???  
		for(alpha* ptrIter1 = GelHead.ptrAlpha;ptrIter1 != NULL;ptrIter1=ptrIter1->pNext)
			ptrIter1->flag = false;
		beta* betaIterNext = NULL;
		for(beta* betaIter = waitsList[Event -1];betaIter != NULL;betaIter = betaIterNext)
		{
			betaIterNext = betaIter->pNext;
			bool qPrev[eLen],qNext[eLen],wNext[eLen];
			int j = betaIter->j;
			alpha* ptrAlpha = betaIter->ptrAlpha;
			if(ptrAlpha->flag == true) continue;
			automaton *ptrAutoQNext=NULL,*ptrAutoQPrev=NULL,*ptrPrevPrev=NULL;
			assignVec(betaIter->q,qPrev,eLen);
			assignVec(betaIter->q,qNext,eLen);
			if(qNext[j] == true) errorAbort("Wrong value for qNext");
			qNext[j] = true;
			//assignVec(betaIter->w,wNext,eLen);
			//if(wNext[j] != true) errorAbort("Wrong Value for wNext");
			//wNext[j] = 0;
			bool ADDWAITS = true;
			// finding the index of q_next -- if it exists
			for(automaton* autoIter = ptrAlpha->automataList;autoIter != NULL;autoIter = autoIter->pNext)
			{
				if(compareQ(qNext,autoIter->q,eLen))	
				{ptrAutoQNext = autoIter; break;}
			}
			//cas  1 :: q_prev = 0
			if(checkQNull(qPrev,eLen) == true) 
			{
				if(ptrAutoQNext != NULL) {ADDWAITS = false;ptrAutoQNext->t1 = time;}
				else 
				{
					ptrAutoQNext = new automaton(eLen);
					if(ptrAutoQNext == NULL) errorAbort("Out of memory in freqCandids");
					ptrAutoQNext->t1 = time;
					assignVec(qNext,ptrAutoQNext->q,eLen);
					ptrAutoQNext->updateMat(eLen,qNext,j);
					addAutoAtHead(ptrAlpha,ptrAutoQNext);
					if(ptrAlpha->automataList != ptrAutoQNext ) errorAbort("automaton not added properly");
				}
				if(ptrAutoQNext == NULL) {errorAbort("in findFreqCandids -autoQNext is NULL:: condition q_prev = 0");}
			}
			//case 2 :: q_prev != 0
			else
			{
				ptrAutoQPrev =NULL;
				for(automaton* autoIter = ptrAlpha->automataList;autoIter != NULL;autoIter = autoIter->pNext)
				{
					if(compareQ(qPrev,autoIter->q,eLen))	{ptrAutoQPrev = autoIter; break;}
					ptrPrevPrev = autoIter;	
				}
				if(ptrAutoQPrev == NULL) {errorAbort("in findFreqCandids - q_prev not found!!");}
				double prevTime = ptrAutoQPrev->t1;
				//deleting the automaton containing qPrev
				if(ptrPrevPrev!= NULL) {ptrPrevPrev->pNext = ptrAutoQPrev->pNext;}
				else {ptrAlpha->automataList = ptrAutoQPrev->pNext;} 
				for(int i =0;i<eLen;i++){
					if(ptrAutoQPrev->betaAddress[i] != NULL){
						if(ptrAutoQPrev->betaAddress[i] == betaIterNext) betaIterNext=betaIterNext->pNext;
						relinkDelBeta(waitsList[ptrAlpha->paiEvents[i] - 1],ptrAutoQPrev->betaAddress[i]);
						delete ptrAutoQPrev->betaAddress[i];
					}
				}
				//creating a duplicate of the tracking matrix
				bool** tempMat;
				tempMat = new bool*[eLen];
				for(int m = 0;m<eLen;m++)
					tempMat[m] = new bool [eLen];
				for(int m =0;m<eLen;m++)
					for(int n =0;n<eLen;n++)
						tempMat[m][n] = ptrAutoQPrev->trackMat[m][n];
				//duplicate created -- 
				ptrAutoQPrev->deleteMat(eLen);
				delete ptrAutoQPrev;		
				if(ptrAutoQNext != NULL) 
				{
					ADDWAITS = false;
					ptrAutoQNext->copyMat(eLen,tempMat);
					ptrAutoQNext->updateMat(eLen,qNext,j);
					ptrAutoQNext->t1 = prevTime;
				}
				else
				{
					ptrAutoQNext = new automaton(eLen);
					if(ptrAutoQNext == NULL) errorAbort("Out of memory in freqCandids");
					ptrAutoQNext->t1 = prevTime;
					assignVec(qNext,ptrAutoQNext->q,eLen);
					ptrAutoQNext->copyMat(eLen,tempMat);
					ptrAutoQNext->updateMat(eLen,qNext,j);
					addAutoAtHead(ptrAlpha,ptrAutoQNext);
					if(ptrAlpha->automataList != ptrAutoQNext) errorAbort("automaton not added properly");
				}
				//destroying the duplicate
				for(int i=0;i<eLen;i++)
					delete []tempMat[i];
				delete []tempMat;
			}
			if((checkQallOne(qNext,eLen)== true) && (time - ptrAutoQNext->t1) <= expTime)
			{
				ptrAlpha->flag = true;
				ptrAlpha->iFrequency++;
				addMatrix(ptrAlpha->ppEntropyMatrix,ptrAutoQNext->trackMat,eLen);
				automaton* autoIterNext = NULL;
				for(automaton* autoIter = ptrAlpha->automataList;autoIter != NULL;autoIter = autoIterNext)
				{
					autoIterNext = autoIter->pNext;
					for(int i =0;i<eLen;i++)
						if(autoIter->betaAddress[i])
						{
							if(autoIter->betaAddress[i] == betaIterNext) betaIterNext=betaIterNext->pNext;
							relinkDelBeta(waitsList[ptrAlpha->paiEvents[i]-1],autoIter->betaAddress[i]);	
						}
					autoIter->deleteMat(eLen);
					delete autoIter;
				}
				ptrAlpha->automataList = NULL;
			}
			if((checkQallOne(qNext,eLen)==false) && ADDWAITS == true)
			{
				//optimize
				for(int i=0;i<eLen;i++)
				{
					bool flag = true;
					for(int k = 0;k<eLen;k++)
						if(ptrAlpha->ppEventMatrix[k][i] == 1 && qNext[k] == 0){	flag = false;break;}
					if(flag == true && qNext[i] == false) wNext[i] = true;
					else wNext[i] = false;
				}
				for(int i=0;i<eLen;i++)
				{
					if(wNext[i] == true)
					{
						beta* ptrBetaTemp = new beta(eLen);
						ptrBetaTemp->ptrAlpha = ptrAlpha;
						assignVec(qNext,ptrBetaTemp->q,eLen);
						//assignVec(wNext,ptrBetaTemp->w,eLen);
						ptrBetaTemp->j = i;
						ptrBetaTemp->pPrev = NULL;
						addBetaAtHead(waitsList[ptrAlpha->paiEvents[i] - 1] , ptrBetaTemp);
						ptrAutoQNext->betaAddress[i] = ptrBetaTemp;
					}
				}
				
			}
		}
	}
	cleanupWaits(waitsList);
	alpha* alphaPrev = NULL;
	alpha* alphaNext = NULL;
#if 1	
	//for(alpha* alphaIter = GelHead.ptrAlpha;alphaIter!=NULL;alphaIter = alphaIter->pNext)
	//	alphaIter->calculate_entropy(eLen);
	for(alpha* alphaIter = GelHead.ptrAlpha;alphaIter!=NULL;alphaIter = alphaNext)
	{
		alphaNext = alphaIter->pNext;
		automaton* autoIterNext;
		for(automaton* autoIter=alphaIter->automataList;autoIter!=NULL;autoIter=autoIterNext)
		{ autoIterNext = autoIter->pNext; autoIter->deleteMat(eLen); delete autoIter;}
		alphaIter->automataList = NULL;
		double fcuk = alphaIter->calculate_entropy(GelHead.iEventLength);
		//bool falg = true;
		//for(int i=0;i<eLen;i++)
		//{
		//	if(alphaIter->paiEvents[i] != i+8) falg = false;
		//}
		//if(falg == true) {alphaIter->display(eLen);}
		if( fcuk < entTresh || alphaIter->iFrequency < tresh)
		{
			
			if(alphaPrev == NULL)
				GelHead.ptrAlpha = alphaIter->pNext;
			else
				alphaPrev->pNext = alphaIter->pNext;
			alphaIter->deleteMat(eLen);
			delete alphaIter;
			GelHead.iEpisodeListLength--;
		}
		else {alphaPrev = alphaIter;}
	}
#endif
	cout << "NUmber of Frequent "<<eLen<<"-Node episodes = "<<GelHead.iEpisodeListLength<<endl<<endl;
	tresh = int(tresh * fudgeFac);
}
void freqEpisodes::cleanupWaits(beta** waitsList)
{
	for(int i=0;i<maxEvents;i++)
	{
		beta* betaIterNext = NULL;
		for(beta* betaIter = waitsList[i];betaIter != NULL;betaIter = betaIterNext)
		{
			betaIterNext = betaIter->pNext;
			delete betaIter;
		}
	}
}
void freqEpisodes::displayFreqEpisodes(char* outfile)
{
	fstream out;
	out.open(outfile,ios::out);
	for(int i=0;i<GelHeads.size();i++)
	{
		int len = GelHeads[i].iEventLength;
		out << len << "-NOde Episodes = "<<GelHeads[i].iEpisodeListLength<<endl;
		for(alpha* alphaIter = GelHeads[i].ptrAlpha; alphaIter!=NULL; alphaIter=alphaIter->pNext)
		{
				//transred(alphaIter->ppEventMatrix,len);
				alphaIter->display(out,len);
		}
	}
	out.close();
}
void freqEpisodes::generateCandidates(stEpisodeGel prevHead,stEpisodeGel& Head,int maxDeg)
{
	int l = prevHead.iEventLength;
	for(alpha* mainIter = prevHead.ptrAlpha;mainIter!=NULL;mainIter = mainIter->pNext)
	{
		alpha* pstBlockStart = NULL;
		for(alpha* blockIter=mainIter->pNext;blockIter!=NULL && blockIter->pstBlockStart == mainIter->pstBlockStart;blockIter=blockIter->pNext)
		{
			alpha* (posEpisodes[3]) = {NULL};
			if(mainIter->paiEvents[l-1] == blockIter->paiEvents[l-1])
				continue;
			generatePossibleCandidates(mainIter,blockIter,posEpisodes,l);
			for(int iTemp=0;iTemp<3 && posEpisodes[iTemp] != NULL;iTemp++)
			{
				bool addFlag = true;
				dummyEpisode* ptrDummy = new dummyEpisode(l);
				for(int jTemp = 0; (jTemp<l-1) && (addFlag == true);jTemp++)
				{
							
#if 1
					for(int kTemp=0;kTemp<jTemp;kTemp++)
					{
						ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp];
						for(int zTemp =0;zTemp<jTemp;zTemp++)
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp];
						}
						for(int zTemp =jTemp;zTemp<l;zTemp++) // CCheck the condition
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] =  posEpisodes[iTemp]->ppEventMatrix[kTemp][zTemp+1];
						}
					}
					for(int kTemp=jTemp;kTemp<l;kTemp++)
					{
						ptrDummy->paiEvents[kTemp] = posEpisodes[iTemp]->paiEvents[kTemp+1];
						for(int zTemp =0;zTemp<jTemp;zTemp++)
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp];
						}
						for(int zTemp =jTemp;zTemp<l;zTemp++) // CCheck the condition
						{
							ptrDummy->ppEventMatrix[kTemp][zTemp] = posEpisodes[iTemp]->ppEventMatrix[kTemp+1][zTemp+1];
						}
					}

#endif				
					if( -1  == existSubEpisode(prevHead.ptrAlpha,ptrDummy->paiEvents,ptrDummy->ppEventMatrix,l))
					{
						addFlag = false;
					}

				}

				//if(true == addFlag && maxDegree(posEpisodes[iTemp]->ppEventMatrix,l+1) <= maxDeg)					
				if(true == addFlag)					
				{
					if(( addCandidateEpisode(Head,posEpisodes[iTemp],&pstBlockStart,0)) == -1)
					{
						cout <<" Out of Memory - Higher Threshold"<<endl;
						exit(0);
					}
				}	
				else
				{
					posEpisodes[iTemp]->deleteMat(l+1);
					delete posEpisodes[iTemp];
				}
				ptrDummy->deleteMat(l);
				delete ptrDummy;
				
			}
			
		}
	}
	
}
void freqEpisodes::generatePossibleCandidates(alpha *pstEpi1,alpha *pstEpi2,alpha *(pstTemp[3]),int iEventLen)
{
	int iTemp =0;
	int iDirFlag =0;

	for(iTemp =0;iTemp<iEventLen-1;iTemp++)
	{
		if((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1]))
		{
			iDirFlag = 1;
		}
		if((1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]))
		{
			iDirFlag = 2;
		}
	}

	if(iDirFlag != 0)
	{
		pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
		if(1 == iDirFlag)
		{
			pstTemp[0]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
		}
		else
		{
			pstTemp[0]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
		}
		return;

	}
	else
	{
		pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);

		for(iTemp =0;iTemp<iEventLen-1;iTemp++)
		{
			if(((1 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (0 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
				((0 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (1 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1])))
			{
				if(iDirFlag != 2)
				{
					iDirFlag = 1;
				}
				else
				{
					return;
				}
			}

			if(((0 == pstEpi1->ppEventMatrix[iEventLen-1][iTemp]) && (1 == pstEpi2->ppEventMatrix[iEventLen-1][iTemp])) ||
				((1 == pstEpi1->ppEventMatrix[iTemp][iEventLen-1]) && (0 == pstEpi2->ppEventMatrix[iTemp][iEventLen-1])))
			{
				if(iDirFlag != 1)
				{
					iDirFlag = 2;
				}
				else
				{
					return;
				}
			}

		}
		switch(iDirFlag)
		{
			case 0:
					{
						pstTemp[1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[1]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
						pstTemp[2] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[2]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
					}
					break;
			case 1:
					{
						pstTemp[1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[1]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
					}
					break;
			case 2:
					{
						pstTemp[1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[1]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
					}
					break;

		}
	}
	return ;
}
freqEpisodes::alpha* freqEpisodes::catEpisodes(alpha *pstEpi1,alpha *pstEpi2,int iEventLen)
{
	alpha* pstTemp = NULL;
	int iTemp =0,jTemp =0;

	pstTemp = new alpha(iEventLen+1);

	for(iTemp =0;iTemp<iEventLen;iTemp++)
	{
		pstTemp->paiEvents[iTemp] = pstEpi1->paiEvents[iTemp];

		for(jTemp =0;jTemp<iEventLen;jTemp++)
		{
			pstTemp->ppEventMatrix[iTemp][jTemp] = pstEpi1->ppEventMatrix[iTemp][jTemp];
		}
	}

	pstTemp->paiEvents[iEventLen] = pstEpi2->paiEvents[iEventLen-1];
	for(jTemp =0;jTemp<iEventLen;jTemp++)
	{
		pstTemp->ppEventMatrix[iEventLen][jTemp] = pstEpi2->ppEventMatrix[iEventLen-1][jTemp];
		pstTemp->ppEventMatrix[jTemp][iEventLen] = pstEpi2->ppEventMatrix[jTemp][iEventLen-1];
	}
	pstTemp->ppEventMatrix[iEventLen][iEventLen] =0;
	return pstTemp;
}

int freqEpisodes::existSubEpisode(alpha *pstFreqEpisodeList,int *paiEvent,bool **ppaiEventMatrix,int iLen)
{
	int iTemp =0,jTemp =0;
	int iCount =0;
	int izTemp =0;
	alpha *pstFreqEpisodeListIter = pstFreqEpisodeList;

	for(;pstFreqEpisodeListIter!= NULL;pstFreqEpisodeListIter = pstFreqEpisodeListIter->pNext)
	{
		izTemp++;
	}

 
	while(pstFreqEpisodeList!= NULL) 
	{
		iCount =0;	
		izTemp++;
		for(iTemp = iLen-1; iTemp >=0  ;iTemp--)
		{
			if(paiEvent[iTemp] != pstFreqEpisodeList->paiEvents[iTemp])
			{
				break;
			}
			iCount++;
		}
		if(iCount == iLen)
		{
			for(iTemp = iLen-1;iTemp >=0  ;iTemp--)
			{
				for(jTemp = iLen-1;jTemp >=0  ;jTemp--)
				{
					if(ppaiEventMatrix[iTemp][jTemp] != pstFreqEpisodeList->ppEventMatrix[iTemp][jTemp])
					{
						goto LABEL;
					}
				}

			}
			return 0;			
		}
LABEL :		
		pstFreqEpisodeList = pstFreqEpisodeList->pNext;

	}

		
	
	return -1;
}
int freqEpisodes::addCandidateEpisode(stEpisodeGel& Head, alpha *pstNode, alpha **pstBlockStart,
						 const int iFrequency)
{
	

	alpha *pstIter = NULL;
	int iEventLen =Head.iEventLength;
	int iTemp =0,jTemp =0;

	

	if(MAX_LONGINT == Head.iEpisodeListLength)
	{
		cout<<"\nCandidate Epiosdes of Length : "<<Head.iEventLength <<" exceeds MAX"<<endl;
		return -1;
	}

	pstNode->iFrequency = iFrequency;
	if(NULL == *pstBlockStart)
	{
		*pstBlockStart = pstNode;
	}
	pstNode->pstBlockStart = *pstBlockStart;	


	Head.iEpisodeListLength++;
	
	if(NULL == Head.ptrAlpha)
	{
		Head.ptrAlpha = pstNode;
		Head.pstTail = pstNode;
		return 0;
	}
	Head.pstTail->pNext = pstNode;
	Head.pstTail = pstNode;	
	return 0;	
}

void freqEpisodes::filter()
{
	int flag = 0;int chk = 0;
	int iTemp,jTemp,eLen;
	
	alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
	alpha *pstPrev1=NULL, *pstPrev2=NULL;
	alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
	for(int headIter = 0;headIter < GelHeads.size();headIter++)
	{
		eLen = GelHeads[headIter].iEventLength;
		pstPrev1 = NULL;chk =0;
		for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ;pstEpisodeIter1 = pstIter1Next)
		{
			pstIter1Next = pstEpisodeIter1->pNext;
			pstPrev2 = NULL;flag=0;
			for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ;pstEpisodeIter2 = pstIter2Next)
                	{
                		flag = 0;
                		pstIter2Next = pstEpisodeIter2->pNext;
				if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen))
				{
					flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif					
					
					if(flag == 1) // to remove the structure pointed by pstEpisodeIter2. 		
					{
						if(pstPrev2 == NULL) 
							{GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;}
						else
							{pstPrev2->pNext = pstEpisodeIter2->pNext;}
						pstEpisodeIter2->deleteMat(eLen); chk++;
						delete pstEpisodeIter2;
						GelHeads[headIter].iEpisodeListLength--;
					}
					else if(flag == -1) // remove the structure pointed by iter1
					{
						pstPrev1->pNext = pstEpisodeIter1->pNext;
						pstEpisodeIter1->deleteMat(eLen); delete pstEpisodeIter1;
						GelHeads[headIter].iEpisodeListLength--;chk++;
						break;
					}
				}
				if(flag == 0) // wen the event types are different
				{
					pstPrev2 = pstEpisodeIter2;
				}
                	}
                	if(flag != -1) 
                		{pstPrev1 = pstEpisodeIter1 ;}	
		}
		
		printf("\nNo. of episodes removed at level-%d = %d\n",eLen,chk);
	}
}

void freqEpisodes::filter(int siz)
{
	int flag = 0;int chk = 0;
	int iTemp,jTemp,eLen;
	alpha *pstEpisodeIter1 = NULL, *pstEpisodeIter2 = NULL;
	alpha *pstPrev1=NULL, *pstPrev2=NULL;
	alpha *pstIter1Next=NULL, *pstIter2Next=NULL;
	//for(int headIter = 0;headIter < GelHeads.size();headIter++)
	//{
		int headIter = siz;
		eLen = GelHeads[headIter].iEventLength;
		pstPrev1 = NULL;chk =0;
		for(pstEpisodeIter1 = GelHeads[headIter].ptrAlpha ; pstEpisodeIter1 != NULL ;pstEpisodeIter1 = pstIter1Next)
		{
			pstIter1Next = pstEpisodeIter1->pNext;
			pstPrev2 = NULL;flag=0;
			for(pstEpisodeIter2 = GelHeads[headIter].ptrAlpha; pstEpisodeIter2 != pstEpisodeIter1 ;pstEpisodeIter2 = pstIter2Next)
                	{
                		flag = 0;
                		pstIter2Next = pstEpisodeIter2->pNext;
				if(compare_event(pstEpisodeIter1->paiEvents,pstEpisodeIter2->paiEvents,eLen))
				{
					flag = compare_containment( (pstEpisodeIter1->ppEventMatrix), (pstEpisodeIter2->ppEventMatrix),eLen);
#if 0
#endif					
					
					if(flag == 1) // to remove the structure pointed by pstEpisodeIter2. 		
					{
						if(pstPrev2 == NULL) 
							{GelHeads[headIter].ptrAlpha = pstEpisodeIter2->pNext;}
						else
							{pstPrev2->pNext = pstEpisodeIter2->pNext;}
						pstEpisodeIter2->deleteMat(eLen); chk++;
						delete pstEpisodeIter2;
						GelHeads[headIter].iEpisodeListLength--;
					}
					else if(flag == -1) // remove the structure pointed by iter1
					{
						pstPrev1->pNext = pstEpisodeIter1->pNext;
						pstEpisodeIter1->deleteMat(eLen); delete pstEpisodeIter1;
						GelHeads[headIter].iEpisodeListLength--;chk++;
						break;
					}
				}
				if(flag == 0) // wen the event types are different
				{
					pstPrev2 = pstEpisodeIter2;
				}
                	}
                	if(flag != -1) 
                		{pstPrev1 = pstEpisodeIter1 ;}	
		}
		
		printf("No. of episodes removed at level-%d = %d\n\n",siz+1,chk);
	//}
}


#define MAXLEN 100
void freqEpisodes::parser(string candid, int iEventLength)
{
	FILE *pfCandid = fopen(candid.c_str(),"r");
	//FILE *pfParseOp = fopen("ParserOut","w");
	char line[MAXLEN];
	int iTemp,jTemp=0,kTemp;
	char strEventTemp[26];
	int colonloc;
	alpha* pstEpisodeTemp;
	int i=0,j=0, iEvent = 0;
	int loc_i=-1,loc_j=-1;
	if(pfCandid == NULL)
	{	printf("1459::Error in Reading Input FIle.\n");exit(0); }
	//Initializing the List header
	stEpisodeGel stCandidateHead;
	stCandidateHead.iEventLength = iEventLength;
	alpha* pstEpisodePrev=NULL;printf("iEventLength = %d\n",iEventLength);
	while(fgets(line, MAXLEN , pfCandid))
	{
		i=0;j=0;loc_i=-1;loc_j=-1;iEvent = 0;
		stCandidateHead.iEpisodeListLength++;
		pstEpisodeTemp = new alpha(iEventLength);
		colonloc = strchr(line,':')-line;printf("1475 :: colonloc : %d\n",colonloc);
		for(iTemp=0,jTemp=0;iTemp<colonloc;iTemp++)
		{
			//if(isspace(getline[iTemp]))
			//	jTemp = 0;
			if(line[iTemp] == ',')
			{	strEventTemp[jTemp] = '\0';jTemp=0; pstEpisodeTemp->paiEvents[iEvent] = atoi(strEventTemp); iEvent++; }
			else if(isdigit(line[iTemp]))
			{	strEventTemp[jTemp] = line[iTemp]; jTemp++; }
			else if(!isspace(line[iTemp]))
			{	printf("1485 ::ERROR ::: INvalid Char in the Input\n"); exit(0); }
		}
		strEventTemp[jTemp] = '\0';
		pstEpisodeTemp->paiEvents[iEvent] = atoi(strEventTemp); iEvent++;
		if(iEvent != iEventLength) {	printf("1489 :: ERROR in number of Events\n"); }
		printf("Event Array : ");
		for(iTemp=0;iTemp<iEventLength;iTemp++) 
			printf("%d ",pstEpisodeTemp->paiEvents[iTemp]);
		printf("\n");
		for(iTemp=colonloc+1,jTemp=0;iTemp<strlen(line);iTemp++)
		{	
			if(line[iTemp] == '>')
			{	strEventTemp[jTemp] = '\0'; i = atoi(strEventTemp); jTemp = 0;}
			else if(line[iTemp] == ','||line[iTemp]=='\n')
			{
				strEventTemp[jTemp] = '\0';
				j = atoi(strEventTemp);
				for(jTemp=0;jTemp < iEventLength;jTemp++)
				{
					if(pstEpisodeTemp->paiEvents[jTemp] == i)
					{	loc_i = jTemp; break; }					
				}
				if(jTemp == iEventLength) { printf("1503 :: ERROR : RHS and LHS doesnt MAtch\n"); }
				for(jTemp=0;jTemp<iEventLength;jTemp++)
				{
					if(pstEpisodeTemp->paiEvents[jTemp] == j)
					{ 	loc_j = jTemp; break; }
				}
				if(jTemp == iEventLength) { printf("1509 :: ERROR : RHS and LHS doesnt MAtch\n"); }
				pstEpisodeTemp->ppEventMatrix[loc_i][loc_j] = 1;
				jTemp = 0;
			}
			else if(isdigit(line[iTemp]))
			{	strEventTemp[jTemp] = line[iTemp];jTemp++;}
			else if(isspace(line[iTemp] == ' '))
			//	jTemp = 0;
			;
			else
			{	 printf("1518 :: ERROR ::: Invalid Character in Candidate Representation.\n"); exit(0); }
		}
		for(iTemp=0;iTemp<iEventLength;iTemp++)
			for(jTemp=0;jTemp<iEventLength;jTemp++)
				if(pstEpisodeTemp->ppEventMatrix[iTemp][jTemp] == 1)
					for(kTemp=0;kTemp<iEventLength;kTemp++)
						if(pstEpisodeTemp->ppEventMatrix[jTemp][kTemp] == 1)
							pstEpisodeTemp->ppEventMatrix[iTemp][kTemp] = 1;
		if(stCandidateHead.ptrAlpha == NULL && pstEpisodePrev == NULL)
			stCandidateHead.ptrAlpha = pstEpisodeTemp;
		else
			pstEpisodePrev->pNext = pstEpisodeTemp;
		pstEpisodePrev = pstEpisodeTemp;
	}
	findFreqCandids(stCandidateHead);
	for(alpha* ptrIter = stCandidateHead.ptrAlpha;ptrIter != NULL;ptrIter=ptrIter->pNext)
		ptrIter->display(iEventLength);
	fclose(pfCandid);
}
int main()
{
	double expTime,fudgeFac; int freqTresh,maxEvents,maxSiz,maxDeg;
	double entropyTresh;
	bool parser_flag = false,filter_flag=false;
	char infile[50];
	string filterOut,outfile;
	fstream in("inputfile");
	cout << "\nINPUT PARAMETERS\n"<<endl;
	while(in)
	{
		string str;
		in >> str;
		if(str == "timefile:")
				{in >> infile;cout << "timefile = " << infile << endl; }
		else if(str=="freqTresh:")
				{in >> freqTresh;cout << "frequncyTresh = "<<freqTresh<<endl;}
		else if(str=="entropyTresh:")
				{in >> entropyTresh;cout << "entropyTresh = "<<entropyTresh<<endl;}
		else if(str == "maxDegree:")
				{in >> maxDeg;cout << "maximum degree in an episode = " << maxDeg;}
		else if(str =="maxEvents:")
				{in >> maxEvents;cout << "maxEvents = "<<maxEvents<<endl;}
		else if(str == "maxSize:")
				{in >> maxSiz;cout << "maxSize = "<<maxSiz<<endl;}
		else if(str =="expTime:")
				{in >> expTime;cout << "expiry time = "<<expTime<<endl;}
		else if(str == "outfile:")
				{in >> outfile;	filterOut = outfile;filterOut+="_fil"; cout << "output file = "<<filterOut<<endl;}
		else if(str == "parser:")
				{string temp; in >> temp; if(temp == "yes") parser_flag = true;}
		else if(str == "fudgeFactor:")
				{in >> fudgeFac;cout << "Fudge Factore = "<<fudgeFac<<endl;}
		else if(str == "filter:")
				{string temp; in >> temp; if(temp == "yes") filter_flag = true;}
		else
			cout << "" << endl<<endl;
	}
	freqEpisodes theOne(expTime,freqTresh,entropyTresh,maxDeg,fudgeFac,maxSiz,maxEvents,infile);
	if(parser_flag == false)
	{
		theOne.findFreqEpisodes();
		theOne.displayFreqEpisodes((char*)outfile.c_str());
		if(filter_flag == true)
		{
			theOne.filter();
			theOne.displayFreqEpisodes((char*)filterOut.c_str());
		}
	}
		
	else
	{
		theOne.parser("candidates",maxSiz);
	}
	
}

