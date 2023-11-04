#ifndef __START_STAGE_H__
#define __START_STAGE_H__

#include "stage.h"
#include "stdafx.h"
class StartStage : public Stage
{
protected:
public:
    void init();
    void update();
    void handleEvent(unsigned char, bool);
    void handleEvent(int, int, int, int);
    void draw();
    void out();
};

#endif