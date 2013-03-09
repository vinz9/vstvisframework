#include "fftBox.h"
#include <math.h>

//----------------------------------------------------------------------------
fftBox::fftBox()
{

}

//----------------------------------------------------------------------------
fftBox::fftBox(AudioEffect* effect):Object3d(effect), mStrength(1.0)
{

}

//----------------------------------------------------------------------------
fftBox::fftBox(AudioEffect* effect, float strength):Object3d(effect), mStrength(strength)
{

}

//----------------------------------------------------------------------------
fftBox::~fftBox()
{

}

//----------------------------------------------------------------------------
void fftBox::draw()
{
	
	float t = ((VstPlugin*)effect)->temps->getElapsedTime();
	
	glPushMatrix();
	glTranslatef(0,-2,-8.0);
	glRotatef(-40.0*cos(t/2.0),1.0,0.0,0.0);
	glRotatef(-90.0*cos(t/2.0),0.0,1.0,0.0);
	//glRotatef(-90.0*cos(3.14/50.0),0.0,1.0,0.0);

	

	for (int i = 0; i<BUFFERSIZE*0.5; i++)
	{
		logfft[i][0] = 2*(log10((double)(1+i))/log10((double)(BUFFERSIZE*0.5)))-1;
		//logfft[i][1] = 0;
		logfft[i][1] = (magnitude[i])/200*mStrength;
	}
	
	glTranslatef(0,0,0.5);
	for (int i =0; i<4; i++)
	{
		drawSide();
		glRotatef(90.0,0.0,1.0,0.0);
		glTranslatef(0.5,0,0.5);
	}
	glPopMatrix();

}
//----------------------------------------------------------------------------
void fftBox::drawSide()
{
	glBegin(GL_QUADS);
	for (int i = 0; i<BUFFERSIZE/2-200; i++)
	{
		/*glNormal3f(0.0,-2*(logfft[i+1][1]-logfft[i][1]),2*(logfft[i+1][0]-logfft[i][0]));

		glVertex3f(-1.0,logfft[i][0],logfft[i][1]);
		glVertex3f(1.0,logfft[i][0],logfft[i][1]);
		glVertex3f(1.0,logfft[i+1][0],logfft[i+1][1]);
		glVertex3f(-1.0,logfft[i+1][0],logfft[i+1][1]);*/

		glNormal3f(-logfft[i][1]*(logfft[i+1][0]-logfft[i][0]),0,logfft[i+1][0]-logfft[i][0]);

		glVertex3f(-0.5,logfft[i][0],0.0);
		glVertex3f(0.0,logfft[i][0],logfft[i][1]);
		glVertex3f(0.0,logfft[i+1][0],logfft[i+1][1]);
		glVertex3f(-0.5,logfft[i+1][0],0.0);

		glNormal3f(logfft[i][1]*(logfft[i+1][0]-logfft[i][0]),0,logfft[i+1][0]-logfft[i][0]);

		glVertex3f(0.0,logfft[i][0],logfft[i][1]);
		glVertex3f(0.5,logfft[i][0],0.0);
		glVertex3f(0.5,logfft[i+1][0],0.0);
		glVertex3f(0.0,logfft[i+1][0],logfft[i+1][1]);		


	}

	glEnd();
}

void fftBox::setInput(int input)
{
	magnitude = (((VstPlugin*)effect)->audioInputs[input]->magnitude);
}