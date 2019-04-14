#pragma once
#include "MyUtil.h"

class road;

extern int roadIdOffset;

extern int carOverNum;  //调度完成的车辆数量，已到达目的地
extern int waitStartCarNum;   //在车库等待出发的车辆数目

extern int carNum;   //所有车辆的数目

typedef struct roadNode
{
	int id; //道路Id
	int length;//道路长度
	int speedLimit;//道路最高速度限制
	int laneNum;//车道数目

	int startNode;//道路起点
	int endNode;//终点
	bool isDoubly;//是否双向。0：单向   1：双向

	int roadCarNum;//路上车辆的数目

	roadNode(vector<int>& temp)
	{
		int i = 0;
		id = temp[i++];
		length = temp[i++];
		speedLimit = temp[i++];
		laneNum = temp[i++];
		startNode = temp[i++];
		endNode = temp[i++];
		isDoubly = temp[i++];
		roadCarNum = 0;
	}

}roadInfo;

class road
{
public:
	road();
	~road();

	void init(vector<int>& temp);
	
	roadInfo get();//返回道路信息
	void runonRoad();//车辆行驶

	//同向行驶的道路，上按车道处理同一车道的车辆， temp为某车道上全部的车辆
	void runByRoadRule(vector<shared_ptr<car>>& temp);

	void flushStatus();

	//判断能否加入该路段
	bool canJoin(int carId, int crossId);

	roadInfo* info;

	//智能指针不初始化时为空
	shared_ptr<car> oneCarPtr;//指向一辆车
	vector<shared_ptr<car>> carsinLane;//一个车道上的车辆

	/*
	
	
	*/

	vector<vector<shared_ptr<car>>> carOneRoadPositive;
	vector<vector<shared_ptr<car>>> carOneRoadNegative;
	
};
