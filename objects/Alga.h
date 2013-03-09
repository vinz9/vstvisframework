#ifndef ALGA_H
#define ALGA_H

#include "Object3d.h"


class Alga : public Object3d
{

public:

	Alga();
	Alga(AudioEffect* effect);
	Alga(AudioEffect* effect, int strength);
	~Alga();

	void draw();
	void setInput(int input);

	

private:

	float attack, k, st, steps;

};

#endif