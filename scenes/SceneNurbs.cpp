#include "SceneNurbs.h"

SceneNurbs::SceneNurbs(AudioEffect* effect):Scene(effect)
{

initScene();
}
//------------------------------------------
void SceneNurbs::initScene()
{

	int in[4];
	in[0] = 2;
	in[1] = 3;
	in[2] = 10;
	in[3] = 10;

	objects3d[0] = new Alga(effect, 300.0);
	objects3d[0]->setParams(in);

	in[0] = 6;
	in[1] = 7;

	objects3d[1] = new FreqCubes(effect, 5.0);
	objects3d[1]->setParams(in);
	objects3d[1]->setInput(0);



	GLfloat L0pos[]={ 0.0,0.0,1.0, 0.0};
	GLfloat L0dif[]={ 1.0,1.0,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
	glLightfv(GL_LIGHT0,GL_POSITION,L0pos);

	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable (GL_POINT_SMOOTH);
	


	level4=0;
	level2 = 0;
	attack = 0;

}

//------------------------------------------
void SceneNurbs::drawScene()
{
	glPointSize(5.0);
	
	float a = ((VstPlugin*)effect)->temps->getElapsedTime()*0.2;

	float level8 = ((VstPlugin*)effect)->audioInputs[8]->level;
	AudioInput* input8 = ((VstPlugin*)effect)->audioInputs[8];
	level2 = level2 + ((VstPlugin*)effect)->audioInputs[2]->level*4.0;
	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);

	drawBlur(1,1.0f,0.02,1-level8*6.0f);
	glDisable(GL_LIGHTING);

	//gluLookAt(0,-15,-15,0,0,0,0,-sin(45.0),cos(45.0));
	gluLookAt(-20*cos(a),9,-20*sin(a),0,0,0,0,1,0);

		if (input8->hasBeenTriggered())
	{
		if (objects3d[0]->polmode == 0)
		{
			objects3d[0]->polmode = 1;
			objects3d[1]->polmode = 1;
		}
		else 
		{
			objects3d[0]->polmode = 0;
			objects3d[1]->polmode = 0;
		}

		input8->updateTrigger();
	}

	
	



	glPushMatrix();

	glRotatef(level2,0,1,0);

	for (int g=0; g<6; g++)
	{
		glRotatef(60,0,1,0);
		glPushMatrix();
		glTranslatef(0,0,-8);
		glRotatef(90,0,0,1);
		glLineWidth(2.0);
		objects3d[0]->draw();
		glLineWidth(1.0);
		glPopMatrix();

	}

	glPopMatrix();
	//glRotatef(50*a,1,0,0);
	float s = 0.6;
	glScalef(s,s,s);
	objects3d[1]->draw();

	

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);

	level4 = ((VstPlugin*)effect)->audioInputs[4]->level;

/*	if (level4*30.0 >1) c =1;
	else c = level4*30.0;*/

	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);

	/*if (attack <1 && level4>0.05) attack = attack + 0.1;
	else if (level4 < 0.05 ) attack = 0;*/

	c = level4*16.0;
	invert(0,2);
}

//------------------------------------------
/*void SceneNurbs::renderScene()
{


}*/

//--------------------------------------------
void SceneNurbs::updateScene()
{

	((VstPlugin*)effect)->audioInputs[0]->peak();
	((VstPlugin*)effect)->audioInputs[1]->peak();
	((VstPlugin*)effect)->audioInputs[2]->peak();
	//((VstPlugin*)effect)->audioInputs[3]->peak();
	((VstPlugin*)effect)->audioInputs[4]->peak();
	//((VstPlugin*)effect)->audioInputs[5]->peak();
	((VstPlugin*)effect)->audioInputs[6]->peak();
	//((VstPlugin*)effect)->audioInputs[7]->peak();
	((VstPlugin*)effect)->audioInputs[8]->peak();

	((VstPlugin*)effect)->audioInputs[0]->fft();
	//((VstPlugin*)effect)->audioInputs[3]->fft();


}

//------------------------------------------------------------------------------
void SceneNurbs::invert(int mode, int pattern)
{	

	float t,ds;
	t = ((VstPlugin*)effect)->temps->getElapsedTime();
	glEnable(GL_TEXTURE_2D);									// Enable 2D Texture Mapping
	glDisable(GL_DEPTH_TEST);									// Disable Depth Testing
	glDisable(GL_LIGHTING);

	if (mode == 0) glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA); 							
	else glBlendFunc(GL_ZERO,GL_ONE_MINUS_DST_COLOR);

	glEnable(GL_BLEND);											// Enable Blending
	glBindTexture(GL_TEXTURE_2D,textures[0].texID);					// Bind To The Blur Texture

	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0.0, 1.0 ,  0.0, 1.0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix

	//float c = 0.5*(sin(t*2)+1);
	glColor4f(c,c,c,c);

	if (pattern == 0)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,1.0);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,1.0);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.0);
		glEnd();
	}
	else if(pattern ==1)
	{
		glBegin(GL_QUADS);											// Begin Drawing Quads
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,0.5);									
			glTexCoord2f(1.0,1.0); glVertex2f(0.5,0.5);							
			glTexCoord2f(1.0,0.0); glVertex2f(0.5,0.0);

			glTexCoord2f(0.0,0.0); glVertex2f(0.5,0.5);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.5,1.0);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,1.0);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.5);
		glEnd();
	}
	else
	{
		
		ds = (sin(3*t)+1)*0.25;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0+ds);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,0.5+ds);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,0.5+ds);							
			glTexCoord2f(1.0,0.0); glVertex2f(1.0,0.0+ds);
		glEnd();
	}

	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending

}


//--------------------------------------------
void SceneNurbs::initInputsAndGl()
{
	((VstPlugin*)effect)->audioInputs[0]->halfLife = 0.05;
	((VstPlugin*)effect)->audioInputs[6]->halfLife = 0.05;
	((VstPlugin*)effect)->audioInputs[8]->halfLife = 0.15;
	((VstPlugin*)effect)->audioInputs[4]->halfLife = 0.01;
}