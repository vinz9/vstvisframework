#include "SceneCubes.h"

SceneCubes::SceneCubes(AudioEffect* effect):Scene(effect)
{

	initScene();
}
//------------------------------------------
void SceneCubes::initScene()
{

	int in[4];
	in[0] = 0;
	in[1] = 0;
	in[2] = 10;
	in[3] = 10;

	objects3d[0] = new Tree(effect, 0, 8.0);
	objects3d[0]->setParams(in);

	objects3d[1] = new Box(effect, 1.0);
	in[0]=in[1]=10;
	objects3d[1]->setParams(in);




	GLfloat L0pos[]={ 0.0,0.0,1.0, 0.0};
	GLfloat L0dif[]={ 1.0,1.0,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
	glLightfv(GL_LIGHT0,GL_POSITION,L0pos);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	
	//((VstPlugin*)effect)->audioInputs[1]->halfLife = 0.05;

	t = 5;
	bX = 0;
	invertFill = TRUE;
}

//------------------------------------------
void SceneCubes::drawScene()
{

	AudioInput* input8 = ((VstPlugin*)effect)->audioInputs[8];

	if (input8->hasBeenTriggered())
	{
		if (invertFill == FALSE) invertFill = TRUE;
		else invertFill = FALSE;
		input8->updateTrigger();
	}

	t = t + ((VstPlugin*)effect)->audioInputs[4]->level*1.0;

	gluLookAt(14*sin(t),5,14*cos(t),0,0,0,0,1,0);

	renderScene();

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float b = ((VstPlugin*)effect)->audioInputs[2]->level;
	drawBlur(20,0.6*b,0.01,0.01f);

	renderScene();

}

//------------------------------------------
void SceneCubes::renderScene()
{

	float s = 1.0;

	if (invertFill == TRUE)
	{
		glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
		glDisable(GL_LIGHTING);
	}

	glPushMatrix();
	glScalef(s,s,s);


	objects3d[0]->draw();
	glRotatef(90.0,0,1,0);
	objects3d[0]->draw();

	glRotatef(180.0,0,0,1);

	objects3d[0]->draw();
	glRotatef(90.0,0,1,0);

	objects3d[0]->draw();

	glPopMatrix();

	if (invertFill == TRUE)
	{
		glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
		glEnable(GL_LIGHTING);
	}

	s = 0.4;
	bX = bX + ((VstPlugin*)effect)->audioInputs[6]->level*0.25;
	float bS = 0; //((VstPlugin*)effect)->audioInputs[6]->level*0.5;

	if (invertFill == FALSE)
	{
		glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
		glDisable(GL_LIGHTING);
	}

	for (int i = 0; i<4; i++)
	{

		glRotatef(90,0,1,0);

		glPushMatrix();
		glTranslatef(0,-0.5,-5);
		glTranslatef(4*sin(bX),0,0);

		glScalef(s+bS,s+bS,s+bS);	
		objects3d[1]->draw();
		glPopMatrix();
	}

	if (invertFill == FALSE)
	{
		glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
		glEnable(GL_LIGHTING);
	}

}


//--------------------------------------------
void SceneCubes::updateScene()
{

	((VstPlugin*)effect)->audioInputs[0]->peak();
	//((VstPlugin*)effect)->audioInputs[1]->peak();
	((VstPlugin*)effect)->audioInputs[2]->peak();
	//((VstPlugin*)effect)->audioInputs[3]->peak();
	((VstPlugin*)effect)->audioInputs[4]->peak();
	//((VstPlugin*)effect)->audioInputs[5]->peak();
	((VstPlugin*)effect)->audioInputs[6]->peak();
	//((VstPlugin*)effect)->audioInputs[7]->peak();
	((VstPlugin*)effect)->audioInputs[8]->peak();

	//((VstPlugin*)effect)->audioInputs[2]->fft();
	//((VstPlugin*)effect)->audioInputs[3]->fft();


}


//--------------------------------------------
void SceneCubes::initInputsAndGl()
{

((VstPlugin*)effect)->audioInputs[0]->halfLife = 0.05;

}