#ifndef FREQCUBES_H
#define FREQCUBES_H

#include "Object3d.h"


class FreqCubes : public Object3d
{

public:

	FreqCubes();
	FreqCubes(AudioEffect* effect);
	FreqCubes(AudioEffect* effect, float strength);
	~FreqCubes();

	void draw();
	void setInput(int input);

private:

	void buildList();

	float st;

	int freq;
	float space;
	int count;
	int in;

	int box;

};

#endif