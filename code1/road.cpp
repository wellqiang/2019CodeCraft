#include "MyUtil.h"

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;
road::road()
{
}


road::~road()
{

}

void road::init(vector<int>& temp)
{
	info = new roadInfo(temp);
	/*
	temp[6]表示是否双向
	temp[3]表示车道数目
	*/
	if (temp[6])
	{
		carOneRoadNegative.resize(1);
	}
	carOneRoadPositive.resize(1);

	vector<shared_ptr<car>> v;
	v.resize(1);

	for (int i = 0; i < temp[3]; i++)
	{
		carOneRoadNegative.push_back(v);
		carOneRoadPositive.push_back(v);
	}
}

/*
处理路上的车辆
包括路口对应的路上的

*/
void road::runonRoad()
{
	
	
	
		
}



/*
第一步处理所有路上的车

*/
void road::runByRoadRule(vector<shared_ptr<car>>& temp)
{
	shared_ptr<car> index, front;
	/*
	shared_ptr 用法
	index = make_shared<car>(vcar[carId])
	
	*/

	int carNum = temp.size();
	if (carNum > 1)
	{
		for (int i = 1; i < carNum; i++)
		{

		}

	}
	

}


/*
在新的一个时间片开始时刷新所有车的状态为可行驶状态
*/
void road::flushStatus()
{
	for (int j = 1; j < info->laneNum; j++)
	{
		
		for (int k = 1; k < carOneRoadNegative[j].size(); k++)
		{
			if (carStatus::stop == carOneRoadNegative[j][k]->info->status)
				carOneRoadNegative[j][k]->info->status = carStatus::run;
		}

		for (int k = 1; k < carOneRoadPositive[j].size(); k++)
		{
			if (carStatus::stop == carOneRoadPositive[j][k]->info->status)
				carOneRoadPositive[j][k]->info->status = carStatus::run;
		}
	}
}

bool road::canJoin(int carId, int crossId)
{
	bool positiveFlag = false;
	if (crossId == info->startNode)
	{
		positiveFlag = true;
	}
	int carNum;
	for (int i = 1; i < info->laneNum; i++)
	{
		if (positiveFlag)
		{
			carNum = carOneRoadPositive[i].size();
			if (carOneRoadPositive[i][carNum]->info->index > 1)
			{
				
				oneCarPtr = make_shared<car>(vcar[carId]);
				carOneRoadPositive[i].push_back(oneCarPtr);
				return true;
			}
			carOneRoadPositive[i];
		}
		else
		{
			carsinLane = carOneRoadPositive[i];
		}
		//如果道路起点没车就可以加入
		
			
	}
	return false;
}


