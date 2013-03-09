#include <math.h>
#include <time.h>

#include "Flower.h"

#define M_PI 3.14159265358979323846

Vect kukka[7]={
    {0.0,0.5,0.0},
    {0.0,0.0,0.0},
    {0.1,-0.2,0.0},
    {0.3,-0.3,0.0},
    {0.5,-0.3,0.0},
    {0.6,-0.2,0.0},
    {0.6,-0.5,0.0}};

Vect kukka_morph[7]={
    {0.0,0.5,0.0},
    {0.0,0.0,0.0},
    {0.2,-0.2,0.0},
    {0.3,-0.15,0.0},
    {0.2,0.0,0.0},
    {0.1,0.0,0.0},
    {0.0,0.1,0.0}};

//----------------------------------------------------------------------------
Flower::Flower()
{

}

//----------------------------------------------------------------------------
Flower::Flower(AudioEffect* effect, int m, float stg1, float stg2):Object3d(effect), mode(m), st1(stg1), st2(stg2)
{
	def = 0;

	montime = 0;
	
	tension = continuity = bias = 0.0;
	tension_new = continuity_new = bias_new = 0.0;

	spd = 2.5;
	posz = posz_new = 0.0;

	timeinput = 0;


	freq = -1;
	np = 12;

	polmode = 0;

	nbands = 16;

}

//----------------------------------------------------------------------------
Flower::~Flower()
{

}

#define F 0.006
#define FR 0.006

//----------------------------------------------------------------------------
void Flower::draw()
{

	if (params[1] == NULL)
	{
	montime = ((VstPlugin*)effect)->temps->getElapsedTime()*500;
	}
	else
	{
		timeinput = timeinput + *params[1] * st2;
		montime = ((VstPlugin*)effect)->temps->getElapsedTime()*100;
	}

	glPushMatrix();
	posz=FR*posz_new+(1.0-FR)*posz;

	glRotatef(-90,1.0,0.0,0.0);

	//glRotatef(montime*0.002+audiobars[1]*14,0.0,0.0,1.0);

	int t;
	
	int time = montime + timeinput;

	tension=F*tension_new+(1.0-F)*tension;
	continuity=F*continuity_new+(1.0-F)*continuity;
	bias=F*bias_new+(1.0-F)*bias;

	//def = ;

	if (mode == 0)
	{
		freq = ((VstPlugin*)effect)->audioInputs[in]->getNote();
		updateActive();
		for (t=0; t<np; t++) 
		{
			glRotatef(360.0/(float)np,0.0,0.0,1.0);
			spline3DMorph(sin(time*0.001*spd)*0.5+0.5, (6.0)*(*params[0])*st1*active[t]);
		}

	}

	else
	{
		for (t=0; t<np; t++) 
		{
			glRotatef(360.0/(float)np,0.0,0.0,1.0);
			spline3DMorph(sin(time*0.001*spd)*0.5+0.5,
				(6.0)*audiobars[(t*(nbands/4)) % nbands]*st1);
				//(6.0)*audiobars[0]*st1);
		}
	}

	 //roty+=(audiobars[15])*0.6;

	posz=0;

	glPopMatrix();


}
//----------------------------------------------------------------------------
void Flower::spline3DMorph(float factor, float poikkeama) {
#define NBTW 8
    int i,j,k;
    int size=sizeof(kukka)/sizeof(Vect);

    float vnyt,vnex;
    //float ti=SDL_GetTicks();
	int ti=montime;
    Vect r,r_morph;
    float rf[17*NBTW*3];
    int c;
    c=0;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

	for (i=0; i<size-3; i++) {
		for (j=0; j<NBTW; j++) {
			splineTCP((float)j/NBTW, &kukka[i], &r);
			splineTCP((float)j/NBTW, &kukka_morph[i], &r_morph);
			for (k=0; k<3; k++)
				rf[c*3+k]=(1.0-factor)*r[k]+factor*r_morph[k];

			rf[c*3+2]=sin(c*M_PI/(NBTW*4))*0.07;
			c+=1;

		}
	}
	for (i=0; i<(c-1); i++) {
		vnyt=(float)i/(float)(c-1)*(float)(size-3);
		vnex=(float)(i+1.0)/(float)(c-1)*(float)(size-3);

		rf[i*3]+=+sin((vnyt*2+ti*0.006))*0.02*poikkeama;
		rf[i*3+1]+=+sin((vnyt*2+ti*0.006))*0.02*poikkeama;
		rf[i*3+3]+=sin((vnex*2.1+ti*0.006))*0.02*poikkeama;
		rf[i*3+3+1]+=+sin((vnex*2.0+ti*0.006))*0.02*poikkeama;

		if (polmode == 0 || polmode == 2) glColor3f(1.0,1.0,1.0);
		else glColor3f(0.0,0.0,0.0);

			glPolygonOffset(3.0,2.0);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glBegin(GL_POLYGON);
			glVertex3f(1.0*rf[i*3],-rf[i*3+2],rf[i*3+1]);
			glVertex3f(1.0*rf[i*3+3],-rf[i*3+3+2],rf[i*3+3+1]);
			glVertex3f(1.0*rf[i*3+3],+rf[i*3+3+2],rf[i*3+3+1]);
			glVertex3f(1.0*rf[i*3],+rf[i*3+2],rf[i*3+1]);
			glEnd();
		

		glPolygonOffset(-1.0,-5.0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glLineWidth(2.0);
		glColor3f(0.0,0.0,0.0);

		if (polmode == 1)
		{
			glLineWidth(1.0);
			glColor3f(1.0,1.0,1.0);
		}
		else if (polmode == 2) glLineWidth(1.0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(1.0*rf[i*3],-rf[i*3+2],rf[i*3+1]*1);
		glVertex3f(1.0*rf[i*3+3],-rf[i*3+3+2],rf[i*3+3+1]*1);
		glVertex3f(1.0*rf[i*3+3],+rf[i*3+3+2],rf[i*3+3+1]*1);
		glVertex3f(1.0*rf[i*3],+rf[i*3+2],rf[i*3+1]*1);
		glEnd();
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------
void Flower::splineTCP(float u, Vect * control, Vect * result)
{

    (*result)[0] = (2*u*u*u - 3*u*u + 1)*control[1][0]
	+ (-2*u*u*u + 3*u*u)*control[2][0]
	+ (u*u*u - 2*u*u + u)*(0.5*(1-tension)*((1+bias)*(1-continuity)*(control[1][0]-control[0][0])
						+ (1-bias)*(1+continuity)*(control[2][0]-control[1][0])))
	+ (u*u*u - u*u)*(0.5*(1-tension)*((1+bias)*(1+continuity)*(control[2][0]-control[1][0])
					  + (1-bias)*(1-continuity)*(control[3][0]-control[2][0])));

    (*result)[1]
	= (2*u*u*u - 3*u*u + 1)*control[1][1]
	+ (-2*u*u*u + 3*u*u)*control[2][1]
	+ (u*u*u - 2*u*u + u)*(0.5*(1-tension)*((1+bias)*(1-continuity)*(control[1][1]-control[0][1])
						+ (1-bias)*(1+continuity)*(control[2][1]-control[1][1])))
	+ (u*u*u - u*u)*(0.5*(1-tension)*((1+bias)*(1+continuity)*(control[2][1]-control[1][1])
					  + (1-bias)*(1-continuity)*(control[3][1]-control[2][1])));

}

//----------------------------------------------------------------------------
void Flower::setInput(int input)
{
	audiobars = (((VstPlugin*)effect)->audioInputs[input]->audiobars);
	in = input;
}

//-------------------------------------------------------
void Flower::updateActive()
{
	for (int i = 0; i<12; i++)
	{
		active[i] = 0;
	}

	switch (freq)
	{
	case 0 :
		active[0] = active[4] = active[8] = 1;
		break;
	case 1 :
		active[1] = active[5] = active[9] = 1;
		break;
	case 2 :
		active[2] = active[6] = active[10] = 1;
		break;
	case 3 :
		active[3] = active[7] = active[11] = 1;
		break;

	case 4 :
		active[0] = active[3] = active[6] = active[9] = 1;
		break;
	case 5 :
		active[1] = active[4] = active[7] = active[10] = 1;
		break;
	case 6 :
		active[2] = active[5] = active[8] = active[11] = 1;
		break;

	case 7 :
		active[0] = active[2] = active[4] = active[6] = active[8] = active[10] = 1;
		break;
	case 8 :
		active[1] = active[3] = active[5] = active[7] = active[9] = active[11] = 1;
		break;

	case 9 :
		active[1] = active[5] = active[7] = active[11] = 1;
		break;
	case 10 :
		active[2] = active[4] = active[8] = active[10] = 1;
		break;

	case 11 :
		active[1] = active[2] = active[4] = active[5] = active[7] = active[8] = active[10] = active[11] = 1;
		break;
	}

}