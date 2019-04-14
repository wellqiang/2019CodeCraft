#pragma once

#include "MyUtil.h"
using namespace std;

class cross;

typedef struct crossNode
{
	int id;//路口ID
	vector<int> roads;//此路口所连接的道路ID    从0计数
	int direction[4];

	crossNode(vector<int>& temp)
	{
		id = temp[0];
		for (int i = 0; i < 4; i++)
			roads.push_back(temp[i+1]);
	}
}crossInfo;

class cross
{
public:
	cross();
	~cross();

	void init(vector<int>& temp);

	crossInfo* info;

};
