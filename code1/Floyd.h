#pragma once

#include "MyUtil.h"


typedef struct passroad {
	int roadId;
	bool isPositive;//1代表正向，0代表反向
}passRoad;


class Floyd
{
public:
	double** twoWayMap;
	double** wayAttributes;
	Floyd(int r, int c);
	~Floyd();
	//构建双向图
	void creatMap();

	//初始化各边权重
	void initWeights();

	//更新各边权重
	void updataWeights(vector<passRoad> path, int carSpeed);

	//确定两点间的最短路径
	vector<passRoad> getShortestPathCore(int startId, int endId,int **dist);
	vector<passRoad> getShortestPath(int startId, int endId, int carSpeedLimit);
	void scheduling(string ansPath);
	void getPath(queue<int> &pathNode, int res, int des, int **dist);
private:
	int row;
	int col;
};




