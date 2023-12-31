#ifndef __STAGE_H__
#define __STAGE_H__

#include "stdafx.h"
using namespace std;

class Object;

typedef class Stage
{
protected:
	vector<Object*> walls;
	unsigned short wallIndex = 0;

	int timer = 0;
	float patternTime = 0;

	short m_seed = 0;
public:
	virtual void init() {};
	virtual void update() {};
	virtual void handleEvent(unsigned char, bool) {};
	virtual void handleEvent(int, int, int, int) {};
	virtual void draw() {};
	virtual void out() {};
	void makePattern(int);
} Stage;

#endif