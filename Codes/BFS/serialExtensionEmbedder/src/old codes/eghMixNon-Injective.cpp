#include"super.h"

#define MAXLEN 100
// loadPattern - loads the pattern from the input format into paiEvents and ppEventmatrix format.
void loadPattern(pattern* p,string lines,int size)
{
	cout << "loading patterns.... " <<endl;
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
	printf("Event Array : ");
	for(iTemp=0;iTemp<size;iTemp++) 
		printf("%d ",p->events[iTemp]);
	printf("\n");
	for(iTemp=colonloc+1,jTemp=0;iTemp<lines.size();iTemp++)
	{	
		if(line[iTemp] == '>')
		{	strEventTemp[jTemp] = '\0'; i = atoi(strEventTemp); jTemp = 0;}
		else if(line[iTemp] == ','||line[iTemp]=='\n')
		{
			strEventTemp[jTemp] = '\0';
			j = atoi(strEventTemp);
			if(i <= size && j <= size)
				p->adjMat[i-1][j-1] = 1;
			else
				cout << "Error :: Input of partial order" << endl;
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



void getSerialXtensions(pattern* p,deque< vector<int> > &Q,int size)
{
	//p->display();
	vector<int> init;
	Q.push_back(init);
	//at every pass one event is added to the elements in the queue.
	//So after 'size' passes we get the entire serial extension.
	for(int pass=0;pass<size;pass++)
	{
		int len = Q.size();
		cout << len << endl;
		for(int k=0;k<len;k++)
		{
			//The top of queue is dumped to 'temp'.
			vector<int> temp = Q[0];
			Q.pop_front();
			bool q[size],w[size];
			//'q' is a vector to represent the current seen events.
			//'w' is a vector to represent the possible events that can be added.
			for(int i=0;i<size;i++)
			{	
				q[i] = false;w[i]=false;
			}
			//Updating 'q' to events seen in temp.
			//Note 'q' is indexed by node number. (v1, v2 , v3...)
			for(int i=0;i<size;i++)
			{
				for(int j=0;j<temp.size();j++)
				{
					if(i == temp[j])
					{
						q[i] = true;
						break;
					}
				}
			}
			//we have now got q
			//cout<<"q = ";displayVec(q,size);
			//The following loop calculates the 'w' based on the adjacent matrix.
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
			// cout<<"w = ";displayVec(w,size);
			// Adding each one of the waiting events to 'temp'.
			for(int i=0;i<size;i++)
			{
				if(w[i] == true)
				{
					Q.push_back(temp);
					Q.back().push_back(i);
				}
			}
		}
	}
	//The following loop maps the node number to event types.
	for(int m=0;m<Q.size();m++)
	{
		for(int n=0;n<Q[m].size();n++)
		{
			int idx = Q[m][n];
			Q[m][n] = p->events[idx];
		}
	}
	
}

void generateStream(vector<deque<vector<int> > > Q,parameters param)
{
	cout << "Parameters :"<<endl;
	cout << "Rho = "<<param.rho<<endl;
	cout << "p = " <<param.p<<endl;
	cout << "eta = "<<param.eta<<endl;
	//deltaTee :time resolution used - do not change.
	double deltaTee = .001;
	cout<<"Number of Embedded patterns = "<<Q.size()<<endl;
	// stream.txt - data file.
	fstream fout;
	fout.open("stream.txt",ios::out);
	vector<int> interRand,episodeEvents;
	vector<bool> flags;
	for(int i=0;i<param.numNeu;i++)
		flags.push_back(false);
	for(int j=0;j<Q.size();j++)
	{
		for(int k=0;k<Q[j][0].size();k++)
		{
			int event = Q[j][0][k];
			flags[event-1] = true;
		}
	}
	for(int i=0;i<flags.size();i++)
	{
		if(flags[i] == false)
			interRand.push_back(i+1);
		else
			episodeEvents.push_back(i+1);
	}
#if 1
	vector<bool> mode;
	vector<int> nos;
	vector<int> serial;
	vector<int> epicount;
	vector<int> length;
	vector<int> count;
	for(int i = 0;i<Q.size();i++)
	{
		mode.push_back(false);
		nos.push_back(Q[i].size());
		length.push_back(Q[i][0].size());
		serial.push_back(0);
		epicount.push_back(0);
		count.push_back(0);
	}
	vector<int> outEvents;
     	int noiseEvents = 0;
    	int signalEvents = 0;
	for(double i=0;i<=param.simTime;i=i+deltaTee)
	{
		for(int j=0;j<Q.size();j++)
		{
	
			if(mode[j] == true)
			{
				if(myRand(0,1) >= param.eta)
				{
					//fout<<Q[j][serial[j]][count[j]]<<","<<i<<endl;
					int eve = Q[j][serial[j]][count[j]];
					outEvents.push_back(eve);
                    			signalEvents++;
					count[j] = count[j]+1;
					if(count[j] == length[j]) 
					{
						count[j] =0;
						mode[j] =false;
					}		
				}
			}
			if(mode[j] == false && myRand(0,1) < param.p) 
			{
				epicount[j]=epicount[j]+1;
				mode[j] = true;
				serial[j] = myRandInt(0,nos[j]-1);
				count[j] = 0;
				int eve = Q[j][serial[j]][count[j]];
				outEvents.push_back(eve);
                		signalEvents++;
				count[j] = count[j]+1;

			}
		}
#if 0
		if(myRand(0,1) < param.rho)
		{
			int idx = myRandInt(0,interRand.size()-1);
			outEvents.push_back(interRand[idx]);
		}	
#endif
		for(int j = 0 ; j<interRand.size();j++)
		{
			if(myRand(0,1) < param.rho)
            		{
				outEvents.push_back(interRand[j]);
                		noiseEvents++;
            		}
		}
		for(int j = 0; j < episodeEvents.size();j++)
		{
			if(myRand(0,1) < param.rho/float(5))
            		{
				outEvents.push_back(episodeEvents[j]);
               			noiseEvents++;
            		}
		}
		random_shuffle(outEvents.begin(),outEvents.end());
		for(int k=0;k<outEvents.size();k++)
			fout<<outEvents[k]<<","<<i<<endl;
		outEvents.clear();
	}
#endif
	fout.close();
    	cout << "Number of noise events = " << noiseEvents << endl;
    	cout << "Number of signal events = " << signalEvents << endl;
    	cout << "THe noise ratio is = " <<  float(noiseEvents)/(noiseEvents+signalEvents) << endl;
}

int main(int argc,char* argv[])
{
	parameters param;
	if(argc != 6)
	{
		cout << "usage :: serialEmbedd.exe <rho> <p> <eta> <number of neurons> <simulation time> " << endl;
		exit(0);
	}
	param.rho = atof(argv[1]);
	param.p = atof(argv[2]);
	param.eta = atof(argv[3]);
	param.numNeu = atoi(argv[4]);
	param.simTime = atof(argv[5]);
	srand(time(0));
	int nop =0;
	fstream pf,fout;
	//'patterns' where the partial orders to be embedded are entered.
	pf.open("patterns",ios::in);
	// 'patternsTransClose' is a file (hidden from user) to store the trasitively complete
	// representation of the p.o.'s. 
	fout.open("patternsTransClose",ios::out);
	// 'Q' contains all the serial extension of p.o.'s.
	vector<deque<vector<int> > > Q;
	// 'nop' - number of partial orders embedded.
	pf >> nop;
	fout << nop << endl;
	for(int n=0;n<nop;n++)
	{
		deque< vector<int> > t;
		int siz;
		pf >> siz;
		fout << siz << endl;
		pattern* p = new pattern(siz);
		string line;
		pf>>line;
		loadPattern(p,line,siz);
		displayTransClose(fout,p);
		getSerialXtensions(p,t,siz);
		cout << "pattern-"<<n+1<< " loaded."<<endl;
		cout << "The serial extension of pattern-"<<n+1<< " are:" << endl;
		displayQ(t);
		Q.push_back(t);
		delete p;
	}	
	generateStream(Q,param);
	fout.close();
	pf.close();
}







