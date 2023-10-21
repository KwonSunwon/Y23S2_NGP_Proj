#ifndef __NORMAL_STAGE_H__
#define __NORMAL_STAGE_H__

#include "stage.h"
#include "stdafx.h"
class NormalStage : public Stage
{
protected:
public:
    void init();
    void update();
    void handleEvent(unsigned char, bool);
    void draw();
    void out();
};

#endif