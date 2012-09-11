#include "SceneBasic.h"

SceneBasic::SceneBasic(AudioEffect* effect):Scene(effect)
{

initScene();
}
//------------------------------------------
void SceneBasic::initScene()
{

	int in[4];
	in[0] = 4;
	in[1] = 5;
	in[2] = 0;
	in[3] = 0;

	objects3d[0] = new fftBox(effect, 2.5);
	objects3d[0]->setInput(2);

	objects3d[1] = new fftBox(effect, 2.5);
	objects3d[1]->setInput(3);

	objects3d[2] = new Box(effect,14);
	objects3d[2]->setParams(in);
	


	GLfloat L0pos[]={ 0.0,0.0,1.0, 1.0};
	GLfloat L0dif[]={ 1.0,1.0,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
	glLightfv(GL_LIGHT0,GL_POSITION,L0pos);

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

}

//------------------------------------------
void SceneBasic::drawScene()
{

	drawBlur(1,1.0f,-0.02,0.15f);

	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.0,2.5,-9);
	glTranslatef(-2.5,0.0,0.0);

	glDisable(GL_LIGHTING);
	glLineWidth(2.0);

	float* buffer = (((VstPlugin*)effect)->audioInputs[0]->buffer);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i<BUFFERSIZE; i++)
		{
			glVertex2f( 4*((float)i/BUFFERSIZE)-2, buffer[i]*1.0);
		}
	glEnd();

	glTranslatef(5,0.0,0.0);

	buffer = (((VstPlugin*)effect)->audioInputs[1]->buffer);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i<BUFFERSIZE; i++)
		{
			glVertex2f( 4*((float)i/BUFFERSIZE)-2, buffer[i]*1.0);
		}
	glEnd();
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
	glLineWidth(1.0);
	
	glPushMatrix();
	glTranslatef(0,2,-1);
	glTranslatef(-2.5,0,0);
	objects3d[0]->draw();

	glTranslatef(5,0,0);
	objects3d[1]->draw();
	glPopMatrix();

	glLineWidth(2.0);

	glPushMatrix();
	
	glTranslatef(0.0,-3,-11.0);
	objects3d[2]->draw();
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
	

	glPopMatrix();

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);
}

//--------------------------------------------
void SceneBasic::updateScene()
{

	((VstPlugin*)effect)->audioInputs[0]->peak();
	((VstPlugin*)effect)->audioInputs[1]->peak();
	((VstPlugin*)effect)->audioInputs[4]->peak();
	((VstPlugin*)effect)->audioInputs[5]->peak();

	((VstPlugin*)effect)->audioInputs[2]->fft();
	((VstPlugin*)effect)->audioInputs[3]->fft();


}

//--------------------------------------------
void SceneBasic::initInputsAndGl()
{

}