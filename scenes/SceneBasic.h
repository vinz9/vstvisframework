#ifndef SCENEBASIC_H_
#define SCENEBASIC_H_

#include "Scene.h"

/*
input 1 = oscilloscope
input 2 = fft boxes
input 3 = cube pan
input 4 = 
input 5 = 
*/

class SceneBasic : public Scene

{

public:

	SceneBasic(AudioEffect* effect);

	void initScene();
	void drawScene();
	void updateScene();
	void initInputsAndGl();


};

#endif