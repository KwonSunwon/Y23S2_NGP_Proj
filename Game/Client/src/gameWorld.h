#ifndef __GAME_WORLD_H__
#define __GAME_WORLD_H__

#include "object.h"
#include "stdafx.h"

class GameWorld
{
private:
    vector<Object *> objects;
    GLuint shaderProgramID;

public:
    GameWorld();
    void draw_all();
    void update_all();
    void add_object(Object *);
    void set_shader(GLuint);
    void del_object(int);
    void del_objects();
    vector<Object*> get_objects();
};

#endif