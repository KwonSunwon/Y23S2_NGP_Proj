#ifndef __EASY_STAGE_H__
#define __EASY_STAGE_H__

#include "stage.h"
#include "stdafx.h"
#include "player.h"
class EasyStage : public Stage
{
protected:
public:
    void init();
    void update();
    void handleEvent(unsigned char, bool);
    void draw();
    void out();

public:
    vector<Player*> otherPlayers;
};

#endif