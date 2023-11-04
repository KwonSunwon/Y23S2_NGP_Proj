#pragma once
#include "gameWorld.h"
GameWorld::GameWorld()
{
}

void GameWorld::set_shader(GLuint shaderProgramID)
{
	this->shaderProgramID = shaderProgramID;
}

void GameWorld::draw_all()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->render(shaderProgramID);
	}

	
}

void GameWorld::add_object(Object* o)
{
	objects.push_back(o);
}

void GameWorld::update_all()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->update();
	}
}

void GameWorld::del_object(int id)
{
	//cout << objects.size();
	for (int i = 0; i < objects.size(); ++i)
	{
		if (id == objects[i]->getId())
		{
			objects.erase(objects.begin() + i);
			//cout << "delete: " << id << endl;
			return;
		}
	}
}

void GameWorld::del_objects()
{
	objects.clear();
}

vector<Object*> GameWorld::get_objects() { return objects; };
