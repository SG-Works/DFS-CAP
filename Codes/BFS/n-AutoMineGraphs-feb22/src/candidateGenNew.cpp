#include"super.h"
#define MAX_LONGINT 10000000
void freqEpisodes::generateCandidates(stEpisodeGel prevHead,stEpisodeGel& Head)
{
	int l = prevHead.iEventLength;
	for(alpha* mainIter = prevHead.ptrAlpha;mainIter!=NULL;mainIter = mainIter->pNext)
	{
		alpha* pstBlockStart = NULL; //addition code starts from the next line. 

		for(alpha* blockIter=mainIter->pPrev;blockIter!=NULL && (blockIter->paiEvents)[l-1] == (mainIter->paiEvents)[l-1] && blockIter->pstBlockStart == mainIter->pstBlockStart;blockIter=blockIter->pPrev)
		{
			alpha* (posEpisodes[3]) = {NULL};
			
			generatePossibleCandidates(mainIter,blockIter,posEpisodes,l);
			// the following for loop performs te sub-episode check
			for(int iTemp=0;iTemp<3 && posEpisodes[iTemp] != NULL;iTemp++)
			{
				// if(( addCandidateEpisode(Head,posEpisodes[iTemp],&pstBlockStart,0)) == -1)
				// 	{
				// 		cout <<" Out of Memory - Higher Threshold"<<endl;
				// 		exit(0);
				// 	}
				bool addFlag = true;
				dummyEpisode* ptrDummy = new dummyEpisode(l);
        alpha* ptrCurrentPosition = blockIter;
				for(int jTemp = l-2; (jTemp>=0) && (addFlag == true);jTemp--)
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
          ptrCurrentPosition = existSubEpisode(ptrCurrentPosition,ptrDummy->paiEvents,ptrDummy->ppEventMatrix,l);
          // if(NULL == ptrCurrentPosition)
					// {
					// 	addFlag = false;
					// }

				}
				// Checking for Lmax and Nmax criteria only if addFlag == true and the check is needed.
				if(true == addFlag && LmaxFlag == true) 
        {
        //   if(lengthMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) > maxLength 
				// || numMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) > maxWidth)
        //     addFlag = false;
        }


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


		for(alpha* blockIter=mainIter;blockIter!=NULL && blockIter->pstBlockStart == mainIter->pstBlockStart;blockIter=blockIter->pNext)
		{
			alpha* (posEpisodes[3]) = {NULL};
			//removed becos we want to combine the po's with same last event also.
			//if(mainIter->paiEvents[l-1] == blockIter->paiEvents[l-1])
			//	continue;
			generatePossibleCandidates(mainIter,blockIter,posEpisodes,l);
			// the following for loop performs te sub-episode check
			for(int iTemp=0;iTemp<3 && posEpisodes[iTemp] != NULL;iTemp++)
			{
				// if(( addCandidateEpisode(Head,posEpisodes[iTemp],&pstBlockStart,0)) == -1)
				// 	{
				// 		cout <<" Out of Memory - Higher Threshold"<<endl;
				// 		exit(0);
				// 	}
				bool addFlag = true;
				dummyEpisode* ptrDummy = new dummyEpisode(l);
        alpha* ptrCurrentPosition = blockIter;
				for(int jTemp = l-2; (jTemp>=0) && (addFlag == true);jTemp--)
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
          ptrCurrentPosition = existSubEpisode(ptrCurrentPosition,ptrDummy->paiEvents,ptrDummy->ppEventMatrix,l);
          // if(NULL == ptrCurrentPosition)
					// {
					// 	addFlag = false;
					// }

				}
				// Checking for Lmax and Nmax criteria only if addFlag == true and the check is needed.
				if(true == addFlag && LmaxFlag == true) 
        {
        //   if(lengthMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) > maxLength 
				// || numMaxPath(posEpisodes[iTemp]->ppEventMatrix,l+1) > maxWidth)
        //     addFlag = false;
        }


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
	bool addFlagY0Y2 = true;
	if (pstEpi1->paiEvents[iEventLen - 1] == pstEpi2->paiEvents[iEventLen - 1])
		addFlagY0Y2 = false;
	int iTemp =0;
	int iDirFlag =0;
	if(episodeType == 1 && addFlagY0Y2 == true)
	{
		pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
		return;
	}
	// the following loop chks if there is a path from E(l-1) to E (l) or the other way
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
		if(1 == iDirFlag)
		{
			pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
			pstTemp[0]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
		}
		else if(addFlagY0Y2 == true)
		{
			pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
			pstTemp[0]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
		}
		return;

	}
	else
	{
		int c = 0;
		if(episodeType != 2 && addFlagY0Y2 == true)
		{
			pstTemp[0] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
			//cout << "Hi" << pstEpi1->paiEvents[iEventLen - 1] << pstEpi2->paiEvents[iEventLen - 1] <<  endl;
			c++;
		}
		
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
						pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
						if(addFlagY0Y2 == true)
						{
							pstTemp[c+1] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
							pstTemp[c+1]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
						}
					}
					break;
			case 1:
					{
						pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
						pstTemp[c]->ppEventMatrix[iEventLen-1][iEventLen] = 1;
					}
					break;
			case 2:
					{
						if(addFlagY0Y2 == true)
						{
							pstTemp[c] = catEpisodes(pstEpi1,pstEpi2,iEventLen);
							pstTemp[c]->ppEventMatrix[iEventLen][iEventLen-1] = 1;
						}
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

freqEpisodes::alpha* freqEpisodes::existSubEpisode(alpha *pstFreqEpisodeList,int *paiEvent,bool **ppaiEventMatrix,int iLen)
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
			return pstFreqEpisodeList;			
		}
LABEL :		
		pstFreqEpisodeList = pstFreqEpisodeList->pNext;

	}

		
	
	return NULL;
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
	pstNode->pPrev = Head.pstTail; //addition for doubly-linked.
	Head.pstTail = pstNode;	
	return 0;	
}
