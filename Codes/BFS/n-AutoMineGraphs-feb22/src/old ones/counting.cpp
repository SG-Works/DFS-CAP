#include"super.h"
void freqEpisodes::findFreqEpisodes(void)
{
	fstream fout;
	fout.open("freqEpisodes",ios::out);
	stEpisodeGel oneCandid;
	oneCandid.iEventLength = 1;
	initCandidates(oneCandid);
	GelHeads.push_back(oneCandid);
	findFreqCandids(GelHeads[0]);
	displayLevelWise(fout,0);
	for(int i=1;i<maxSiz;i++)
	{
		stEpisodeGel tempGelHead;
		tempGelHead.iEventLength = i+1;
		cout << "Generating "<<i+1<<"-Node Candidates..."<<endl;
		generateCandidates(GelHeads[i-1],tempGelHead);
		GelHeads.push_back(tempGelHead);
		findFreqCandids(GelHeads[i]);
		displayLevelWise(fout,i);
	}
	fout.close();
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
	for(alpha* ptrIter1 = GelHead.ptrAlpha;ptrIter1 != NULL;ptrIter1=ptrIter1->pNext)
	{
		bool wPrev[eLen];
		// intializing the waits list -- finding the wait events ..
		initW(wPrev,ptrIter1->ppEventMatrix,eLen);
		ptrIter1->automataList = NULL;
		ptrIter1->iFrequency = 0;
		for(int i=0;i<eLen;i++)
		{
			if(wPrev[i] == true)
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
	}
	int Event;
	while((Event = evntStream.getnextevent()) != -1)
	{
		if(Event > maxEvents) errorAbort("Wrong Event");
		double time = evntStream.getTime();
		//setting all the flags to zero - why ??? - if an automata completes a count 
		//on seeing event 'e' it shud not make new transitions further. 
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
					{
						if(autoIter->betaAddress[i])
						{
							if(autoIter->betaAddress[i] == betaIterNext) betaIterNext=betaIterNext->pNext;
							relinkDelBeta(waitsList[ptrAlpha->paiEvents[i]-1],autoIter->betaAddress[i]);
							delete autoIter->betaAddress[i];	
						}
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
		alphaIter->entropy = fcuk;
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
				alphaIter->display(out,len);
		}
	}
	out.close();
}

void freqEpisodes::displayLevelWise(fstream& out,int siz)
{
	int len = GelHeads[siz].iEventLength;
	out << len << "-NOde Episodes = "<<GelHeads[siz].iEpisodeListLength<<endl;
	for(alpha* alphaIter = GelHeads[siz].ptrAlpha;alphaIter!=NULL;alphaIter=alphaIter->pNext)
	{
		alphaIter->display(out,len);
	}
}
