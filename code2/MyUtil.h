#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <algorithm>

#include "car.h"
#include "cross.h"
#include "road.h"


using namespace std;





class MyUtil
{
public:
	MyUtil();
	~MyUtil();
};

void initIdMap();

void answerAnalyze(vector<passRoad> &answer);
void carNumAnalyze(vector<car> &cars);
void roadAnalyze(vector<road> &roads);


