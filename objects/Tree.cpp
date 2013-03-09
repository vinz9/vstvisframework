
#include "Tree.h"


int lbox;

//----------------------------------------------------------------------------
Tree::Tree()
{
	buildList();

}

//----------------------------------------------------------------------------
Tree::Tree(AudioEffect* effect):Object3d(effect)
{
	buildList();

	leftArm = rightArm = 0.0;
	boxScale = 1.0;
	redcolor = 1.0;

	animate = 0;
	chemin = 0;
	trigger = 0;
	counter = 0;

	aSt=1.0;

}

//----------------------------------------------------------------------------
Tree::Tree(AudioEffect* effect, int animMode, float armsSt):Object3d(effect),animate(animMode),aSt(armsSt)
{
	buildList();

	leftArm = rightArm = 0.0;
	boxScale = 1.0;
	redcolor = 1.0;

	chemin = 0;
	trigger = 0;
	counter = 0;

}

//----------------------------------------------------------------------------
Tree::~Tree()
{

}

//----------------------------------------------------------------------------
void Tree::draw()
{			

/*	if (*params[3] < 1.0)
	{
		redcolor = 1 - *params[3];
	}
	else
	{
		redcolor = 0.0;
	}

	glColor3f(redcolor,1.0,1.0);*/
	

	float st = 2.0;

	if (animate == 0)
	{
		leftArm = *params[0]*aSt;
		rightArm = *params[1]*aSt;
	//	boxScale = (1.0+*params[2]*st);
	}
	else if (animate == 1)
	{
		if (chemin == 0)
		{
			if((leftArm + (*params[0])/st) < 3.0 )
			{
				leftArm = leftArm + (*params[0])/st;
				rightArm = rightArm + (*params[0])/st;
			}
			else
			{
				chemin = 1;
				//leftArm = 2- (leftArm + (*params[0])/5.0 - 2);
				//rightArm = 2 - (rightArm + (*params[0])/5.0 -2 );
			}
		}
		else
		{
			if ((leftArm - (*params[0])/st) > 0 )
			{
				leftArm =leftArm - (*params[0])/st;
				rightArm = rightArm - (*params[0])/st;
			}
			else
			{
				chemin = 0;
				//leftArm = 0 - (2 - leftArm - (*params[0])/5.0);
				//rightArm = 0 - (2 - rightArm - (*params[0])/5.0);
			}
		}
	}
	else if (animate == 2)
	{
		if (*params[0] > 0.2 && trigger == 0 )
		{
			trigger = 1;
		}

		if (trigger == 1)
		{
			if (counter < 40)
			{
				if (leftArm <= 2 && chemin == 0)
				{
					leftArm = leftArm + 0.1;
					rightArm = rightArm + 0.1;
				}
				else
				{

					leftArm = leftArm - 0.1;
					rightArm = rightArm - 0.1;
					chemin = 1;
				}
				counter ++;
			}
			else 
			{
				counter = 0;
				trigger = 0;
				leftArm = rightArm = 0;
				chemin = 0;
			}

		}

	}

	glPushMatrix();
	glTranslatef(-0.5,0.0,0.0);

	glScalef(boxScale,1.0,1.0);
	glCallList(lbox);
	glScalef(1.0/boxScale,1.0,1.0);


	if(leftArm<=1) {
		glRotatef(30.0*leftArm,0.0,0.0,1.0);
		glTranslatef(0.0,1.2*leftArm,0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);
	}
	else {
		glRotatef(30.0,0.0,0.0,1.0);
		glTranslatef(0.0,1.2,0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);

		glRotatef(30.0*(leftArm-1),0.0,0.0,1.0);
		glTranslatef(0.0,1.2*(leftArm-1),0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);

	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5,0.0,0.0);

	glScalef(boxScale,1.0,1.0);
	glCallList(lbox);
	glScalef(1.0/boxScale,1.0,1.0);

	if(rightArm<=1) {
		glRotatef(-30.0*rightArm,0.0,0.0,1.0);
		glTranslatef(0.0,1.2*rightArm,0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);
	}
	else {
		glRotatef(-30.0,0.0,0.0,1.0);
		glTranslatef(0.0,1.2,0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);

		glRotatef(-30.0*(rightArm-1),0.0,0.0,1.0);
		glTranslatef(0.0,1.2*(rightArm-1),0.0);
		glScalef(boxScale,1.0,1.0);
		glCallList(lbox);
		glScalef(1.0/boxScale,1.0,1.0);
	}
	glPopMatrix();

	glColor3f(1.0,1.0,1.0);

}

//----------------------------------------------------------------------------
void Tree::updateParams()
{
	/*params[1] = ((VstPlugin*)effect)->audioInputs[0]->level;
	params[0] = ((VstPlugin*)effect)->audioInputs[1]->level;*/

}

//----------------------------------------------------------------------------
void Tree::buildList()
{

lbox=glGenLists(1);
glNewList(lbox,GL_COMPILE);

glBegin(GL_QUADS);									// Draw A Quad

		glNormal3f(0.0,1.0,0.0);
		//glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f( 0.15f, 0.5f,-0.25f);					// Top Right Of The Quad (Top)
		glVertex3f(-0.15f, 0.5f,-0.25f);					// Top Left Of The Quad (Top)
		glVertex3f(-0.15f, 0.5f, 0.25f);					// Bottom Left Of The Quad (Top)
		glVertex3f( 0.15f, 0.5f, 0.25f);					// Bottom Right Of The Quad (Top)

		glNormal3f(0.0,-1.0,0.0);
		//glColor3f(1.0f,0.25f,0.0f);						// Set The Color To Orange
		glVertex3f( 0.15f,-0.5f, 0.25f);					// Top Right Of The Quad (Bottom)
		glVertex3f(-0.15f,-0.5f, 0.25f);					// Top Left Of The Quad (Bottom)
		glVertex3f(-0.15f,-0.5f,-0.25f);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( 0.15f,-0.5f,-0.25f);					// Bottom Right Of The Quad (Bottom)

		glNormal3f(0.0,0.0,1.0);
		//glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f( 0.15f, 0.5f, 0.25f);					// Top Right Of The Quad (Front)
		glVertex3f(-0.15f, 0.5f, 0.25f);					// Top Left Of The Quad (Front)
		glVertex3f(-0.15f,-0.5f, 0.25f);					// Bottom Left Of The Quad (Front)
		glVertex3f( 0.15f,-0.5f, 0.25f);					// Bottom Right Of The Quad (Front)

		glNormal3f(0.0,0.0,-1.0);
		//glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
		glVertex3f( 0.15f,-0.5f,-0.25f);					// Top Right Of The Quad (Back)
		glVertex3f(-0.15f,-0.5f,-0.25f);					// Top Left Of The Quad (Back)
		glVertex3f(-0.15f, 0.5f,-0.25f);					// Bottom Left Of The Quad (Back)
		glVertex3f( 0.15f, 0.5f,-0.25f);					// Bottom Right Of The Quad (Back)

		glNormal3f(-1.0,0.0,0.0);
		//glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f(-0.15f, 0.5f, 0.25f);					// Top Right Of The Quad (Left)
		glVertex3f(-0.15f, 0.5f,-0.25f);					// Top Left Of The Quad (Left)
		glVertex3f(-0.15f,-0.5f,-0.25f);					// Bottom Left Of The Quad (Left)
		glVertex3f(-0.15f,-0.5f, 0.25f);					// Bottom Right Of The Quad (Left)

		glNormal3f(1.0,0.0,0.0);
		//glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
		glVertex3f( 0.15f, 0.5f,-0.25f);					// Top Right Of The Quad (Right)
		glVertex3f( 0.15f, 0.5f, 0.25f);					// Top Left Of The Quad (Right)
		glVertex3f( 0.15f,-0.5f, 0.25f);					// Bottom Left Of The Quad (Right)
		glVertex3f( 0.15f,-0.5f,-0.25f);					// Bottom Right Of The Quad (Right)

	glEnd();											// Done Drawing The Quad

glEndList();
}