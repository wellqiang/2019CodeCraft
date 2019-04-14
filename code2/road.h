#pragma once
#include "MyUtil.h"

using namespace std;

class road;

typedef struct roadNode
{
	int id; //道路Id
	int length;//道路长度
	int speedLimit;//道路最高速度限制
	int laneNum;//车道数目

	int startNode;//道路起点
	int endNode;//终点
	bool isDoubly;//是否双向。0：单向   1：双向

	roadNode(vector<int>& temp)
	{
		int i = 0;
		id = temp[i++];
		length = temp[i++];
		speedLimit = temp[i++];
		laneNum = temp[i++];
		startNode = temp[i++];
		endNode = temp[i++];
		isDoubly = static_cast<bool>(temp[i]);
	}

}roadInfo;

class road
{
public:
	road();
	~road();

	void init(vector<int>& temp);

	roadInfo* info;
};
