#ifndef BOX_H
#define BOX_H

#include "Object3d.h"


class Box : public Object3d
{

public:

	Box();
	Box(AudioEffect* effect);
	Box(AudioEffect* effect, int strength);
	~Box();

	void draw();

private:

	void buildList();
	int dStrength;

};

#endif