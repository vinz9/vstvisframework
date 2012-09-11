#include "SceneSpheres.h"

SceneSpheres::SceneSpheres(AudioEffect* effect):Scene(effect)
{

	initScene();
}
//------------------------------------------
void SceneSpheres::initScene()
{

	int in[4];
	in[0] = 2;
	in[1] = 3;
	in[2] = 10;
	in[3] = 10;

	objects3d[0] = new FreqSpheres(effect, 14.0, 5.0);
	objects3d[0]->setInput(0);
	objects3d[0]->setParams(in);

	in[0] = 6;
	in[1] = 10;

	objects3d[1] = new NurbsBox(effect, 8.0);
	objects3d[1]->setParams(in);

	GLfloat L0pos[]={ 0.0,0.0,1.0, 0.0};
	GLfloat L0dif[]={ 1.0,1.0,1.0};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
	glLightfv(GL_LIGHT0,GL_POSITION,L0pos);

	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	

//LoadTGA(&textures[0],"white.tga");
sgn = 1;
iv = 0;
attack = 0;
level4 = level8 = 0;
	
}

//------------------------------------------
void SceneSpheres::drawScene()
{
	glPointSize(2.0);
	AudioInput* input2 = ((VstPlugin*)effect)->audioInputs[2];

	float level0 = input2->level;

	if (input2->hasBeenTriggered())
	{
		sgn=sgn*-1;
		input2->updateTrigger();
	}

	//drawBlur(1,1.0f,-0.02,0.15f);
	drawBlur(1,1.0f,sgn*0.02,1-level0*2.3f);

	float a = ((VstPlugin*)effect)->temps->getElapsedTime();

	//gluLookAt(-20*cos(a),0,-20*sin(a),0,0,0,0,1,0);
	gluLookAt(0,5,-16,0,0,0,0,1,0);

	glEnable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
glPushMatrix();
	glRotatef(50*a,0,1,0);
	objects3d[0]->draw();
glPopMatrix();	

float s = 2.0;

level8 = level8+((VstPlugin*)effect)->audioInputs[8]->level*15.0;

glPushMatrix();
glTranslatef(0,-0.5,0);
glRotatef(-180,0,1,0);
for (int i = 0; i<2; i++)
{
glRotatef(180,0,1,0);

glPushMatrix();
glRotatef(level8,0,0,1);
glTranslatef(-8,0,0);
glScalef(s,s,s);

objects3d[1]->draw();
glRotatef(180,0,0,1);
objects3d[1]->draw();
glPopMatrix();	
}

glPopMatrix();	

glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, SIZE, SIZE, 0);

	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);


	level4 = ((VstPlugin*)effect)->audioInputs[4]->level;

	if (attack <1 && level4>0.05) attack = attack + 0.25;
	else if (level4 < 0.05 && attack > 0) attack = attack - 0.25;

	c = level4*16.0;

	invert(0,0);
	


}

//------------------------------------------
/*void SceneSpheres::renderScene()
{


}*/

//--------------------------------------------
void SceneSpheres::updateScene()
{
	((VstPlugin*)effect)->audioInputs[0]->peak();
	((VstPlugin*)effect)->audioInputs[2]->peak();
	((VstPlugin*)effect)->audioInputs[4]->peak();
	((VstPlugin*)effect)->audioInputs[6]->peak();
	((VstPlugin*)effect)->audioInputs[8]->peak();

	((VstPlugin*)effect)->audioInputs[0]->fft();
}

//------------------------------------------------------------------------------
void SceneSpheres::invert(int mode, int pattern)
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
		
		ds = (sin(2*t)+1)*0.375;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0); glVertex2f(0.0,0.0+ds);									
			glTexCoord2f(0.0,1.0);   glVertex2f(0.0,0.25+ds);									
			glTexCoord2f(1.0,1.0); glVertex2f(1.0,0.25+ds);							
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
void SceneSpheres::initInputsAndGl()
{
	((VstPlugin*)effect)->audioInputs[0]->halfLife = 0.02;
	((VstPlugin*)effect)->audioInputs[4]->halfLife = 0.02;
}