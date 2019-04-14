#include "MyUtil.h"

extern vector<car> vcar;
car::car()
{
}


car::~car()
{
}

void car::init(vector<int>& temp)
{
	info = new carInfo(temp);
}

