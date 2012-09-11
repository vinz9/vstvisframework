#include "Alga.h"

GLfloat ctrlStart[6][3] = {
	{ 0.0, 0.0, 0.0}, { -0.2, 0.0, 0.0},
	{-1.5, 0.0, 0.0},{-2.0, 0.5, 0.0},
	{-1.5, 2.0, 0.0},{-1.0, 1.0, 0.0}};

GLfloat ctrlFinish[6][3] = {
	{ 0.0, 0.0, 0.0}, { -1.0, 0.0, 0.0}, 
	{-1.0, 2.0, 0.0},{0.0, 3.0, 0.0},
	{0.0, 4.0, 0.0},{0.0, 4.2, 0.0}};

GLfloat ctrlTemp[6][3];
GLfloat inc[6][3];

//----------------------------------------------------------------------------
Alga::Alga()
{
}

//----------------------------------------------------------------------------
Alga::Alga(AudioEffect* effect):Object3d(effect)
{

}

//----------------------------------------------------------------------------
Alga::Alga(AudioEffect* effect, int strength):Object3d(effect),st(strength)
{

	k = 0;
	steps = 100;
	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j <3; j++)
		{
			inc[i][j] = (ctrlFinish[i][j] - ctrlStart[i][j])/steps;
		}
	}
	attack = 1.0;

	glEnable(GL_MAP1_VERTEX_3);

	polmode = 0;
}

//----------------------------------------------------------------------------
Alga::~Alga()
{

	glDisable(GL_MAP1_VERTEX_3);
}

//----------------------------------------------------------------------------
void Alga::draw()
{

	//if (attack <1 && level0>0.1) attack = attack + 0.05;
	//else if (level0 < 0.1) attack =0;
	
	k = *params[0] * st * attack;
	//k = k + *params[0] * st;

	if (k<steps)
	{
		for (int i = 0; i<6; i++)
		{
			for (int j = 0; j <3; j++)
			{
				ctrlTemp[i][j] = (ctrlStart[i][j] + inc[i][j]*k);
			}
		}
		//k++;
	}
	/*else if (k<2*steps)	
	{
		for (int i = 0; i<6; i++)
		{
			for (int j = 0; j <3; j++)
			{
				ctrlTemp[i][j] = ctrlFinish[i][j] - inc[i][j]*(k-steps);
			}
		}
		//k++;
	}*/
	//else k = 0;
	float a = ((VstPlugin*)effect)->temps->getElapsedTime();
	ctrlTemp[2][0] = ctrlTemp[4][0]-0.6+0.6*sin(2*a);

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 6, &ctrlTemp[0][0]);
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 6, &ctrlStart[0][0]);

	int res = 20;

	glMapGrid1f(res, 0.0, 1.0);

	glPushMatrix();

	for (int j = 0; j <2; j++)
		{
			//glRotatef(180,0,0,1);
			for (int i = 0; i<24; i++) 
			{
				glRotatef(15,0,1,0);
				
				if (polmode == 0)
				{
					glColor3f(1.0, 1.0, 1.0);
					glLineWidth(2.0);
					glEvalMesh1(GL_LINE, 0, res);
				}
				else
				{


					glColor4f(1.0,1.0,1.0,0.2);

					glDisable(GL_DEPTH_TEST);
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);

					glEvalMesh1(GL_POINT, 0, res);

					glEnable(GL_DEPTH_TEST);
					glEnable(GL_LIGHTING);
					glDisable(GL_BLEND);
				}

				

				/*glPointSize(5.0);
				glColor3f(1.0, 1.0, 0.0);

				glBegin(GL_POINTS);
				for (int i = 0; i < 6; i++) 
				glVertex3fv(&ctrlTemp[i][0]);
				glEnd();
				
				glPointSize(2.0);
				glColor3f(1.0, 1.0, 1.0);*/
			}
		}

	glPopMatrix();
		
	

}
//----------------------------------------------------------------------------
void Alga::setInput(int input)
{

}