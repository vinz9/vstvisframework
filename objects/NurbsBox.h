#ifndef NURBSBOX_H
#define NURBSBOX_H

#include "Object3d.h"


class NurbsBox : public Object3d
{

public:

	NurbsBox();
	NurbsBox(AudioEffect* effect);
	NurbsBox(AudioEffect* effect, float strength);
	~NurbsBox();

	void draw();

private:

	float attack, l, st, steps;
	int chemin;

};

#endif