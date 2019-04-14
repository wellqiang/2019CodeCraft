#include "MyUtil.h"

road::road()
{
}

road::~road()
{

}

void road::init(vector<int>& temp)
{
	info = new roadInfo(temp);

}
