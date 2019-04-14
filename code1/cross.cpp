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
	sort(info->roads.begin(), info->roads.end());
	sort(info->garage.begin(), info->garage.end());
}

/*
处理路口
*/

void cross::runCross()
{
	int nextRoad = 0;
	int roadId = 0;
	for (int j = 0; j < 4; j++)
	{
		roadId = info->roads[j];
		if (roadId > 0)
		{
			shared_ptr<road> roadPtr = make_shared<road>(vroad[roadId - roadIdOffset]);
			shared_ptr<vector<vector<shared_ptr<car>>>> tempRoadPtr;
			//如果这个路口是某条路的出路,且是正向分支的,找到路口的车判断其是否是等待通过路口状态
			if (roadPtr->info->endNode == info->id)
			{
				tempRoadPtr = make_shared<vector<vector<shared_ptr<car>>>>(roadPtr->carOneRoadPositive);
			}
			//如果这个路口是某条路的出路，是反向分支的
			else if (roadPtr->info->startNode == info->id && roadPtr->info->isDoubly)
			{
				tempRoadPtr = make_shared<vector<vector<shared_ptr<car>>>>(roadPtr->carOneRoadNegative);
			}


			//tempRoadPtr 表示在该路口要处理通过路口的某条道路的正向或反向车道
			int laneNum = tempRoadPtr->size();
			shared_ptr<vector<shared_ptr<car>>> carsLanePtr;    //指向某个车道上的车的指针
			for (int i = 1; i < laneNum; i++)
			{

				carsLanePtr = make_shared<vector<shared_ptr<car>>>(tempRoadPtr->at(i));
				int carNum = carsLanePtr->size();
				if (carNum > 0)
				{
					for (int k = 0; k < carNum; k++)
					{
						if (carsLanePtr->at(k)->info->status == carStatus::waitCross)
						{

						
						}
					}
				}

			}
		}
	}
}

/*
处理在车库中的车，让其上路
*/
void cross::runGarageCaratCross(int currentTime)
{
	int carNum = info->garage.size();   //当前路口车库的车辆数目
	int carId = 0;
	
	if (carNum > 0)
	{
		for (int i = 0; i < carNum; i++)
		{
			
			carId = info->garage[i];
			//如果到达该车的出发时刻,加入出发队列
			if (vcar[carId-carIdOffset].info->start >= currentTime)
			{
				info->readyGo.push_back(carId);
			}
			
		}

		//有需要出发的车辆，排序按车辆id升序出发
		if (info->readyGo.size() > 0)
		{
			int nextRoad = 0;
			sort(info->readyGo.begin(), info->readyGo.end());
			for (int j = 0; j < info->readyGo.size(); j++)
			{
				/*
				出发时按照规划好的路线或者随机选择的路线出发
				*/
				nextRoad = vcar[info->readyGo[j] - carIdOffset].info->planThroughRoadId.front();
				/*
				判断要加入的车道是否有位置可以加入，有则加入，
				并去掉计划路线中的该路段，
				在实际行驶路线中加入该路段
				*/
				if (vroad[nextRoad - roadIdOffset].canJoin(info->readyGo[j], info->id))
				{

					vcar[info->readyGo[j] - carIdOffset].info->planThroughRoadId.pop();
					vcar[info->readyGo[j] - carIdOffset].info->throughRoadId.push_back(nextRoad);
					waitStartCarNum--;
				}
			}
		}
	}
}
