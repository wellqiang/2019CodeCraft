#pragma once

#include "MyUtil.h"

using namespace std;

class car;

typedef struct passroad {
    int roadId;
    bool isPositive;//1????????0??????
}passRoad;


typedef struct carNode
{
	int id;
	int start;//车联出发地点和目的地
	int end;
	int speedLimit;//车辆自身最高速度限制
	int timePlan; //计划出发时间

	bool priority;
	bool preset;

    vector<passRoad> throughRoad;//车辆经过的道路ID

	carNode(vector<int>& temp)
	{
		int i = 0;
		id = temp[i++];
		start = temp[i++];
		end = temp[i++];
		speedLimit = temp[i++];
		timePlan = temp[i++];
		priority = static_cast<bool>(temp[i++]);
		preset = static_cast<bool>(temp[i]);
	}
}carInfo;

class car
{
public:
	car();
	~car();

	void init(vector<int>& temp);

	carInfo* info;//车辆信息

};
