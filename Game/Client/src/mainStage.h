#ifndef __MAIN_STAGE_H__
#define __MAIN_STAGE_H__

#include "stage.h"
#include "stdafx.h"
class MainStage : public Stage
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