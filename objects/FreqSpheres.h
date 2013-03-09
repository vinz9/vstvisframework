#ifndef FREQSPHERES_H
#define FREQSPHERES_H

#include "Object3d.h"


class FreqSpheres : public Object3d
{

public:

	FreqSpheres();
	FreqSpheres(AudioEffect* effect);
	FreqSpheres(AudioEffect* effect, float strength, float strength2);
	~FreqSpheres();

	void draw();
	void setInput(int input);

private:

	void buildList();
	void drawSphere();

	float st,st2;
	float t;

	int freq;
	int in;
	float attack;
	int anim;

	int box;

	GLUquadricObj *quadratic;

};

#endif