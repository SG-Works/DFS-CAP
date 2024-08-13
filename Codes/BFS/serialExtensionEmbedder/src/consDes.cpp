#include"super.h"
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

pattern::~pattern()
{
	for(int i=0;i<size;i++)
		adjMat[i].clear();
	adjMat.clear();
}
