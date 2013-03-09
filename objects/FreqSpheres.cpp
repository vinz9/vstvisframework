#include "FreqSpheres.h"


//----------------------------------------------------------------------------
FreqSpheres::FreqSpheres()
{
	buildList();
}

//----------------------------------------------------------------------------
FreqSpheres::FreqSpheres(AudioEffect* effect):Object3d(effect)
{
	buildList();
	st = 10.0;
}

//----------------------------------------------------------------------------
FreqSpheres::FreqSpheres(AudioEffect* effect, float strength, float strength2):
				Object3d(effect),st(strength),st2(strength2),freq(-1),t(0)
{
	buildList();

	quadratic = gluNewQuadric();
	//gluQuadricNormals(quadratic, GLU_SMOOTH);
	//gluQuadricNormals(quadratic, GLU_FLAT);
}

//----------------------------------------------------------------------------
FreqSpheres::~FreqSpheres()
{
	gluDeleteQuadric(quadratic);
}

//----------------------------------------------------------------------------
void FreqSpheres::draw()
{
	t = ((VstPlugin*)effect)->temps->getElapsedTime();

	float level0 = ((VstPlugin*)effect)->audioInputs[in]->level;
	freq = ((VstPlugin*)effect)->audioInputs[in]->getNote();

	if (freq>-1) 
	{
		anim = freq;
	}
	if(level0 <0.01) anim=12;

	if (attack <1 && level0>0.1) attack = attack + 0.25;
	else if (level0 < 0.1) attack =0;

	float dis = level0*st*attack;

	if (anim == 12)
		drawSphere();

	glPushMatrix();

	if (anim >0 && anim <4)
	{
		for (int i = 0; i<(anim+1); i++)
		{
			glRotatef(360/(float)(anim+1),0,1,0);
			glPushMatrix();
				glTranslatef(dis,0,0);
				drawSphere();
			glPopMatrix();
		}
	}

	if (anim == 4)
	{
		drawSphere();
		for (int i = 0; i<4; i++)
		{
			glRotatef(360/4.0,0,1,0);
			glPushMatrix();
				glTranslatef(dis,0,0);
				drawSphere();
			glPopMatrix();
		}
	}

		if (anim == 5)
	{
		drawSphere();
		for (int i = 0; i<4; i++)
		{
			glRotatef(360/4.0,0,1,0);
			glPushMatrix();
				glTranslatef(dis,0,0);
				drawSphere();
			glPopMatrix();
		}
	}

	if (anim == 6)
	{
		glPushMatrix();
			for (int i = 0; i<4; i++)
			{
				glRotatef(90.0,0,1,0);
				glPushMatrix();
					glTranslatef(dis,0,0);
					drawSphere();
				glPopMatrix();
			}
		glPopMatrix();

		glTranslatef(0,dis,0);
		drawSphere();
		glTranslatef(0,-2*dis,0);
		drawSphere();

	}

	if (anim == 7)
	{
		drawSphere();
		glPushMatrix();
			for (int i = 0; i<4; i++)
			{
				glRotatef(90.0,0,1,0);
				glPushMatrix();
					glTranslatef(dis,0,0);
					drawSphere();
				glPopMatrix();

			}
		glPopMatrix();
		glTranslatef(0,dis,0);
		drawSphere();
		glTranslatef(0,-2*dis,0);
		drawSphere();


	}

	if (anim == 8)
	{
		glTranslatef(0,dis,0);
		drawSphere();
		glTranslatef(0,-2*dis,0);
		drawSphere();
	}

	if (anim == 9 )
	{
		for (int i = 0; i<3; i++)
		{
			glRotatef(120.0,0,0,1);
			glPushMatrix();
				glTranslatef(dis,0,0);
				drawSphere();
			glPopMatrix();
		}
	}

	if (anim == 10 )
	{
		drawSphere();
		for (int i = 0; i<3; i++)
		{
			glRotatef(120.0,0,0,1);
			glPushMatrix();
				glTranslatef(dis,0,0);
				drawSphere();
			glPopMatrix();
		}
	}

	if (anim == 11 )
	{
		glTranslatef(0,dis,0);
		drawSphere();
		glTranslatef(0,-2*dis,0);
		drawSphere();
		glTranslatef(dis, dis,0);
		drawSphere();
		glTranslatef(-2*dis,0,0);
		glRotatef(180,0,1,0);
		drawSphere();
		
	}

	if (anim == 0 )
	{
		drawSphere();
		glTranslatef(0,dis,0);
		drawSphere();
		glTranslatef(0,-2*dis,0);
		drawSphere();
		glTranslatef(dis, dis,0);
		drawSphere();
		glTranslatef(-2*dis,0,0);
		glRotatef(180,0,1,0);
		drawSphere();
	}
	glPopMatrix();
}

//----------------------------------------------------------------------------
void FreqSpheres::drawSphere()
{
	int res = 10;
	float dis2 = *params[0]* st2;
	glRotatef(90,1,0,0);
	gluSphere(quadratic,1.0f,res,res);

	glPushMatrix();
		glRotatef(-t*200,0,0,1);
		glTranslatef(dis2,0,0);
		gluSphere(quadratic,0.4f,res,res);
		glTranslatef(-dis2,0,0);
		glRotatef(180,0,0,1);
		glTranslatef(dis2,0,0);
		gluSphere(quadratic,0.4f,res,res);
	glPopMatrix();

	glRotatef(-90,1,0,0);

}
//----------------------------------------------------------------------------
void FreqSpheres::buildList()
{

	box=glGenLists(1);
	glNewList(box,GL_COMPILE);

	glBegin(GL_QUADS);									// Draw A Quad

	glNormal3f(0.0,1.0,0.0);
	//glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
	glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
	glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)

	glNormal3f(0.0,-1.0,0.0);
	//glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
	glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
	glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)

	glNormal3f(0.0,0.0,1.0);
	//glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
	glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
	glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
	glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)

	glNormal3f(0.0,0.0,-1.0);
	//glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
	glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
	glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
	glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)

	glNormal3f(-1.0,0.0,0.0);
	//glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)

	glNormal3f(1.0,0.0,0.0);
	//glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
	glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
	glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)

	glEnd();											// Done Drawing The Quad
	glEndList();
}

void FreqSpheres::setInput(int input)
{
	in = input;
}