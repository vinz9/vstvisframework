#ifndef FFTBOX_H_
#define FFTBOX_H_

#define BUFFERSIZE 2048

#include "Object3d.h"


class fftBox : public Object3d
{

public:

	fftBox();
	fftBox(AudioEffect* effect);
	fftBox(AudioEffect* effect, float mStrength);
	~fftBox();

	void draw();
	void setInput(int input);

	float logfft[BUFFERSIZE/2][2];
	float* magnitude;	

private:

	float mStrength;

	void drawSide();

};

#endif