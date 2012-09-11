#ifndef VISFRAMEWORK_H_
#define VISFRAMEWORK_H_

#include "VSTGLEditor.h"
#include "VSTGLTimer.h"
#include "VstPlugin.h"

#include <iostream>

#include "SceneBasic.h"
#include "SceneCubes.h"
#include "SceneNurbs.h"
#include "SceneSpheres.h"
#include "SceneFlowers.h"

#include "Scene.h"

#define NUMSCENES 5


///	Simple VSTGL example.
class VisFrameWork : public VSTGLEditor,
					  public Timer
{
  public:

	Scene* scenes[NUMSCENES];
	Scene* currentScene;
	int s;

	VisFrameWork(AudioEffect *effect);
	~VisFrameWork();

	void changeScene(int scene);

	void guiOpen();
	void guiClose();
	void draw();
	void timerCallback();
	void onMouseDown(int button, int x, int y);
	void onGLKeyDown(const VstKeyCode& key);

  private:

};

#endif
