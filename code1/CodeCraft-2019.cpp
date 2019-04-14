
// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//
#include "MyUtil.h"
#include "Floyd.h"
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
vector<vector<int>> answers;  

vector<cross> vcross;
vector<road> vroad;
vector<car> vcar;

/*对应数组的偏移量，
使用vcar vcross, vroad时
用实际编号-对应偏移量，即可得到对应编号车辆在数组中的存放位置

如车辆编号10001，carIdOffset为10000
10001-10000=1 即为10001号车辆在vcar中的存放位置

*/
int crossIdOffset = 0;
int roadIdOffset = 500;
int carIdOffset = 10000;

int carOverNum = 0;  //调度完成的车辆数量，已到达目的地
int waitStartCarNum = 0;   //在车库等待出发的车辆数目

int carNum = 0;   //所有车辆的数目

//int carOnRoad = 0;   //在道路上的车数目 ,可以不用，在程序中使用所有车辆数目-调度完成的-在车库中的


void initData(string carPath,string crossPath,string roadPath);
void readText(string path, vector<vector<int>>& temp);



int main(int argc, char *argv[])
{

	std::cout << "Begin" << std::endl;
	
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;
	
	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file
	
	
	//读文件并初始化对象
	initData(carPath,crossPath,roadPath);
	//初始化ID到数组下边的映射数组
	initIdMap();

	Floyd floyd(vcross.size()-1, vcross.size() - 1);
	
	floyd.creatMap();

	floyd.scheduling(answerPath);

    return 0;
}


/*道路行驶规则, 返回当前的可行驶的实际行驶速度，考虑因素
1. 道路和车的限速比较，
2. 该车前方的车辆是否影响到后车的速度
*/
//int roadRule(int carId, int roadId)
//{
//	int speedV = vcar[carId + carIdOffset].info->speedLimit;
//	int roadLimitV = vroad[roadId + roadIdOffset].info->speedLimit;
//
//	return speedV;
//}

void initData(string carPath,string crossPath,string roadPath)
{
	//保留第一个位置，从1计数
	vcar.resize(1);
	vroad.resize(1);
	vcross.resize(1);

	readText(carPath, cars);
	readText(crossPath, crosses);
	readText(roadPath, roads);
	//readText("answer.txt", answers);

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

		//先不考虑id不连续问题， 把车停进车库
		//vcross[cars[i][1]].info->garage.push_back(cars[i][0]);
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
