#ifndef SCENENURBS_H_
#define SCENENURBS_H_

#include "Scene.h"

/*
input 1 = note : solid cube
input 2 = algae
input 3 = color inversion band
input 4 = middle cubes space
input 5 = lines/dots inversion + blur
*/

class SceneNurbs : public Scene

{
public:
	SceneNurbs(AudioEffect* effect);

	void initScene();
	void drawScene();
	void updateScene();
	void initInputsAndGl();

private:

	int k;
	float c;
	float steps;
	float attack;
	float level2, level4;

	void invert(int mode, int pattern);

};

#endif