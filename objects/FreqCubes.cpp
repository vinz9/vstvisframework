#include "FreqCubes.h"


//----------------------------------------------------------------------------
FreqCubes::FreqCubes()
{
	buildList();
}

//----------------------------------------------------------------------------
FreqCubes::FreqCubes(AudioEffect* effect):Object3d(effect)
{
	buildList();
	st = 4.0;
	count = 0;
}

//----------------------------------------------------------------------------
FreqCubes::FreqCubes(AudioEffect* effect, float strength):Object3d(effect),st(strength),count(0),freq(-1)
{
	buildList();
	polmode = 0;
}

//----------------------------------------------------------------------------
FreqCubes::~FreqCubes()
{

}

//----------------------------------------------------------------------------
void FreqCubes::draw()
{

	freq = ((VstPlugin*)effect)->audioInputs[in]->getNote();

	glLineWidth(1.0);

	space = 0.5 + *params[0]*st;

	count = 0;

	glPushMatrix();
	glTranslatef(-2.0-space,-1-0.5*space,-1-0.5*space);
	glPushMatrix();
	for (int y=0; y<2; y++)
	{
		glPushMatrix();
		for (int z = 0; z<2; z++)
		{
			glPushMatrix();
			for (int x = 0; x<3; x++)
			{
				if (count == freq)
				{

					glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
					glEnable(GL_LIGHTING);
				}
				else
				{
					if (polmode == 0)
					{
						glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
						glDisable(GL_LIGHTING);
					}
					else
					{
						glPolygonMode(GL_FRONT_AND_BACK , GL_POINT);

						glDisable(GL_LIGHTING);
						glDisable(GL_DEPTH_TEST);
						glDisable(GL_LIGHTING);
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						glColor4f(1.0,1.0,1.0,0.2);
						glPointSize(10.0f);
					}

				}


				glCallList(box);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

	glColor4f(1.0,1.0,1.0,1.0);
	glPointSize(2.0f);
				glTranslatef(2.0+space,0,0);
				count++;
			}
			glPopMatrix();
			glTranslatef(0,0,2.0+space);
		}
		glPopMatrix();
		glTranslatef(0,2.0+space,0);
	}
	glPopMatrix();
	glPopMatrix();

	glLineWidth(2.0);
}
//----------------------------------------------------------------------------
void FreqCubes::buildList()
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

void FreqCubes::setInput(int input)
{
	in = input;
}