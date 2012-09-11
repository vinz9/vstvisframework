#include "VisFrameWork.h"
#include <math.h>

//----------------------------------------------------------------------------
VisFrameWork::VisFrameWork(AudioEffect *effect):
VSTGLEditor(effect, Antialias4x),
Timer(30) //30ms == ~30fps?
{
	//Set the opengl context's size - This must be called here!
	setRect(0, 0, WIDTH, HEIGHT);

	s=0;
	
}

//----------------------------------------------------------------------------
VisFrameWork::~VisFrameWork()
{


}

//----------------------------------------------------------------------------
void VisFrameWork::guiOpen()
{

	//Setup OpenGL stuff.
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, getWidth(), getHeight());

    //Set viewing projection.
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)getWidth()/(GLfloat)getHeight(), 0.1f, 100.0f);

    //Position viewer.
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do


	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
	glPointSize(2.0f);
	glEnable(GL_NORMALIZE);

	//BuildFont();
	
	scenes[0] = new SceneBasic(effect);
	scenes[1] = new SceneCubes(effect);
	scenes[2] = new SceneNurbs(effect);
	scenes[3] = new SceneSpheres(effect);
	scenes[4] = new SceneFlowers(effect);

	changeScene(s);

//Start timer to constantly update gui.
	start();
}

//----------------------------------------------------------------------------
void VisFrameWork::guiClose()
{
	//Stop the timer.
	stop();
	for (int i = 0; i<4; i++)
	{
		delete scenes[i];
	}

	//KillFont();

}

//----------------------------------------------------------------------------
void VisFrameWork::draw()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//if (((VstPlugin*)effect)->temps->getElapsedTime() > 20)
	//currentScene = scenes[1];
	
	currentScene->drawScene();


	/*glColor3f(1,1,1);
	glRasterPos2f(0,0);
	glPrint("blabla");*/
											
	const GLubyte *errString;
	errString = gluErrorString(glGetError());

}

//----------------------------------------------------------------------------
void VisFrameWork::timerCallback()
{
	
	currentScene->updateScene();

	refreshGraphics();
}

//----------------------------------------------------------------------------
void VisFrameWork::onMouseDown(int button, int x, int y) {

	if (button ==1) {

		SetFocus(tempHWnd); 
	}

}
//----------------------------------------------------------------------------
void VisFrameWork::onGLKeyDown(const VstKeyCode& key) {

	if (key.character == 72) {
		if (s+1 < NUMSCENES)
		{
			s++;
		}
		else
		{
			s = 0;
		}
		changeScene(s);
	}
	if (key.character == 74) {
		//currentScene=scenes[1];
	}
}

//----------------------------------------------------------------------------
void VisFrameWork::changeScene(int scene)
{
	currentScene=scenes[s];
	currentScene->initInputsAndGl();
	
}

