#pragma once

#include "MyUtil.h"

class cross;

extern int crossIdOffset;

extern int roadIdOffset;

typedef struct crossNode
{
	int id;//路口ID
	vector<int> roads;//此路口所连接的道路ID    从0计数
	vector<int> garage;//此路口车库车辆ID     从0计数

	vector<int> readyGo; //到达时间可以出发的车辆，可以选择出发或者不出发

	crossNode(vector<int>& temp)
	{
		
		id = temp[0];
		for (int i = 0; i < 4; i++)
			roads.push_back(temp[i+1]);
	}
}crossInfo;

class cross
{
public:
	cross();
	~cross();

	void init(vector<int>& temp);

	void runCross();//处理所有路口的车，然后处理路口所连接道路的车

	void runGarageCaratCross(int currentTime);//调度车库中的车，让车库中的车上路

	crossInfo* info;

	
};
