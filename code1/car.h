#pragma once

#include "MyUtil.h"

using namespace std;

class car;
extern int carIdOffset;

extern int carOverNum;  //调度完成的车辆数量，已到达目的地
extern int waitStartCarNum;   //在车库等待出发的车辆数目

extern int carNum;   //所有车辆的数目

enum carStatus
{
	waitStart,//等待出发状态
	run,//行驶状态
	waitCross,//等待通过路口状态
	wait,   // 等待状态
	stop,   //终止状态
	over,//车辆调度完成
			  //被前方车辆堵住（待加入）
};

typedef struct carNode
{
	int id;
	int start;//车联出发地点和目的地
	int end;
	int speedLimit;//车辆自身最高速度限制
	int timePlan; //计划出发时间

	int speedCurrent;//车辆当前速度
	int timeStart; //实际出发时间
	int timeRun;//车辆总运行时间

	carStatus status; //车辆当前状态
	int index;//车辆当前位置
	vector<int> throughRoadId;//车辆经过的道路ID

	//车辆计划经过的道路id     暂时没有初始化，可以使用floyd算法，计算一个初始路径
	queue<int> planThroughRoadId;  
	//当前行驶道路和车道（待加入）

	carNode(vector<int>& temp)
	{
		int i = 0;
		id = temp[i++];
		start = temp[i++];
		end = temp[i++];
		speedLimit = temp[i++];
		timePlan = temp[i++];
	}
}carInfo;

class car
{
public:
	car();
	~car();

	void init(vector<int>& temp);


	void go();

	//void setStatu(carStatus statu);//更新车辆状态
	//void setSpeed(int speed);//改变车辆当前速度，根据行驶规则、前车速度和位置
	
	carInfo get();//返回车辆信息
	carInfo* info;//车辆信息
};
