#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "../VstPlugin.h"
//#include "audioeffectx.h"

#define MAXPARAMS 4

class Object3d
{
public:
	Object3d();
	Object3d(AudioEffect* effect);
	~Object3d();

	AudioEffect* effect;

	virtual void draw() = 0;

	virtual void setInput(int input) {};

	void setParams(int* inputs);

	float* params[MAXPARAMS];
	//float params[10];
	int animate;
	int animSpeed;

	int polmode;

	float** keyframes;
	float** allframes;




private:


};

#endif
