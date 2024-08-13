#include"super.h"
int main(int argc,char* argv[])
{
	if(argc != 6) 
	{	cout<<"usage : miner.exe <datastream> <freqTresh> <entropyTresh> <expiryTime> <output file>"<<endl;exit(0);}
	double expTime,fudgeFac=1.0, rho; int freqTresh,maxEvents,maxSiz,maxLength,maxWidth,NoTimeTicks;
	double entropyTresh;
	bool parser_flag = false,filter_flag=false,Lmax_flag=false;
	char infile[200];
	int episodeType =0;
	string filterOut,outfile;
	fstream in("inputfile");
	cout << "\nINPUT PARAMETERS\n"<<endl;
	strcpy(infile,argv[1]);
	cout << "argv[1] "  << argv[1] << endl;
	cout << "infile after copying from argv[1]: " << infile << endl;
    freqTresh = atoi(argv[2]); cout << "FrequencyTresh = "<<freqTresh<<endl;
    //freqUpper = atoi(argv[3]); cout << "Upper Cut off = " << freqUpper << endl;
    entropyTresh = atof(argv[3]); cout <<"Entropy Tresh = "<<entropyTresh<<endl;
    expTime = atof(argv[4]); cout << "Expiry Time = "<<expTime<<endl;
    outfile += argv[5]; cout << "Output File = " << outfile << endl;
    filterOut = outfile;
    filterOut += "_fil"; cout << "Output with maximality filtering = " << filterOut << endl;
	while(in)
	{
		string str;
		in >> str;
		//if(str == "timefile:")
		//		{in >> infile;cout << "timefile = " << infile << endl; }
		//if(str=="freqTresh:")
		//		{in >> freqTresh;cout << "frequncyTresh = "<<freqTresh<<endl;}
		//else if(str=="entropyTresh:")
		//		{in >> entropyTresh;cout << "entropyTresh = "<<entropyTresh<<endl;}
		if(str == "rho:")
				{in >> rho ; cout << "rho or one node frequency of each event = " << rho << endl;}
		if(str == "NoTimeTicks:")
				{in >> NoTimeTicks; cout << "No of TimeTicks = " << NoTimeTicks << endl;}
		else if(str == "maxLengthSize:")
				{in >> maxLength;cout << "maximum degree in an episode = " << maxLength<<endl;}
		else if(str == "numMaxPaths:")
				{in >> maxWidth; cout << "maximum number of paths = " << maxWidth << endl;}
		else if(str =="maxEvents:")
				{in >> maxEvents;cout << "maxEvents = "<<maxEvents<<endl;}
		else if(str == "maxSize:")
				{in >> maxSiz;cout << "maxSize = "<<maxSiz<<endl;}
		//else if(str =="expTime:")
		//		{in >> expTime;cout << "expiry time = "<<expTime<<endl;}
		else if(str == "LmaxFlag:")
				{string temp; in >> temp; if(temp=="true") Lmax_flag=true;
          string output; if(Lmax_flag == true) output = "yes"; else output = "no";
          cout <<"Trimming candidates based on L_max and N_max " << output <<endl;}
		else if(str == "parser:")
				{string temp; in >> temp; if(temp == "yes") parser_flag = true;}
		else if(str == "fudgeFactor:")
				{in >> fudgeFac;cout << "Fudge Factore = "<<fudgeFac<<endl;}
		else if(str == "filter:")
				{string temp; in >> temp; if(temp == "yes") filter_flag = true;}
		else if(str =="episodeType:")
		{
			string temp; in >> temp;
			if(temp == "parallel") episodeType = 1;
			else if(temp == "serial") episodeType = 2;
		}
		else
			;
	}
	cout<<endl<<endl;
	// expTime = expiry time 
	// maxLength = L_max
	// maxWidth = N_max
	// maxSiz = maximun level of mining
	// maxEvents = total number of events [1,2,... , maxEvents]
	// episodeType = serial(2) / parallel(1) / general(0)
	cout << "infile: " << infile << endl;
	freqEpisodes theOne(expTime,freqTresh,entropyTresh,maxLength,maxWidth,fudgeFac,maxSiz,maxEvents,infile,outfile,episodeType,Lmax_flag,NoTimeTicks, rho);
	// parser_flag == false implies full apriori counting
	if(parser_flag == false)
	{
		// the foll func is defined in countingNew.cpp or counting.cpp
		theOne.findFreqEpisodes();
		//theOne.displayFreqEpisodes((char*)outfile.c_str());
		if(filter_flag == true)
		{
			theOne.filter();
			theOne.displayFreqEpisodes((char*)filterOut.c_str());
		}
	}
	// counting only the list of candidates in the text file "candidates".	
	else
	{
		theOne.parser("candidates");
	} // can pass a flag variable to decide whether to do statistical threshold calculation.
	cout << "Done" << endl;
}

