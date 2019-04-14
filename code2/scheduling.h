#pragma once

#include "MyUtil.h"

class Floyd
{
public:
	double** twoWayMap;
	double** wayAttributes;
    double** weights0;
    double** weights1;
    double** weights2;
	vector<vector<passRoad>> vergetoEdge;

	Floyd(int r, int c);
	~Floyd();

	//init weight map
	void creatMap();

	//Dijstra
	vector<passRoad> searchPath(vector<int> &pprev, int v, int u);
	void Dijkstra(int n, int v, vector<double> &dist, vector<int> &pprev, double** weights);
	vector<passRoad> getGijkstraPath(int startId, int endId, double** weights);
    double getMinPathLengh(int startId, int endId, double** weights);

	void updataWeights(vector<passRoad> path, int carSpeed, double** weights);

	void scheduling(string ansPath, vector<car> &vcar_copyTemp, double** weights0, double** weights1, int bach);

	void initDirection(cross &curCross);
    void classificationCarCore(vector<car> &carClassPri, vector<car> &carClass1, vector<car> &carClass2,
                               vector<car> &carClassSlow, double** weights0, double** weights1);
    void initClassWeight(int carType, double** weights);

private:
	int row;
	int col;
	double Q;
	double DecRoad;

};




