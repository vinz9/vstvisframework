#ifndef SCENECUBES_H_
#define SCENECUBES_H_

#include "Scene.h"


/*
input 1 = center object
input 2 = glow
input 3 = camera rotation
input 4 = cubes translation
input 5 = filling inversion
*/

class SceneCubes : public Scene

{

public:

	SceneCubes(AudioEffect* effect);

	void initScene();
	void drawScene();
	void updateScene();
	void initInputsAndGl();

private:
	void renderScene();

	float t, bX;
	bool invertFill;
};

#endif