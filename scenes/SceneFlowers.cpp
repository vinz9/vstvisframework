#include "SceneFlowers.h"

SceneFlowers::SceneFlowers(AudioEffect* effect):Scene(effect)
{

initScene();
}
//------------------------------------------
void SceneFlowers::initScene()
{

	int in[4];
	in[0] = 0;
	in[1] = 2;
	in[2] = 10;
	in[3] = 10;

	objects3d[0] = new Flower(effect, 0, 6.0, 200);
	objects3d[0]->setInput(0);
	objects3d[0]->setParams(in);
	objects3d[0]->polmode = 1;

	objects3d[1] = new Flower(effect, 1, 8.5, 100);
	((VstPlugin*)effect)->audioInputs[4]->initFilters(16);
	objects3d[1]->setInput(4);
	

	in[0] = 6;
	in[1] = 7;

	objects3d[3] = new RandomLine(effect,1.0);
	objects3d[3]->setParams(in);

	dis = 0;
	kal = 3;
	level2 = 0;
	
}

//------------------------------------------
void SceneFlowers::drawScene()
{

	//drawBlur(1,1.0,-0.02,0.15f);
	float a = ((VstPlugin*)effect)->temps->getElapsedTime();

	dis = ((VstPlugin*)effect)->audioInputs[4]->level*0.3;

	gluLookAt(0,0,-2,0,0,0,0,1,0);

	int coef = 30;
		

		glPushMatrix();
		
		//glRotatef(a*coef,0,1,0);
		glRotatef(45*cos(a)-90,0,1,0);

		glTranslatef(0,-0.4,0);
		
		glRotatef(45,0,0,1);
		glTranslatef(0,dis,0);

		objects3d[1]->draw();
		glPopMatrix();

	level2 = level2 + ((VstPlugin*)effect)->audioInputs[2]->level* 1.2;
	float s = 0.4 + 0.15*sin(level2);

	glPushMatrix();
		
		glTranslatef(-0.8,0.15,0);
		glRotatef(a*coef*2,0,1,0);

	glTranslatef(-0.05,-0.1,0);
		glRotatef(45,0,0,1);
		glScalef(s,s,s);
		objects3d[0]->draw();
	glPopMatrix();

		glPushMatrix();
		
		glTranslatef(0.8,0.15,0);
		glRotatef(a*coef*2,0,1,0);

	glTranslatef(-0.05,-0.1,0);
		glRotatef(45,0,0,1);
		glScalef(s,s,s);
		objects3d[0]->draw();
	glPopMatrix();

	objects3d[3]->draw();

	AudioInput* input9 = ((VstPlugin*)effect)->audioInputs[9];

	if (input9->hasBeenTriggered())
	{
		if (objects3d[0]->polmode == 1) 
		{
			objects3d[0]->polmode = 2;
			objects3d[1]->polmode = 1;
		}
		else
		{
			objects3d[0]->polmode = 1;
			objects3d[1]->polmode = 0;
		}
		input9->updateTrigger();
	}


	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);

	AudioInput* input8 = ((VstPlugin*)effect)->audioInputs[8];

	if (input8->hasBeenTriggered())
	{
		if (kal == 3) kal = 0;
		else kal++ ;
		input8->updateTrigger();
	}

	kalei(kal);

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);
	drawBlur(20,0.1,0.01f,0.01);

}

//--------------------------------------------
void SceneFlowers::updateScene()
{

	((VstPlugin*)effect)->audioInputs[0]->peak();
	((VstPlugin*)effect)->audioInputs[2]->peak();
	((VstPlugin*)effect)->audioInputs[4]->peak();
	((VstPlugin*)effect)->audioInputs[6]->peak();
	((VstPlugin*)effect)->audioInputs[8]->peak();
	((VstPlugin*)effect)->audioInputs[9]->peak();
		
	((VstPlugin*)effect)->audioInputs[4]->processFilters();
	((VstPlugin*)effect)->audioInputs[0]->fft();


}

//--------------------------------------------
void SceneFlowers::initInputsAndGl()
{

	((VstPlugin*)effect)->audioInputs[0]->halfLife = 0.1;
	((VstPlugin*)effect)->audioInputs[4]->halfLife = 0.2;

}

