#include "iostream"
#include "MyUtil.h"
#include "scheduling.h"

/*
读取的
car.txt
cross.txt
road.txt
answer.txt
*/
vector<vector<int>> cars;
vector<vector<int>> crosses;
vector<vector<int>> roads;
vector<vector<int>> presetAnswers;

/*
 * 存放车辆信息的数组
 *
 */
vector<cross> vcross;
vector<road> vroad;
vector<car> vcar;

map<int, vector<int>>timetoPresetCarId;
set<int> carSpeed;

extern map<int, int> roadIdtoIndex;
extern map<int, int> crossIdtoIndex;
extern map<int, int> carIdtoIndex;

void initData(string carPath, string crossPath, string roadPath, string presetAnswerPath);
void readText(string path, vector<vector<int>>& temp);
void readPresetAnswer(string path, vector<vector<int>>& temp);

int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 6){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string presetAnswerPath(argv[4]);
	std::string answerPath(argv[5]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "presetAnswerPath is " << presetAnswerPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	
    //读文件并初始化对象
    initData(carPath, crossPath, roadPath, presetAnswerPath);

    cout << "read file over !" << endl;
    Floyd floyd((int)vcross.size() - 1, (int)vcross.size() - 1);

    for(int i = 0;i < 4;i++)
    {
        if(vcross[1].info->roads[i] != -1) {
            vcross[1].info->direction[i] = 0;
            break;
        }
    }

    floyd.initDirection(vcross[1]);

    floyd.creatMap();

    vector<car> carClass1;
    vector<car> carClass2;
    vector<car> carClassPri;
    vector<car> carClassSlow;

    floyd.initClassWeight(0,floyd.weights0);
    floyd.initClassWeight(2,floyd.weights1);

    floyd.classificationCarCore(carClassPri , carClass1, carClass2, carClassSlow, floyd.weights0, floyd.weights1);

    cout << "classification over !" << endl;
    int i = 0;

    floyd.scheduling(answerPath, carClassPri, floyd.wayAttributes, floyd.twoWayMap, ++i);
    floyd.scheduling(answerPath, carClassSlow, floyd.twoWayMap, floyd.weights2, ++i);

    floyd.scheduling(answerPath, carClass1, floyd.weights2, floyd.weights0, ++i);
    floyd.scheduling(answerPath, carClass2, floyd.weights0, floyd.weights1, ++i);
    

    return 0;
}

void initData(string carPath, string crossPath, string roadPath, string presetAnswerPath)
{
    //保留第一个位置，从1计数
    vcar.resize(1);
    vroad.resize(1);
    vcross.resize(1);

    readText(carPath, cars);
    readText(crossPath, crosses);
    readText(roadPath, roads);
    readPresetAnswer(presetAnswerPath, presetAnswers);

    for (int i = 0; i < roads.size(); i++)
    {
        road r;
        r.init(roads[i]);
        vroad.push_back(r);
    }

    for (int i = 0; i < crosses.size(); i++)
    {
        cross c;
        c.init(crosses[i]);
        vcross.push_back(c);
    }

    for (int i = 0; i < cars.size(); i++)
    {
        car c;
        c.init(cars[i]);
        vcar.push_back(c);
        carSpeed.insert(c.info->speedLimit);
    }
    //初始化ID到数组下边的映射数组
    initIdMap();

    //加载预先设置的车辆的行车路径
    for(auto temp : presetAnswers)
    {
        int indexCar = carIdtoIndex[temp[0]];

        int nowCross, nextCross;

        nowCross = vcar[indexCar].info->start;

        timetoPresetCarId[temp[1]].push_back(temp[0]);

        for(int i=2; i<temp.size(); ++i)
        {
            passRoad p;
            p.roadId = temp[i];
            nextCross = vroad[roadIdtoIndex[p.roadId]].info->startNode;

            p.isPositive = (nowCross == nextCross);

            vcar[indexCar].info->throughRoad.push_back(p);

            if(p.isPositive)
                nowCross=vroad[roadIdtoIndex[p.roadId]].info->endNode;
            else
                nowCross=vroad[roadIdtoIndex[p.roadId]].info->startNode;
        }
    }
}


void readText(string path, vector<vector<int>>& temp)
{
    ifstream infile;   //输入流

    infile.open(path, ios::in);
    if (!infile.is_open())
        cout << "Open file failure" << endl;
    string line;
    int num = 0;
    while (getline(infile, line))
    {
        if (line[0] != '#')
        {
            //去掉没用的字符
            line.erase(remove(line.begin(), line.end(), '('), line.end());
            line.erase(remove(line.begin(), line.end(), ','), line.end());
            line.erase(remove(line.begin(), line.end(), ')'), line.end());

            //从字符串读取数字
            int x;
            stringstream ss(line); //建立stringstream对象，初始化流内容为line所代表的字符串

            vector<int> v;
            while (ss >> x)            //从line中一次读取数字存入x
            {
                v.push_back(x);
            }

            temp.push_back(v);
            //计算车辆数目,从0计数
            num++;
        }
    }
    infile.close();   //关闭文件
}

void readPresetAnswer(string path, vector<vector<int>>& temp)
{
    ifstream infile;   //输入流

    infile.open(path, ios::in);
    if (!infile.is_open())
        cout << "Open file failure" << endl;
    string line;
    string templine;
    int num = 0;
    while (getline(infile, templine))
    {
        line = "";
        if (templine[0] != '#')
        {

            for(int i=0; i<templine.size(); i++)
            {
                line+=templine[i];
                if(templine[i]==',' && templine[i+1]!=' ')
                {
                    line+=' ';
                }
            }

            //去掉没用的字符
            line.erase(remove(line.begin(), line.end(), '('), line.end());
            line.erase(remove(line.begin(), line.end(), ','), line.end());
            line.erase(remove(line.begin(), line.end(), ')'), line.end());

            //从字符串读取数字
            int x;
            stringstream ss(line); //建立stringstream对象，初始化流内容为line所代表的字符串

            vector<int> v;
            while (ss >> x)            //从line中一次读取数字存入x
            {
                v.push_back(x);
            }

            temp.push_back(v);
            //计算车辆数目,从0计数
            num++;
        }
    }
    infile.close();   //关闭文件
}
