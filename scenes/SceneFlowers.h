#ifndef SCENEFLOWERS_H_
#define SCENEFLOWERS_H_

#include "Scene.h"

/*
input 1 = little flowers notes
input 2 = little flowers petals + size
input 3 = center flower
input 4 = chaser position
input 5a = kaleidoscope mode
input 5b = fill inversion

*/

class SceneFlowers : public Scene

{

public:

	SceneFlowers(AudioEffect* effect);

	void initScene();
	void drawScene();
	void updateScene();
	void initInputsAndGl();

private:

	int kal;
	float dis, level2;
};

#endif