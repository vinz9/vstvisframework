/* flower code from http://asilvast.kapsi.fi/pseudotoad/projects */

#ifndef FLOWER_H_
#define FLOWER_H_

#include "Object3d.h"

typedef float Vect[3];

class Flower : public Object3d
{
public:

	Flower();
	Flower(AudioEffect* effect, int m, float stg1, float stg2);
	~Flower();

	void draw();
	void setInput(int input);

private:

	float* audiobars;

	void spline3DMorph(float factor, float poikkeama);
	void splineTCP(float u, Vect * control, Vect * result);

	void updateActive();

	int active[12];

	float tension, continuity, bias;
	float tension_new, continuity_new, bias_new;

	float spd;

	float posz, posz_new;
	float timef;

	int montime, in, mode, freq, np, nbands;

	float timeinput, st1, st2, def;

};

#endif
