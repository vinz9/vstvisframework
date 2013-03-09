#include "RandomLine.h"


//----------------------------------------------------------------------------
RandomLine::RandomLine()
{

}

//----------------------------------------------------------------------------
RandomLine::RandomLine(AudioEffect* effect, float stg1):Object3d(effect),st1(stg1)
{

time = 0;
mtime = 0;
}

//----------------------------------------------------------------------------
RandomLine::~RandomLine()
{

}

//----------------------------------------------------------------------------
void RandomLine::draw()
{


	time = ((VstPlugin*)effect)->temps->getElapsedTime();

	mtime = mtime - *params[0]*st1;

	sample = 0;
	flip = 0;


	glPushMatrix();

    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();	

	//glOrtho(0.0,1.0,1.0,0.0,0.0,10.0);
	glOrtho(0.0,1.0,1.4,0.0,0.0,10.0);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);									
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);

glColor4f(1,1,1,0.3);
glLineWidth(4.0);
glPointSize(10.0);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);

	for (int i = 0; i<200; i++)
	{
		sample = sample+0.005;

		//drawSpiral();
		drawChaser();

		glVertex2f(x,y);
	}

	glEnd();


	glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glLineWidth(2.0);
	glPointSize(2.0);
		glEnable(GL_DEPTH_TEST);									
	glEnable(GL_LIGHTING);

	glDisable(GL_BLEND);

}

//----------------------------------------------------------------------------
void RandomLine::drawChaser()

{

	//vol=(bass+mid+treb)*0.25;
	//vol=vol*vol;

	//mtime=mtime+vol*0.01*(50/fps);

	q1=mtime*0.5;

	//zm=-1.015+(rad/(bass*60));
	
//--------------------
	
	n=sample*6.283;
	phs=-sample * 0.2;
	tm=q1 + phs*4;

	flip=flip+1;
	//flip=flip*below(flip,2);
	if (flip>=2)
	{
		flip = 0;
	}

	xp=0;
	yp=(flip*0.1-0.05)*(sample-1);
	//yp = 0.05*sample;
	zp=0;

	//spin;

	ang=tm*20 + sin(tm*76 + time*4)*0.4;
	sa=sin(ang);ca=cos(ang);
	xr=xp*sa + yp*ca;
	yr=xp*ca - yp*sa;
	zr=zp;

	xp=xr;
	yp=yr + 0.05 + (sin(tm)*0.5 + 0.5)*0.2 + 0.05;
	zp=zr;

	//wrist movement;
	ang=sin(tm*2);

	xq=xp;
	sa=sin(ang);ca=cos(ang);
	yq=yp*sa + zp*ca;
	zq=yp*ca - zp*sa;


	ang=tm*8;
	sa=sin(ang);ca=cos(ang);
	xp=xq*sa + yq*ca;
	yp=xq*ca - yq*sa;
	zp=zq;

	//forearm movement;
	zp=zp-0.3;
	ang=3.14 + sin(tm*2 - 0.5)*2.5;
	xq=xp;
	sa=sin(ang);ca=cos(ang);
	yq=yp*sa + zp*ca;
	zq=yp*ca - zp*sa;

	//upper arm twist
	ang=-1.0 + cos(tm*3 + 0.5);
	xp=xq*sa + yq*ca;
	yp=xq*ca - yq*sa;
	zp=zq;

	//upper arm outward;
	zp=zp-0.35;
	ang=cos(tm*1)*1.75 - 1.05;
	sa=sin(ang);ca=cos(ang);
	xq=xp*sa + zp*ca;
	yq=yp;
	zq=xp*ca - zp*sa;

	//upper arm up down;
	ang=cos(tm);
	xp=xq;
	sa=sin(ang);ca=cos(ang);
	yp=yq*ca - zq*sa;
	zp=yq*sa + zq*ca;

	//xp=xq;yp=yq;zp=zq;


	//project into screenspace and draw on screen
	zp=zp+1.5;
	xs=xp/zp;
	ys=yp/zp;

	x=xs+0.5;
	y=ys*1.3+0.5;


	//a=(1-sample);
}


//----------------------------------------------------------------------------
void RandomLine::drawSpiral()

{
		xp = 3.14*0.1*sample*cos(2*6.28*sample+2*time);
		yp = 3.14*0.1*sample*sin(6.28*sample+time);

		ang = 6.25*sin(0.5*time);
		xq = xp*cos(ang)-yp*sin(ang);
		yq = xp*sin(ang)+yp*cos(ang);
		xp = xq + 0.2*sin(2.0*time);

		x = xp + 0.5;
		y = yq + 0.5;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//repere
/*glColor3f(1.0,0.0,0.0);

	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0.5,0);	
	glEnd();

	glColor3f(0.0,1.0,0.0);

	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0.0,0.5);	
	glEnd();*/