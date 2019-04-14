#include "Floyd.h"
#include <algorithm>
#include <stack>
#include <queue>
#include <cmath>

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;

//ÓÉIDµ½Êý×éÏÂ±êµÄÓ³Éä
extern map<int, int> roadIdtoIndex;
extern map<int, int> crossIdtoIndex;
extern map<int, int> carIdtoIndex;

int outCarOneTime ;//Ò»¸öÊ±¼äÆ¬³ö¿â³µÁ¾

const double Q = 200;
const double DecRoad = 0.92;


//¹¹Ôìº¯Êý
Floyd::Floyd(int r, int c)
{
	row = r;
	col = c;

	twoWayMap = new double* [row + 1];
	wayAttributes = new double*[row + 1];
	for (int i = 0; i <= row; i++) 
	{
		twoWayMap[i] = new double[col + 1];
		wayAttributes[i] = new double[col + 1];
	}

	for (int i = 0; i <= row; i++)
		for (int j = 0; j <= col; j++) {
			twoWayMap[i][j] = 99999999.0;
			wayAttributes[i][j] = 99999999.0;
		}
}

//Îö¹¹º¯Êý
Floyd::~Floyd()
{
	for (int i = 0; i <= row; i++)
	{
		delete[] twoWayMap[i];
		delete[] wayAttributes[i];
	}

	delete[] twoWayMap;
	delete[] wayAttributes;
}

//¹¹½¨Ë«ÏòÍ¼
void Floyd::creatMap()
{
	double max = 0.0;
	double min = 99999999.0;
	for (int i = 1; i < vroad.size(); i++)
	{
		road roadTemp(vroad[i]);
		twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] = \
		(double)roadTemp.info->length / (double)roadTemp.info->laneNum;
		max = max > twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] ? max : twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]];
		min = min < twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] ? min : twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]];

		//wayAttributes[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] = twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]];
		if(roadTemp.info->isDoubly)
		{
			twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] = \
			(double)roadTemp.info->length / (double)roadTemp.info->laneNum;
			max = max > twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] ? max : twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]];
			min = min < twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] ? min : twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]];
			//wayAttributes[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] = twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]];
		}
	}
	//¶ÔÔ­Ê¼Êý¾Ý½øÐÐ¹éÒ»»¯
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= row; j++)
		{
			if (twoWayMap[i][j] != 99999999.0)
			{
				twoWayMap[i][j] = ((twoWayMap[i][j] - min) / (max - min)) * 10.0 + 10;
				wayAttributes[i][j] = twoWayMap[i][j];
			}
		}
	}
}


//³õÊ¼»¯¸÷±ßÈ¨ÖØ
void Floyd::initWeights()
{

}

//¸üÐÂ¸÷±ßÈ¨ÖØ
void Floyd::updataWeights(vector<passRoad> path, int carSpeed)
{
	for (auto pathTemp : path)
	{
		double ave = Q / ((double)vroad[roadIdtoIndex[pathTemp.roadId]].info->length * (double)vroad[roadIdtoIndex[pathTemp.roadId]].info->laneNum);
		if (pathTemp.isPositive)//ÕýÏò
		{
			twoWayMap[crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->startNode]][crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->endNode]] += ave;

		}
		else
		{
			twoWayMap[crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->endNode]][crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->startNode]] += ave;
		}
	}
}

void Floyd::getPath(queue<int> &pathNode, int res, int des, int **dist)
{
	if (dist[res][des] != -1)
	{
		int mid = dist[res][des];
		getPath(pathNode, res, mid, dist);
		//pathNode.push_back(mid);
		getPath(pathNode, mid, des, dist);
	}
	else
	{
		pathNode.push(des);
	}
}


vector<passRoad> Floyd::getShortestPathCore(int startId, int endId, int **dist)
{
	vector<passRoad> res;
	queue<int> pathNode; //cross id
	int s = crossIdtoIndex[startId];//将id转换为坐标
	int e = crossIdtoIndex[endId];

	pathNode.push(s);
	getPath(pathNode, s, e, dist);

	int start = pathNode.front();
	pathNode.pop();

	while (!pathNode.empty())
	{
		int end = pathNode.front();
		pathNode.pop();
		//找到道路
		
		for (auto roadT : (vcross[start].info->roads))
		{
			if (roadT != -1)
			{
				if (vroad[roadIdtoIndex[roadT]].info->startNode == vcross[start].info->id && vroad[roadIdtoIndex[roadT]].info->endNode == vcross[end].info->id)//正向行驶
				{
					passRoad temp;
					temp.roadId = vroad[roadIdtoIndex[roadT]].info->id;
					temp.isPositive = true;
					res.push_back(temp);
				}
				if (vroad[roadIdtoIndex[roadT]].info->startNode == vcross[end].info->id && vroad[roadIdtoIndex[roadT]].info->endNode == vcross[start].info->id && vroad[roadIdtoIndex[roadT]].info->isDoubly)//反向行驶，必须保证是双向车道
				{
					passRoad temp;
					temp.roadId = vroad[roadIdtoIndex[roadT]].info->id;
					temp.isPositive = false;
					res.push_back(temp);
				}
			}
		}
		start = end;
	}
	for (int i = 0; i <= row; i++)
	{
		delete[] dist[i];
	}

	delete[] dist;
	return res;
}

//È·¶¨Á½µã¼äµÄ×î¶ÌÂ·¾¶
vector<passRoad> Floyd::getShortestPath(int startId, int endId, int carSpeedLimit)
{
	//Â·¾¶Í¼
	int** dist = new int*[row + 1];
	for (int i = 0; i <= row; i++)
		dist[i] = new int[col + 1];
	//ÁÚ½Ó¾ØÕó
	double** A = new double*[row + 1];
	for (int i = 0; i <= row; i++)
		A[i] = new double[col + 1];
	//³õÊ¼»¯Â·¾¶Í¼¡¢ÁÚ½Ó¾ØÕó
	for (int i = 0; i <= row; i++)
		for (int j = 0; j <= col; j++)
		{
			dist[i][j] = -1;
			A[i][j] = twoWayMap[i][j];
		}

	//¸üÐÂ×î¶ÌÂ·¾¶±í
	for (int k = 1; k <= row; k++)
		for (int i = 1; i <= row; i++)
			for (int j = 1; j <= row; j++)
			{
				if (A[i][j] > (A[i][k] + A[k][j]))
				{
					A[i][j] = A[i][k] + A[k][j];
					dist[i][j] = k;
				}
			}
	for (int i = 0; i <= row; i++)
	{
		delete[] A[i];
	}

	delete[] A;
	return getShortestPathCore(startId, endId, dist);
}

bool comTime(const car& a, const car& b)//´ÎÊýÒ»¶¨ÒªÒýÓÃ
{
	return a.info->timePlan < b.info->timePlan;
}

bool comSpeed(const car& a, const car& b)
{
	return a.info->speedLimit > b.info->speedLimit;
}

//¶Ô³µÁ¾½øÐÐµ÷¶È
//¶Ô³µÁŸœøÐÐµ÷¶È
void Floyd::scheduling(string ansPath)
{
	vector<car> vcar_copyTemp(vcar.size() - 1);
	//¶Ô³µÁŸ°ŽÕÕ³ö·¢Ê±ŒäÅÅÐò
	copy(++vcar.begin(), vcar.end(), vcar_copyTemp.begin());
	stable_sort(vcar_copyTemp.begin(), vcar_copyTemp.end(), comTime);
	stable_sort(vcar_copyTemp.begin(), vcar_copyTemp.end(), comSpeed);

	queue<car> vcar_copy;
	for(auto vcarTemp:vcar_copyTemp)
		vcar_copy.push(vcarTemp);

	ofstream answers(ansPath,ios::out);
	if (!answers)
		cout << "open file failed!" << endl;

	//ÖžÏòÐèÒª³ö¿âµÄ³µ
	int T = 1;
	bool over = true;
	while (over)
	{
		//Ê¹µÃÈšÖØ»Ó·¢
		for (int m = 1; m <= row; m++) {
			for (int n = 1; n <= row; n++)
				if (twoWayMap[m][n] != 99999999.0) {
					twoWayMap[m][n] = wayAttributes[m][n] + (twoWayMap[m][n] - wayAttributes[m][n]) * DecRoad;
				}
		}

		if (T < 300)outCarOneTime = 50;
		else if(T<500) outCarOneTime = 45;
		else if(T<1000) outCarOneTime = 48;
		else outCarOneTime = 50;
	
		for (int i = 0; i < outCarOneTime; i++)
		{
			car schedulCar = vcar_copy.front();
			if (schedulCar.info->timePlan > T)
			{
				T++;
				cout << "skip !!!"<< endl;
				break;
			}
			vcar_copy.pop();
			//¶ÔÒ»žöÊ±ŒäÆ¬ÄÚµ÷¶ÈµÄ³µÁŸœøÐÐµ÷¶È
			vector<passRoad>pathTemp(getShortestPath(schedulCar.info->start, schedulCar.info->end, schedulCar.info->speedLimit));
			answers << "(";
			answers << schedulCar.info->id << ", ";
			answers << T;
			for (auto path : pathTemp)
			{
				answers << ", " << path.roadId;
			}
			answers << ")";
			answers << endl;

			updataWeights(pathTemp,schedulCar.info->speedLimit);
			if(vcar_copy.empty())
			{
				over = false;
				break;
			}
		}
		T++;
		//cout << T << endl;
	}
}