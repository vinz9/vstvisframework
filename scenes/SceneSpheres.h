#ifndef SCENESPHERES_H_
#define SCENESPHERES_H_

#include "Scene.h"

/*
input 1 = center spheres
input 2 = blur + little spheres
input 3 = color inversion
input 4 = blobs size
input 5 = blobs rotation
*/

class SceneSpheres : public Scene

{
public:
	SceneSpheres(AudioEffect* effect);

	void initScene();
	void drawScene();
	void updateScene();
	void initInputsAndGl();

private:

	void invert(int mode, int pattern);

	int sgn;
	int iv;

	float c, attack, level4, level8;
};

#endif