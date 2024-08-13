#include"super.h"
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


#define MAXLEN 200
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
	int lineo = 0;
	while(fgets(line, MAXLEN , pfCandid))
	{
		lineo++;
		i=0;j=0;loc_i=-1;loc_j=-1;iEvent = 0;
		stCandidateHead.iEpisodeListLength++;
		pstEpisodeTemp = new alpha(iEventLength);
		colonloc = strchr(line,':')-line;printf("1475 :: colonloc : %d\n",colonloc);
		cout << "lengt of line = " <<strlen(line)<<endl;
		for(iTemp=0,jTemp=0;iTemp<colonloc;iTemp++)
		{
			//if(isspace(getline[iTemp]))
			//	jTemp = 0;
			if(line[iTemp] == ',')
			{	strEventTemp[jTemp] = '\0';jTemp=0; pstEpisodeTemp->paiEvents[iEvent] = atoi(strEventTemp); iEvent++; }
			else if(isdigit(line[iTemp]))
			{	strEventTemp[jTemp] = line[iTemp]; jTemp++; }
			else if(!isspace(line[iTemp]))
			{	printf("1485 ::ERROR ::: INvalid Char in the Input. line number : %d\n",lineo); exit(0); }
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
				//cout<<loc_i<<" " <<loc_j<<endl;
				if(loc_i != -1 && loc_j != -1)
					pstEpisodeTemp->ppEventMatrix[loc_i][loc_j] = 1;
				//cout<<"after..."<<endl;
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
	{
		transred(ptrIter->ppEventMatrix,iEventLength);
		ptrIter->display(iEventLength);
	}
	fclose(pfCandid);
}
