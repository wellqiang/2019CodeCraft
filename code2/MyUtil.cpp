#include "MyUtil.h"
//#include "Floyd.h"

extern vector<cross> vcross;
extern vector<road> vroad;
extern vector<car> vcar;

//由ID到数组下标的映射
map<int, int> roadIdtoIndex;
map<int, int> crossIdtoIndex;
map<int, int> carIdtoIndex;


MyUtil::MyUtil()
{
}


MyUtil::~MyUtil()
{
}

void initIdMap()
{
	//初始化roadIdtoIndex（由roadID查询此id在数组phero中的下标）
	for (int i = 1; i < vroad.size(); i++)
	{
		roadIdtoIndex[vroad[i].info->id] = i;
	}
	for (int i = 1; i < vcar.size(); i++)
	{
		carIdtoIndex[vcar[i].info->id] = i;
	}
	for (int i = 1; i < vcross.size(); i++)
	{
		crossIdtoIndex[vcross[i].info->id] = i;
	}
}



/*
 * 每条道路上经过多少车
 *
 * 每个楼口过多少车
 *
 *每辆车的answer
 *
 */
vector<int> carNumPerRoad;
int carN=0;

void answerAnalyze(vector<passRoad> &answer)
{
    carNumPerRoad.resize(vroad.size());
    carN++;

    for(auto t: answer)
    {
        carNumPerRoad[roadIdtoIndex[t.roadId]] ++;
    }
    if(carN == vcar.size()-1)
    {
        for(int i=0; i<carNumPerRoad.size(); ++i)
        {
            cout<<"road "<<i<<"  "<<carNumPerRoad[i]<<endl;
        }
    }

}


/*
 * 统计排序后的车辆数组
 * 不同速度有多少辆车
 * 统计发车时间
 *
 *cars为按速度排序后的发车数组
 */

void carNumAnalyze(vector<car> &cars)
{
    int lastV = cars[0].info->speedLimit;
    int num=0;
    int minT= cars[0].info->timePlan;
    int maxT= cars[0].info->timePlan;
    int priorityNum=0;
    int presetNum=0;
    for(auto c : cars)
    {
        if(c.info->speedLimit == lastV)
            num++;
        else
        {
            cout<<"V "<<lastV<<"  "<<num<<endl;
            lastV=c.info->speedLimit;
            num=0;
        }

        if(minT>c.info->timePlan)
            minT=c.info->timePlan;

        if(maxT<c.info->timePlan)
            maxT=c.info->timePlan;
        if(c.info->preset)
            presetNum++;
        if(c.info->priority)
            priorityNum++;

    }
    cout<<"V "<<lastV<<"  "<<num<<endl;

    cout<<"maxTime CAR "<<maxT<<endl;
    cout<<"minTime CAR "<<minT<<endl;

    cout<<"priority CAR num "<<priorityNum<<endl;
    cout<<"preset CAR num "<<presetNum<<endl;
    cout<<"total CAR num "<<cars.size()<<endl;

}


/*
 * 统计道路长度
 * 统计车道数目
 * 统计车道单双向
 *
 * 最低速度，最高速度
 *
 *roads为道路数组
 */

void roadAnalyze(vector<road> &roads)
{
    double averLen=0;
    double averLane=0;
    vector<int> laneNum(10, 0);
    int danxiang=0;
    int sumLen=0;
    int sumLane=0;
    int minV=vroad[1].info->speedLimit;
    int maxV = vroad[1].info->speedLimit;
    for(int i=1; i<roads.size(); ++i)
    {
        sumLen+=vroad[i].info->length;
        sumLane+=vroad[i].info->laneNum;
        laneNum[vroad[i].info->laneNum]++;
        if(vroad[i].info->isDoubly)
            danxiang++;

        if(minV>vroad[i].info->speedLimit)
            minV = vroad[i].info->speedLimit;
        if(maxV<vroad[i].info->speedLimit)
            maxV = vroad[i].info->speedLimit;

    }
    averLane = sumLane/(double)(vroad.size()-1);
    averLen = sumLen / (double)(vroad.size()-1);

    cout<<"road num "<<vroad.size()-1<<endl;
    cout<<"road low V "<<minV<<endl;
    cout<<"road high V "<<maxV<<endl;
    cout<<"road aver LaneNum "<<averLane<<endl;
    cout<<"road aver length "<<averLen<<endl;
    cout<<"danxiang lanenum "<<danxiang<<endl;

}




