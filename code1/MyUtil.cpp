#include "MyUtil.h"

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;

//由ID到数组下标的映射
map<int, int> roadIdtoIndex;
map<int, int> crossIdtoIndex;
map<int, int> carIdtoIndex;

MyUtil::MyUtil()
{
}


MyUtil::~MyUtil()
{
}

void initIdMap()
{
	//初始化roadIdtoIndex（由roadID查询此id在数组phero中的下标）
	for (int i = 1; i < vroad.size(); i++)
	{
		roadIdtoIndex[vroad[i].info->id] = i;
	}
	for (int i = 1; i < vcar.size(); i++)
	{
		carIdtoIndex[vcar[i].info->id] = i;
	}
	for (int i = 1; i < vcross.size(); i++)
	{
		crossIdtoIndex[vcross[i].info->id] = i;
	}
}





