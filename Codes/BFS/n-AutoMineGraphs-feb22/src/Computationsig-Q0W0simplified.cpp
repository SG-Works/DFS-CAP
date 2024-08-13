#include"super.h"


using namespace std;

double LogOnePlusX(double x);
double NormalCDFInverse(double p);
double RationalApproximation(double t);
void demo();

// compute log(1+x) without losing precision for small values of x
double LogOnePlusX(double x)
{
    if (x <= -1.0)
    {
    	//std::stringstream os;
        cout << "Invalid input argument (" << x << "); must be greater than -1.0";
        // throw std::invalid_argument( os.str() );
    }

    if (fabs(x) > 1e-4)
    {
           // x is large enough that the obvious evaluation is OK
           return log(1.0 + x);
    }

    // Use Taylor approx. log(1 + x) = x - x^2/2 with error roughly x^3/3
    // Since |x| < 10^-4, |x|^3 < 10^-12, relative error less than 10^-8
    return (-0.5*x + 1.0)*x;
}


double RationalApproximation(double t)
{
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) / (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}


double NormalCDFInverse(double p)
{
    if (p <= 0.0 || p >= 1.0)
        {
	        //std::stringstream os;
		cout << "Invalid input argument (" << p << "); must be larger than 0 but less than 1.";
	        // throw std::invalid_argument( os.str() );
	}

        // See article above for explanation of this section.
       if (p < 0.5)
            {
                    // F^-1(p) = - G^-1(p)
                    return -RationalApproximation( sqrt(-2.0*log(p)) );
            }
      else
      {
          // F^-1(p) = G^-1(1-p)
          return RationalApproximation(sqrt(-2.0*log(1-p)));
      }
}



//Read a pattern  from file 

class state
{
public:
	vector<bool> q;
	vector<bool> w;
	//Depending on the size N of the p.o. read, intialise q and w with size N bool false arrays.
	vector <int> nextstate;// should this be malloced OR  vectorised?
	vector <int> transition_on_node; //transition on node rather.
	// On seeing an event transition-on-event[i] from w(current), we transit to nextstate[i]; For any event not in transition-on-event we remain
	// in the current state.
	// constructor and descructor.
	state(int);
	~state();
};
state::state(int n)
{
	for(int i=0;i<n;i++)
	{
	q.push_back(0);
	w.push_back(0);
	}
}
state::~state()
{
	q.clear();
	w.clear();
	nextstate.clear();
	transition_on_node.clear();
}

//class p  declared.
class pattern
{
public:
	int size;
	//note he is not mallocing.
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

//its constructors and destructors.
pattern::pattern(int n)
{
	size = n;
	for(int i=0;i<n;i++)
	{
		vector<bool> temp;
		adjMat.push_back(temp);
		//this pushback is necessary because temp actually pushes a bool vector each time in the loop.
		//The for loop fills each such vector of bools with actual bool values.
		events.push_back(0);
		for(int j=0;j<n;j++)
			adjMat[i].push_back(0);
	}
}

pattern::~pattern()
{
	//what about the 1Darray events?
	//actually erases the 2D array.
	for(int i=0;i<size;i++)
		adjMat[i].clear();
	adjMat.clear();
}



void loadPattern(pattern* p,string lines,int size)
{
	//lines reads the pattern
	cout << "loading pattern.... " <<endl;
	char* line = new char[lines.size()+1]; // where is it deleted?
	strcpy(line,lines.c_str());
	int iTemp,jTemp=0,kTemp;
	char strEventTemp[26];
	int i=0,j=0, iEvent = 0;
	int loc_i=-1,loc_j=-1;
	int colonloc; //colonloc locates : which indicates the end of the array of events information in the current line
	colonloc = (int)lines.find_first_of(":");
	//VVI strTemp captures a number like 134 by reading 1,3 and 4 separately as characters followed by a null character.
	//This is now fed to the atoi function which extracts the integer 134 from the string 134\0.
	for(iTemp=0,jTemp=0;iTemp<colonloc;iTemp++)
	{
		if(line[iTemp] == ',')
		{	strEventTemp[jTemp] = '\0';jTemp=0; p->events[iEvent] = atoi(strEventTemp); iEvent++; }
		else if(isdigit(line[iTemp]))
		{	strEventTemp[jTemp] = line[iTemp]; jTemp++; }
		else if(!isspace(line[iTemp]))
		{	printf("1485 ::ERROR ::: INvalid Char in the Input\n"); exit(0); }
	}
	//reads the last event of the array
	strEventTemp[jTemp] = '\0';
	p->events[iEvent] = atoi(strEventTemp); iEvent++;
	if(iEvent != size) {	printf("1489 :: ERROR in number of Events\n"); }

	//printf("Event Array : ");
	//for(iTemp=0;iTemp<size;iTemp++) 
	//	printf("%d ",p->events[iTemp]);
	//printf("\n");
	
	//reading the set of edges from the remaining portion of  the line
	for(iTemp=colonloc+1,jTemp=0;iTemp<lines.size();iTemp++)
	{	
		if(line[iTemp] == '>')
		{	strEventTemp[jTemp] = '\0'; i = atoi(strEventTemp); jTemp = 0;}
		//strEventTemp does a similar job as before
		//i captures the integer before '>' and j captures the integer after '>' OR before ','.
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
	//Perform transitive closure
	for(iTemp=0;iTemp<size;iTemp++)
		for(jTemp=0;jTemp<size;jTemp++)
			if(p->adjMat[iTemp][jTemp] == 1)
				for(kTemp=0;kTemp<size;kTemp++)
					if(p->adjMat[jTemp][kTemp] == 1)
						p->adjMat[iTemp][kTemp] = 1;
}

void LoadEventProbabilityArray(vector<double> &EventProbability,int M, bool single)
{
	//lines reads each symbol's probability.
	cout << "loading probabilities.... " <<endl;
	//char* line = new char[M]; // where is it deleted?
	//strcpy(line,lines.c_str());
	string line;
	char strEventNumber[10]; 
	fstream PtrInputProb;
	if(1 == single)
		{ PtrInputProb.open("InputProbabilities-single",ios::in); }
	else
		{ PtrInputProb.open("InputProbabilities-multiple",ios::in); }
	//cout << M << " " << single << " " << "file pointer points to appropriate file" << endl ;
	//Write a loop to read M lines.
	for(int i=0; i<M; i++)
	{
		PtrInputProb >> line;	
		//a small loop to read the symbol and its probabilities.
		int commaloc;
		commaloc = (int)line.find_first_of(",");
		//cout << commaloc << endl;
		int jtemp, itemp;
		for(jtemp = commaloc+1, itemp = 0; jtemp < line.length(); jtemp++, itemp++) //.size() also does the same.
			strEventNumber[itemp] = line[jtemp];
		strEventNumber[itemp] = '\0'; 
		//EventProbability[i] = atof(strEventNumber);  //This is a common mistake you have been doing all the while.
		EventProbability.push_back(atof(strEventNumber));
	}
}



class doubleindex
{
public:
	int start;
	int end;
};



double ComputeSignificanceThreshold(int *alphag, bool **alphae, bool single, int M, int NoTimeTicks, double rho, int siz) 
{
		deque< vector<int> > t;
		//p is a pointer to a pattern.
		pattern* p = new pattern(siz);
		for(int i=0; i<siz; i++)
		{
			p->events[i] = alphag[i]; 
		}

		for(int i=0; i<siz; i++)
		{
			for(int j=0; j<siz; j++)
			{
				p->adjMat[i][j] = alphae[i][j] ;
			}
			cout << endl;
		}
		//copy the alphag and alphae details into pattern p.
		//we have read the partial order episode details into the object p.
		//creating the Finite State automaton followed by the P matrix are the next two major steps.
		//To do this we first create the vector of states (also the FSA) where state class is defined above.
		vector<state> S;
		//Initialize the vector by Q0 and W0;
		state s0(siz);
		//find all those elements whose set of parents is empty and hence initialize s0.w

		cout << " printing the event array of the pattern " << endl;

		for(int i=0; i<siz; i++)
		{
			cout << p->events[i] << " " ;
		}
		cout << endl;
		cout << " printing the adjacency matrix of the pattern " << endl;

		for(int i=0; i<siz; i++)
		{
			for(int j=0; j<siz; j++)
			{
				cout << p->adjMat[i][j]  ;
			}
			cout << endl;
		}
		for(int j=0;j<siz;j++)
		{
			bool flag=true;
			for(int i=0;i<siz;i++)
			{
				if(p->adjMat[i][j]==1)
				{
					flag=false;
					break;	
				}
			}
			//if flag is still true, then all of j's parents are non-existent.
			if(flag==true)
			{
				s0.w[j]=1;
			}
		}
		for(int j=0;j<siz; j++)
		{
			//cout<< s0.w[j] << endl;
		}
		S.push_back(s0);
		//the main loop of constructing the  set of states with Qsize varying from 1 to siz(N) starts.
		//we need 2 indices which keep track of start and end of all Qsize i candidates.
		//storing it in a vector of objects (each obj. containing 2 indices) size siz(N)
		vector<doubleindex> doub_ind_list;
		doubleindex i0;
		i0.start=0;
		i0.end=0;
		doub_ind_list.push_back(i0);
		//the outer loop indicates each level of the automaton graph. A level corresponds to states having the same size Q.
		for(int ilevel=0; ilevel< siz; ilevel++)
		{
			doubleindex ind;
			ind.start=doub_ind_list[ilevel].end + 1;
			ind.end = ind.start - 1;
			//this indicates going through all the states at a given level.
			for(int jlistofstates=doub_ind_list[ilevel].start; jlistofstates<=doub_ind_list[ilevel].end; jlistofstates++)
			{
				if(jlistofstates==doub_ind_list[ilevel].start)
				{
				//then the set of next states generated by the first state at each level can be blindly generated and added to the end of S.
				//compute S[jlistofstates]'s set of next states for each level i. 
				int no_elements_added = 0;
				for(int j=0;j<siz;j++) //this j is essentially over the w vector's ones.
				{
					if(S[jlistofstates].w[j]==1)
					{
						state stemp(siz);
						//copy the q of S[jlistofstates] to stemp's q.
						for(int l=0; l<siz; l++)
							stemp.q[l]=S[jlistofstates].q[l];	
							
						stemp.q[j]=1;	

						//this loop looks at all children i of j
						//computation of wnext ends.
						for(int i=0; i<siz; i++)
						{		
							if(p->adjMat[j][i]==1)
							{
								bool parentseenflag=true;
								for(int k=0;k<siz;k++)
								{
									//if any of its parents are not in q, then dont add this child to w.
									if(p->adjMat[k][i]==1 && stemp.q[k]==0)
									{
										parentseenflag=false;
										break;
									}
								}
								if(parentseenflag==true)
									stemp.w[i] = 1;
							}
						}
						//put all the elements from W0 into stem.w[i] except the one being moved into Q0.
						for(int l=0;l<siz;l++)
						{
							if(S[jlistofstates].w[l]==1)
							{
							stemp.w[l]=S[jlistofstates].w[l];
							}
						}
						stemp.w[j]=0;	
						
						//computation of wnext ends.
						S.push_back(stemp);
						no_elements_added ++;//
						S[jlistofstates].nextstate.push_back(ind.end + no_elements_added);
						S[jlistofstates].transition_on_node.push_back(j);
					}
				}	
					ind.end = ind.end + no_elements_added ;
				}

				else//the jlistofstates is greater than the first element of the current level
				{
				//form the potential Q of the next state. Then move from start of level l+1 till the partial end.
				//i.e. from ind.start to ind.end and verify whether the potential Q is not already created.
				int no_elements_added = 0;
				//go through all elements in Wcur
				for(int j=0;j<siz;j++)
				{
					if(S[jlistofstates].w[j]==1)
					{
						state stemp(siz);
						//copy the q of S[j] to stemp's q.
						for(int l=0; l<siz; l++)
							stemp.q[l]=S[jlistofstates].q[l];	
							
						stemp.q[j]=1;
						//for each 
						bool flagalreadyexists=false;
						int lvalue = -1;
						
						//cout << "ind.start " << ind.start << " ind.end " << ind.end << endl;
						//check whether there already exists an automaton q same as stemp.q .
						//ind.start to ind.end gives you the elements which need to be checked.
						for(int l= ind.start; l<= ind.end; l++)
						{ 
							//compare S[l].q with stemp.q, if found, you just have to fill the
							//next state with the index just found. which should be ind.start+l-1.
							bool flag1=true;
							for(int m=0; m<siz; m++)
							{
								if(S[l].q[m]!=stemp.q[m])
								{
									flag1=false;
									break;
								}
							}
							//flag1==false means the current element of the list of states doesnt match continue the l loop and move
							//to the next element
							//true means one has found the state and hence stop the outer l loop, store the lvalue needed.
							if(flag1==true)
							{
								lvalue = l;
								flagalreadyexists=true;
								break;
							}
						}
						//if the state is found, then just update the nextstate and transition_on_node of the current state.
						if(flagalreadyexists==true)
						{
							S[jlistofstates].nextstate.push_back(lvalue );
							S[jlistofstates].transition_on_node.push_back(j); 
							//dont want to look at g(v_j) which is present in the pattern p's events.
						}
						//else create compute the stemp.w and push stemp into the list S as before.
						else
						{
						for(int i=0; i<siz; i++)
						{		
							if(p->adjMat[j][i]==1)
							{
								bool parentseenflag=true;
								for(int k=0;k<siz;k++)
								{
									//if any of its parents are not in q, then dont add this child to w.
									if(p->adjMat[k][i]==1 && stemp.q[k]==0)
									{
										parentseenflag=false;
										break;
									}
								}
								if(parentseenflag==true)
									stemp.w[i] = 1;
							}
						}
						//put all the elements from W0 into stem.w[i] except the one being moved into Q0.
						for(int l=0;l<siz;l++)
						{
							if(S[jlistofstates].w[l]==1)
							{
							stemp.w[l]=S[jlistofstates].w[l];
							}
						}

						stemp.w[j]=0;	
						S.push_back(stemp);
						no_elements_added ++;//
						S[jlistofstates].nextstate.push_back(ind.end + no_elements_added);
						S[jlistofstates].transition_on_node.push_back(j);		
						//index.ended has to be updated.
						//Also outside this loop doubleindexarray has to updated.
						}
					}
				}
				ind.end = ind.end + no_elements_added ;
			}
		}

			doub_ind_list.push_back(ind);
			//check whether sufficent no. of elements are added.
			//cout << "ind.start " << ind.start << " ind.end " << ind.end << endl;
			//size of S check
			//cout << "size of S " <<  S.size() << endl;
	}

	//prints out the elements of the list of states 
	for(int i=0; i<S.size(); i++)
	{
		cout << "state no. " << i << endl;
		cout << "Q:" << endl;
		for(int j=0; j<siz; j++)
			cout << S[i].q[j] << " " ;
		cout << endl;
		cout << "W:" << endl;
		for(int j=0; j<siz; j++)
			cout << S[i].w[j] << " " ;
		cout << endl;
		if(i< S.size()-1)
		{
		for(int j=0; j<S[i].nextstate.size() ; j++)
			cout << (S[i].nextstate)[j] << " " ;
		cout << endl;
		}
	}

cout << endl;


//Given the set of states S with the information of state transitions, we need to find the  P for datastreams of type 1. straightforward.
//For datastreams of type 2, let us go down S and construct the associated P on the fly.

//just like the events array of the class pattern, we have an array of probabilities.
//Suppose somehow we read the probabilities.

vector<double> EventProbability;//just like the events array can have repeated events, similarly this array can have repeated probabilities.
//EventProbability[i] is the probability of the ith event.
vector< vector<double> > TransitionProbabilityMatrix_single;
vector< vector<double> > TransitionProbabilityMatrix_multiple;

//read the eventprobabilities from inputprobabilities-single OR inputprobabilities-multiple.
fstream PtrInputProb_single;//pattern file pointer.
fstream PtrInputProb_multiple;//pattern file pointer.


//call the array of probabilities loading function 
 //M is the no. of event types.
// this loading happens from either inputprobabilities-single OR inputprobabilities-multiple.
if (0 == single)
{
	cout << "MULTIPLE-EVENT-TYPES-PER-TIMETICK-MODE" << endl;
}
else
{
	cout << "SINGLE-EVENT-TYPE-PER-TIMETICK-MODE" << endl;
}


if(1 == single )
{
//To read the probabilities from a file, use the following function.
//LoadEventProbabilityArray(EventProbability,M, single);

//uniform iid model.
for(int i=0; i<M; i++)
{
	EventProbability[i] = 1/M ;
}

//printing the EventProbability
cout << "printing event probabilities" << endl;
for(int i=0; i<M; i++)
{
	cout << EventProbability[i] << endl;
}
cout << endl;

//Initialise a zero matrix of size S.size().
for(int i=0; i<S.size(); i++)
{
		vector<double> temp;
		TransitionProbabilityMatrix_single.push_back(temp);
		//this pushback is necessary because temp actually pushes a bool vector each time in the loop.
		//The for loop fills each such vector of bools with actual bool values.
		for(int j=0;j<S.size();j++)
			TransitionProbabilityMatrix_single[i].push_back(0.0);
}


for(int j=0; j<S.size()-1; j++) //except final state whose transition matches that of the start state.
{
	double outgoingprobabilities = 0;
	for(int i=0; i<(S[j].nextstate).size(); i++)
	{
		TransitionProbabilityMatrix_single[j][S[j].nextstate[i]] = EventProbability[ p->events[ S[j].transition_on_node[i]] -1 ];
		outgoingprobabilities += 	EventProbability[ p->events[ S[j].transition_on_node[i] ] -1 ];
	}
	TransitionProbabilityMatrix_single[j][j] = 1 - outgoingprobabilities ;
}

for(int i=0; i<S.size(); i++)
TransitionProbabilityMatrix_single[S.size() - 1][i] = TransitionProbabilityMatrix_single[0][i];

//Use this P to solve mu and sigma of the interarrival distributions.
//ouputs the P matrisc
for(int i=0; i<S.size(); i++)
{
	for(int j=0; j<S.size(); j++)
	{
		cout << TransitionProbabilityMatrix_single[i][j] << " " ;
	}
	cout << endl;
}

}

//P construction for multipleevents.
if(0== single )
{
//To read the probabilities from a file, use the following function.
//LoadEventProbabilityArray(EventProbability,M,single);

//uniform i.i.d. model.
for(int i=0; i<M; i++)
{
	EventProbability[i]=rho ;
}
cout << "printing event probabilities" << endl;
for(int i=0; i<M; i++)
{
	cout << EventProbability[i] << endl;
}
cout << endl;
//construct a matrix of this size.
for(int i=0; i<S.size(); i++)
{
		vector<double> temp;
		TransitionProbabilityMatrix_multiple.push_back(temp);
		//this pushback is necessary because temp actually pushes a bool vector each time in the loop.
		//The for loop fills each such vector of bools with actual bool values.
		for(int j=0;j<S.size();j++)
			TransitionProbabilityMatrix_multiple[i].push_back(0.0);
}



for(int jstates=0; jstates<S.size()-1; jstates++) //except final state whose transition matches that of the start state.
{
	int sizeW = (S[jstates].transition_on_node).size();
	cout << "state no. " << jstates << endl;
	cout << "size W_cur " << sizeW << endl;

	vector<bool> waitstates;
	//initialise waitstates to all zeros.
	for(int l=0; l< sizeW ; l++)
		waitstates.push_back(0);

	//1.extract S[j]'s Q
		

	//search for the next state and also increment loop.
	//isubsetsW varies from 0  to 2^{sizeW} - 1.
	for(int isubsetsW=0; isubsetsW < exp2(sizeW); isubsetsW++)
	{
		vector<bool> qtemp;
		int countq = 0;
		//countq gives the no. of elements in Qcur U {subset of Wcur} in the loop next.
		for(int l=0; l< siz; l++) //siz is the size of the episode under consideration.
		{
			qtemp.push_back( S[jstates].q[l] ); // copies the contents of the current q to qtemp.
			if( S[jstates].q[l] == 1)
				countq ++;
		}
		//subset formation comes at the beginning here
		

		//Add the elements in waitsstates to qtemp to form the new Q which has to searched
		//transition probability value.
		double TransitionProbabilityvalue=1;
		//the foll. loop adds the current subset of Wcur to Qcur via addition to qtemp.
		for(int ibitvector=0; ibitvector< sizeW; ibitvector++)
		{
			if(waitstates[ibitvector] == 1)
			{
				qtemp[ (S[jstates].transition_on_node)[ibitvector] ] = 1;
				countq ++;
				TransitionProbabilityvalue = TransitionProbabilityvalue* EventProbability[ p->events[ S[jstates].transition_on_node[ibitvector] ] -1 ];
			}
			else
				TransitionProbabilityvalue = TransitionProbabilityvalue* (1 - EventProbability[ p->events[ S[jstates].transition_on_node[ibitvector] ] -1 ]);
			cout << "TransProbvalueintheloop " << TransitionProbabilityvalue <<  endl ;
		}
		cout << "TransitionProbabilityValue " << TransitionProbabilityvalue << endl;
		//search for this new set of accepted states in doub_ind_list[countq].
		cout << "next states' countq " << countq << endl;
		cout << "waitstates bitvector status" << endl;
		for(int i=0; i<sizeW; i++)
			cout << waitstates[i] << " " ;
		cout << endl;


		int lvalue = -1;
		for(int istateslevel= doub_ind_list[countq].start; istateslevel<=doub_ind_list[countq].end; istateslevel++)
		{
			//compare S[l].q with stemp.q, if found, you just have to fill the
			bool flag1=true;
			for(int m=0; m<siz; m++)
			{
				if(S[istateslevel].q[m]!=qtemp[m])
				{
					flag1=false;
					break;
				}
			}
			//flag1==false means the current element of the list of states doesnt match continue the l loop and move
			//to the next element
			//true means one has found the state and hence stop the outer istateslevel loop, store the lvalue needed.
			if(flag1==true)
			{
				lvalue = istateslevel;
				break;
			}
		}
		if(-1==lvalue )
		cout << "error" <<endl;
		else 
			TransitionProbabilityMatrix_multiple[jstates][lvalue]= TransitionProbabilityvalue;
		
			//compare S[i].q with the vector temp. Use the previous comparison ideas.


		//increment the waitstates boolean vector to generate next subset of the current W
		bool carry=1;
		for(int ibitvector=0; carry==1 && ibitvector < sizeW ; ibitvector++)
		{
	 		if(waitstates[ibitvector] == 0)
			{
				waitstates[ibitvector] = 1;
				carry = 0;
			}
			else
			{	 	
				waitstates[ibitvector] = 0;
				carry = 1;
			}
		}
		qtemp.clear();
	}
}

for(int i=0; i<S.size(); i++)
TransitionProbabilityMatrix_multiple[S.size() - 1][i] = TransitionProbabilityMatrix_multiple[0][i];

for(int i=0; i<S.size(); i++)
{
	for(int j=0; j<S.size(); j++)
	{
		cout << TransitionProbabilityMatrix_multiple[i][j] << " " ;
	}
	cout << endl;
}

cout << " output all row sums " << endl;
for(int i=0; i<S.size(); i++)
{
	double rowsum = 0.0;
	for(int j=0; j<S.size(); j++)
	{
		rowsum+= TransitionProbabilityMatrix_multiple[i][j] ;
	}
	cout << rowsum ;
	cout << endl;
}

}

//till now, we have transition probability matrix over the state space either for (i)single OR (ii)multiple

//Let us solve the mean vector first
//Let S=size of the state space.
//To solve either of these we need the (S-1) X (S-1) submatrix of TransitionProbabilityMatrix.


double mean, variance;

if(0 == single)
{

int P_size = TransitionProbabilityMatrix_multiple.size();
int N = P_size -1;
//P_size is same in both cases.
LaGenMatDouble A(N,N),B(N,N);
LaVectorDouble m1(N),m2(N),b(N),c(N);
//Extract the submatrix
for(int i=0; i<N; i++)
for(int j=0; j<N; j++)
{
	A(i,j)= TransitionProbabilityMatrix_multiple[i][j];
}

for(int i=0; i<N; i++)
for(int j=0; j<N; j++)
{
	B(i,j)= -1*A(i,j);
	if(i==j)
	{
		B(i,j) = 1 + B(i,j);
	}

}

b=1.0;
LaLinearSolve(B,m1,b);
mean = m1(0);
cout << "mean of the waiting time " << mean << endl;

for(int i=0; i<N; i++)
{
	c(i)=0;
	for(int j=0; j<N; j++)
	{
		c(i)=c(i) + A(i,j)*m1(j);
	}
	c(i)=2*c(i);
}

LaLinearSolve(B,m2,c);

variance = m2(0) - pow(m1(0),2) + m1(0);
cout << "variance of the waiting time " << variance << endl;

}

else
{

int P_size = TransitionProbabilityMatrix_single.size();
int N = P_size -1;
//P_size is same in both cases.
LaGenMatDouble A(N,N),B(N,N);
LaVectorDouble m1(N),m2(N),b(N),c(N);
//Extract the submatrix
for(int i=0; i<N; i++)
for(int j=0; j<N; j++)
{
	A(i,j)= TransitionProbabilityMatrix_single[i][j];
}

for(int i=0; i<N; i++)
for(int j=0; j<N; j++)
{
	B(i,j)= -1*A(i,j);
	if(i==j)
	{
		B(i,j) = 1 + B(i,j);
	}

}

b=1.0;
LaLinearSolve(B,m1,b);

mean = m1(0);
cout << "mean of the waiting time " << mean << endl;

for(int i=0; i<N; i++)
{
	c(i)=0;
	for(int j=0; j<N; j++)
	{
		c(i)=c(i) + A(i,j)*m1(j);
	}
	c(i)=2*c(i);
}

LaLinearSolve(B,m2,c);

variance = m2(0) - pow(m1(0),2) + m1(0);
cout << "variance of the waiting time " << variance << endl;

}

//computing the threshold.
//T is a parameter which has to be sent to the significance computation function.
double T;
T = double (NoTimeTicks);
double Type1Error = 0.5;
double FreqThresh = (T/mean) + pow(variance*T,0.5) * pow(mean,-1.5) * NormalCDFInverse(1-Type1Error) ;

return (int (FreqThresh));

//cout << "Frequency-Threshold = " << FreqThresh << endl;



}




