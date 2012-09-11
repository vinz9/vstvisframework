/* line equation from milkdrop preset EOS + Phat - Chasers */

#ifndef RANDOMLINE_H
#define RANDOMLINE_H

#include "Object3d.h"


class RandomLine : public Object3d
{

public:

	RandomLine();
	RandomLine(AudioEffect* effect, float stg1);
	~RandomLine();

	void draw();
	void drawSpiral();
	void drawChaser();

private:

	float st1, decal;

	int temps;
	float time;

	float sample;

	float xp,xq,xr,xs,x;
	float yp,yq,yr,ys,y;
	float zp, zq, zr;
	float ang;
	float sa, ca;
	float flip, tm, q1, phs, n;
	float mtime;

};

#endif