#include "scheduling.h"

#include <algorithm>
#include <stack>
#include <queue>
#include <cmath>

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;

extern map<int, int> roadIdtoIndex;
extern map<int, int> crossIdtoIndex;
extern map<int, int> carIdtoIndex;
extern set<int> carSpeed;
extern map<int, vector<int>>timetoPresetCarId;

int outCarOneTime ;

const double maxDouble = 100000.0;

Floyd::Floyd(int r, int c)
{
    Q=150;
    DecRoad = 0.9;
	row = r;
	col = c;

	twoWayMap = new double* [row + 1];
	wayAttributes = new double*[row + 1];

    weights0 = new double* [row + 1];
    weights1 = new double* [row + 1];
    weights2 = new double* [row + 1];
	for (int i = 0; i <= row; i++)
	{
		twoWayMap[i] = new double[col + 1];
		wayAttributes[i] = new double[col + 1];
        weights0[i] = new double[col + 1];
        weights1[i] = new double[col + 1];
        weights2[i] = new double[col + 1];
	}

	for (int i = 0; i <= row; i++)
		for (int j = 0; j <= col; j++) {
			twoWayMap[i][j] = maxDouble;
			wayAttributes[i][j] = maxDouble;
            weights0[i][j] = maxDouble;
            weights1[i][j] = maxDouble;
            weights2[i][j] = maxDouble;
		}

	for(int i=0; i<c+1; ++i)
	{
		vector<passRoad> v(c+1);
		vergetoEdge.push_back(v);
	}

	for(int i=1; i<vroad.size(); ++i)
	{
		passRoad p;
		p.roadId=vroad[i].info->id;
		vergetoEdge[crossIdtoIndex[vroad[i].info->startNode]][crossIdtoIndex[vroad[i].info->endNode]] = p;
		if(vroad[i].info->isDoubly)
			vergetoEdge[crossIdtoIndex[vroad[i].info->endNode]][crossIdtoIndex[vroad[i].info->startNode]] = p;
	}
}


Floyd::~Floyd()
{
	for (int i = 0; i <= row; i++)
	{
		delete[] twoWayMap[i];
		delete[] wayAttributes[i];
        delete[] weights0[i];
        delete[] weights1[i];
        delete[] weights2[i];
	}

	delete[] twoWayMap;
	delete[] wayAttributes;
    delete[] weights0;
    delete[] weights1;
    delete[] weights2;
}


void Floyd::creatMap()
{
	double max = 0.0;
	double min = maxDouble;
	for (int i = 1; i < vroad.size(); i++)
	{
		road roadTemp(vroad[i]);
		twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] = \
		(double)roadTemp.info->length / (double)roadTemp.info->laneNum;
		max = max > twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] ? max : twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]];
		min = min < twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]] ? min : twoWayMap[crossIdtoIndex[roadTemp.info->startNode]][crossIdtoIndex[roadTemp.info->endNode]];

		if(roadTemp.info->isDoubly)
		{
			twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] = \
			(double)roadTemp.info->length / (double)roadTemp.info->laneNum;
			max = max > twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] ? max : twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]];
			min = min < twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]] ? min : twoWayMap[crossIdtoIndex[roadTemp.info->endNode]][crossIdtoIndex[roadTemp.info->startNode]];

		}
	}

	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= row; j++)
		{
			if (twoWayMap[i][j] != maxDouble)
			{
				twoWayMap[i][j] = ((twoWayMap[i][j] - min) / (max - min)) * 10.0 + 10;
				wayAttributes[i][j] = twoWayMap[i][j];
                weights0[i][j] = twoWayMap[i][j];
                weights1[i][j] = twoWayMap[i][j];
                weights2[i][j] = twoWayMap[i][j];
			}
		}
	}
}

int reverseDirection(int direction)
{
    if(direction == 0)
        return  2;
    else if(direction == 1)
        return 3;
    else if(direction == 2)
        return 0;
    else
        return 1;
}

cross initOneDirection(cross preCross, int curDirection, int roadId)
{
    road roadTemp = vroad[roadIdtoIndex[roadId]];

    if(preCross.info->id == roadTemp.info->startNode) {
        cross &curCross = vcross[crossIdtoIndex[roadTemp.info->endNode]];
        for(int i = 0;i < 4;i++)
        {
            if(curCross.info->roads[i] == roadId)
            {
                curCross.info->direction[i] = reverseDirection(curDirection);
                return vcross[crossIdtoIndex[roadTemp.info->endNode]];
            }
        }
    } else {
        cross &curCross = vcross[crossIdtoIndex[roadTemp.info->startNode]];
        for(int i = 0;i < 4;i++)
        {
            if(curCross.info->roads[i] == roadId)
            {
                curCross.info->direction[i] = reverseDirection(curDirection);
                return vcross[crossIdtoIndex[roadTemp.info->startNode]];
            }
        }
    }
}

int directionAdd(int &d)
{
    ++d;
    if(d > 3)
        d = 0;
    return d;
}

void initOneCrossD(cross &curCross, int roadIndex, int dir)
{
    curCross.info->direction[directionAdd(roadIndex)] = directionAdd(dir);

    curCross.info->direction[directionAdd(roadIndex)] = directionAdd(dir);

    curCross.info->direction[directionAdd(roadIndex)] = directionAdd(dir);
}

void Floyd::initDirection(cross &curCross)
{
    if(curCross.info->direction[0] + curCross.info->direction[1] + curCross.info->direction[2] +curCross.info->direction[3] == 6)
        return;
    for(int i = 0;i < 4;i++)
    {
        if(curCross.info->direction[i] != -1 && curCross.info->roads[i] != -1)
        {
            //one direction to all
            initOneCrossD(curCross, i, curCross.info->direction[i]);
            break;
        }
    }

    for(int i = 0;i < 4;i++)
    {
        if(curCross.info->roads[i] != -1)
        {
            cross crossNext = initOneDirection(curCross, curCross.info->direction[i], curCross.info->roads[i]);
            initDirection(crossNext);
        }
    }
}

void Floyd::initClassWeight(int carType, double** weights)
{
    for(int i = 1;i < vcross.size();i++)
    {
        for(int d = 0;d < 4;d++)
        {
            if(vcross[i].info->direction[d] == carType && vcross[i].info->roads[d] != -1)
            {
                road roadTemp = vroad[roadIdtoIndex[vcross[i].info->roads[d]]];
                int x = vcross[i].info->id;
                int y = (x == roadTemp.info->startNode)?roadTemp.info->endNode:roadTemp.info->startNode;
                weights[crossIdtoIndex[x]][crossIdtoIndex[y]] = maxDouble;
                break;
            }
        }
    }
}

//classification the cars
void Floyd::classificationCarCore(vector<car> &carClassPri, vector<car> &carClass1, vector<car> &carClass2, vector<car> &carClassSlow, double** weights0, double** weights1)
{
    for(int i = 1;i < vcar.size();i++)
    {
        car carTemp = vcar[i];

        if(!carTemp.info->preset) {

            if (carTemp.info->priority) {
                carClassPri.push_back(carTemp);
            }
            else {
                //add the slow cars
                set<int>::iterator it = ++carSpeed.begin();
                if(carTemp.info->speedLimit <= *(++it))
                {
                    carClassSlow.push_back(carTemp);
                } 
                else
                {
                    if (getMinPathLengh(carTemp.info->start, carTemp.info->end, weights0) < maxDouble) {
                        carClass1.push_back(carTemp);
                    }
                    else if(getMinPathLengh(carTemp.info->start, carTemp.info->end, weights1) < maxDouble)
                    {
                    	carClass2.push_back(carTemp);
                    }
                    else {
                        carClassSlow.push_back(carTemp);
                    }
                }
            }
        }
    }

    cout << "classPri :" << carClassPri.size() << endl;
    cout << "class1:" << carClass1.size()<<endl;
    cout << "class2:" << carClass2.size()<<endl;
    cout << "classSlow:" << carClassSlow.size() << endl;
}


void Floyd::updataWeights(vector<passRoad> path, int carSpeed, double** weights)
{
	double ave;
	for (auto pathTemp : path)
	{
		ave = Q / ((double)vroad[roadIdtoIndex[pathTemp.roadId]].info->length * (double)vroad[roadIdtoIndex[pathTemp.roadId]].info->laneNum);

        if (pathTemp.isPositive)
		{
            weights[crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->startNode]][crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->endNode]] += ave;
		}
		else
		{
            weights[crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->endNode]][crossIdtoIndex[vroad[roadIdtoIndex[pathTemp.roadId]].info->startNode]] += ave;
		}
	}
}

bool comTime(const car& a, const car& b)//´ÎÊýÒ»¶¨ÒªÒýÓÃ
{
	return a.info->timePlan < b.info->timePlan;
}


bool comSpeed(const car& a, const car& b)
{
	return a.info->speedLimit > b.info->speedLimit;
}


bool comPriority(const car& a, const car& b)
{
    return a.info->priority > b.info->priority;
}

static 	int T = 0;
void Floyd::scheduling(string ansPath, vector<car> &vcar_copyTemp, double** lastWeights,double** weights, int bach)
{
	stable_sort(vcar_copyTemp.begin(), vcar_copyTemp.end(), comTime);
	stable_sort(vcar_copyTemp.begin(), vcar_copyTemp.end(), comSpeed);

	for (int m = 1; m <= row; m++) {
		for (int n = 1; n <= row; n++)
			if (weights[m][n] < maxDouble && lastWeights[m][n] < maxDouble) {
				weights[m][n] = lastWeights[m][n];
			}
	}

	queue<car> vcar_copy;
	for(auto vcarTemp:vcar_copyTemp){
		if(!vcarTemp.info->preset)
			vcar_copy.push(vcarTemp);
	}
    ofstream answers;
    if(bach == 1)
        answers.open(ansPath,ios::out);
    else
	    answers.open(ansPath,ios::app);
	if (!answers)
		cout << "open file failed!" << endl;

	bool over = true;
    int lastT = T;
	while (over)
	{
		for (int m = 1; m <= row; m++) {
			for (int n = 1; n <= row; n++)
				if (weights[m][n] < maxDouble) {
					weights[m][n] = wayAttributes[m][n] + (weights[m][n] - wayAttributes[m][n]) * DecRoad;
                    //cout << bach << " :The weights is :"<< weights[m][n] << endl;
				}
		}

        if(bach == 1)
            outCarOneTime = 18;
        if(bach == 2) {
        	if(T < lastT + 100)
        		outCarOneTime = 18;
        	else
            	outCarOneTime = 30;
        }
        if(bach == 3) {
            outCarOneTime = 35;
        }
        if(bach == 4) {
        	if(T < lastT + 150)
        		outCarOneTime = 35;
        	else
            	outCarOneTime = 45;
        }
        //update weight for preset car in T time
        if(timetoPresetCarId[++T].size())
        {
            for(int i = 0;i < timetoPresetCarId[T].size();++i)
            {
                int carId = timetoPresetCarId[T][i];
                vector<passRoad> pathTemp = vcar[carIdtoIndex[carId]].info->throughRoad;
                updataWeights(pathTemp, vcar[carIdtoIndex[carId]].info->speedLimit, weights);
            }
        }

        outCarOneTime -= timetoPresetCarId[T].size();
        if(outCarOneTime <= 0){
         	T++;
            continue;
        }
		for (int i = 0; i < outCarOneTime; i++)
		{
			car schedulCar = vcar_copy.front();
			if (schedulCar.info->timePlan > T)
			{
				//T++;
				cout << "skip !!!"<< endl;
				break;
			}
			vcar_copy.pop();

            vector<passRoad> pathTemp;

            //使用floyd计算路径
           //pathTemp = getShortestPath(schedulCar.info->start, schedulCar.info->end, schedulCar.info->speedLimit);
            // 使用dijstra计算路径
            pathTemp = getGijkstraPath(schedulCar.info->start, schedulCar.info->end, weights);
            //cout << pathLen <<endl;
            //analyzePath(pathTemp);
            answers << "(";
            answers << schedulCar.info->id << ", ";
            answers << T;
            for (auto path : pathTemp)
            {
                answers << ", " << path.roadId;
            }
            answers << ")";
            answers << endl;

            updataWeights(pathTemp, schedulCar.info->speedLimit, weights);

			if(vcar_copy.empty())
			{
				over = false;
				break;
			}
		}
	}
    answers.close();
}


//v是对应路口id的坐标
void Floyd::Dijkstra(int n, int v, vector<double> &dist, vector<int> &pprev, double** weights)
{
	vector<bool> s(row+1);    // 判断是否已存入该点到S集合中

	for(int i=1; i<=n; ++i)
	{
		dist[i] = weights[v][i];
		s[i] = 0;     // 初始都未用过该点
		if(dist[i] == maxDouble)
			pprev[i] = 0;
		else
			pprev[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;

	// 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
	// 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
	// 注意是从第二个节点开始，第一个为源点
	for(int i=2; i<=n; ++i)
	{
		double tmp = maxDouble;
		int u = v;
		// 找出当前未使用的点j的dist[j]最小值
		for(int j=1; j<=n; ++j)
			if((!s[j]) && dist[j]<tmp)
			{
				u = j;              // u保存当前邻接点中距离最小的点的号码
				tmp = dist[j];
			}
		s[u] = 1;    // 表示u点已存入S集合中

		// 更新dist
		for(int j=1; j<=n; ++j)
			if((!s[j]) && weights[u][j]<maxDouble)
			{
				double newdist = dist[u] + weights[u][j];
				if(newdist < dist[j])
				{
					dist[j] = newdist;
					pprev[j] = u;
				}
			}
	}
}

// 查找从源点v到终点u的路径，并输出，v u是对应路口id的坐标
vector<passRoad> Floyd::searchPath(vector<int> &pprev,int v, int u)
{
	vector<passRoad> temp;

	vector<int> que(row+1);
	int tot = 1;
	que[tot] = u;
	tot++;
	int tmp = pprev[u];
	while(tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = pprev[tmp];
	}
	que[tot] = v;

	for(int i=tot; i>1; --i)
	{
		passRoad p;
		p = vergetoEdge[que[i]][que[i-1]];
		p.isPositive = (que[i] == vroad[roadIdtoIndex[p.roadId]].info->startNode);
		temp.push_back(p);
	}
	return temp;
}

vector<passRoad> Floyd::getGijkstraPath(int startId, int endId, double** weights)
{
	// 各数组都从下标1开始
	vector<double> dist(row+1);     // 表示当前点到源点的最短路径长度
	vector<int> pprev(row+1);     // 记录当前点的前一个结点

	int v=crossIdtoIndex[startId];
	int u = crossIdtoIndex[endId];

	Dijkstra(row, v, dist, pprev, weights);

    if(dist[u] >= maxDouble) {
        Dijkstra(row, v, dist, pprev, Floyd::twoWayMap);
    }
	return searchPath(pprev, v, u);
}

double Floyd::getMinPathLengh(int startId, int endId, double** weights)
{
    // 各数组都从下标1开始
    vector<double> dist(row+1);     // 表示当前点到源点的最短路径长度
    vector<int> pprev(row+1);     // 记录当前点的前一个结点

    int v=crossIdtoIndex[startId];
    int u = crossIdtoIndex[endId];

    Dijkstra(row, v, dist, pprev, weights);
    return dist[u];
}
