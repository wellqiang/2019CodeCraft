#include "MyUtil.h"

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;
cross::cross()
{
}


cross::~cross()
{
}

void cross::init(vector<int>& temp)
{
	info = new crossInfo(temp);
	//sort(info->roads.begin(), info->roads.end());
	for(int i = 0;i < 4;i ++)
		this->info->direction[i] = -1;
}