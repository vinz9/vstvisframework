#include <math.h>
#include <stdlib.h>
#include "AudioInput.h"
//#include "fft/FFTRealFixLen.h"

//void smbFft(float *fftBuffer, long fftFrameSize, long sign);

//----------------------------------------------------------------------------
AudioInput::AudioInput(float sRate):level(0.0f),sampleRate(44100.0f),halfLife(0.01f),maxFreq(0),maxMagni(0)
{
	buffer = new float[BUFFERSIZE];
	magnitude = new float[BUFFERSIZE/2];
	audiobars = NULL;
	temp_bars = NULL;
}

//----------------------------------------------------------------------------
AudioInput::AudioInput():level(0.0f),sampleRate(44100.0f),halfLife(0.01f),maxFreq(0),maxMagni(0)
{
	buffer = new float[BUFFERSIZE];
	magnitude = new float[BUFFERSIZE/2];
	audiobars = NULL;
	temp_bars = NULL;
}

//----------------------------------------------------------------------------
AudioInput::~AudioInput()
{
	if (buffer)
	delete[] buffer;

	if (magnitude)
		delete[] magnitude;

	if (audiobars != NULL)
		delete[] audiobars;

	if (temp_bars != NULL)
	{
		delete[] temp_bars;

	 for (int i=0; i<nof_bands; i++) 
	{
		
		delete bp_filter[i];
	}
	}


}

//----------------------------------------------------------------------------
void AudioInput::peak()
{
	float scalar = pow( 0.5, 1.0/(halfLife * sampleRate));
	float in = 0;

	for(int i = 0; i<BUFFERSIZE; i++)
	{	
		in = fabs(buffer[i]);
		if ( in >= level )
		{
			level = in;
		}
		else
		{
			level = level * scalar;
			if( level < 0.00001 ) level = 0.0;
		}
	}
}

//----------------------------------------------------------------------------
void AudioInput::fft()
{
	double window, imag, real;
	maxFreq = 0;
	maxMagni = 0.0;

	/*float out[BUFFERSIZE];

	FFTRealFixLen <11> fft_object; // 1024-point (2^10) FFT object constructed.

	for (int k = 0; k < BUFFERSIZE;k++) {
		window = -.5*cos(2.*M_PI*(double)k/(double)BUFFERSIZE)+.5;
		buffer[k] = buffer[k] * window;
	}

	fft_object.do_fft (out, buffer); 

	for (int k = 0; k <= BUFFERSIZE*0.5; k++) {

		real = out[k];
		imag = out[(BUFFERSIZE/2)-1+k];

		magnitude[k] = 2.*sqrt(real*real + imag*imag);

		if (magnitude[k] > maxMagni)
		{
			maxMagni = magnitude[k];
			maxFreq = k;
		}

	}*/

	 /* do windowing and re,im interleave */
	float gFFTworksp[2*BUFFERSIZE];
	
	for (int k = 0; k < BUFFERSIZE;k++) {
		window = -.5f*cos(2.*M_PI*k/BUFFERSIZE)+.5f;
		//window = 1;
		gFFTworksp[2*k] = buffer[k] * window;
		gFFTworksp[2*k+1] = 0.;
	}

	smbFft(gFFTworksp, BUFFERSIZE, -1);

	for (int k = 0; k < (0.5*BUFFERSIZE); k++) {

		real = gFFTworksp[2*k];
		imag = gFFTworksp[2*k+1];

		magnitude[k] = 2.*sqrt(real*real + imag*imag);

		if (magnitude[k] > maxMagni)
		{
			maxMagni = magnitude[k];
			maxFreq = k;
		}

	}
}

//----------------------------------------------------------------------------
NOTCH_FILTER *  AudioInput::initNotch(float cutoff)
{
    //NOTCH_FILTER * l=(NOTCH_FILTER*)malloc(sizeof(NOTCH_FILTER));

	NOTCH_FILTER * l= new NOTCH_FILTER;

    float steep = 0.999f;
    float r = steep * 0.99609375f;
    float f = cos(M_PI * cutoff / (sampleRate*0.5));
    l->cutoff = cutoff;
    l->a0 = (1 - r) * sqrt( r * (r - 4 * (f * f) + 2) + 1);
    l->b1 = 2 * f * r;
    l->b2 = -(r * r);

    l->x1 = 0.0;
    l->x2 = 0.0;
    return l;
}

//----------------------------------------------------------------------------
float AudioInput::processNotch(NOTCH_FILTER * l, float x0)
{
    float outp = l->a0 * x0 + l->b1 * l->x1 + l->b2 * l->x2;
    l->x2 = l->x1;
    l->x1 = outp;

	if (outp < 0.001) outp = 0.0;

    return outp;
}

//----------------------------------------------------------------------------
void AudioInput::initFilters(int nbands)
{
	float b;
	audiobars = new float[nbands];
	temp_bars = new float[nbands];
	nof_bands=nbands;
    for (int i=0; i<nof_bands; i++) 
	{
		b=80.0f+i*(22000.0f-80.0f)/nof_bands;
		bp_filter[i]=initNotch(b);
	}
}

#define HEIGHT 1.0
#define D 0.45
#define TAU 0.25
#define DIF 5.0

//----------------------------------------------------------------------------
void AudioInput::processFilters()
{
	float scalar = pow( 0.5f, 1.0f/(halfLife * sampleRate));

	float f;


	float lk=2.0;
	float l0=2.0;

	float scale = HEIGHT / ( log((1 - D) / D) * 2 );
	float x00 = D*D*1.0/(2 * D - 1);
	float y00 = -log(-x00) * scale;
	float y;

	for (int i=0; i<BUFFERSIZE; i++) {
		for (int b=0; b<nof_bands; b++)
		{
			//temp_bars[b]=0.0;

			f=processNotch(bp_filter[b],buffer[i]);

			/*if (fabs(f)>audiobars[b])
			{
				audiobars[b]=fabs(f);
			}
			else {
				audiobars[b] = audiobars[b] * scalar;
				if( audiobars[b] < 0.00001 ) audiobars[b] = 0.0;
			}*/

			if (fabs(f)>temp_bars[b])
			{
				temp_bars[b]=fabs(f);
			}

			else {
				temp_bars[b] = temp_bars[b] * scalar;
				if( temp_bars[b] < 0.00001 ) temp_bars[b] = 0.0;
			}

			if (temp_bars[b]>0)
			{
			y=temp_bars[b];
			y=y*(b*lk+l0);

			y = ( log(y - x00) * scale + y00 ); 

			y = ( (DIF-2.0)*y +
				(b==0       ? 0 : temp_bars[b-1]) +
				(b==31 ? 0 : temp_bars[b+1])) / DIF;

			y=(1.0-TAU)*audiobars[b]+TAU*y;
			audiobars[b]=y;
			}
			else audiobars[b]=temp_bars[b];

		}
	}
}

//----------------------------------------------------------------------------
bool AudioInput::hasBeenTriggered()
{
	if (level > 0.1) activation = TRUE;
	else
	{
		activation = FALSE; 
		activated = FALSE;
	}
	return activation && !activated;
}

//----------------------------------------------------------------------------
bool AudioInput::isBeingTriggered()
{
	if (level > 0.05) activation = TRUE;
	else
	{
		activated = FALSE;
	}
	return activation && !activated;
}

//----------------------------------------------------------------------------
void AudioInput::updateTrigger()
{
		activation = FALSE;
		activated = TRUE;
}

//----------------------------------------------------------------------------
int AudioInput::getNote()
{
	int freq = -1;

	if (6<=maxFreq && maxFreq<12)
	{
		freq = (maxFreq-6)*2.0;
	}
	if (12<=maxFreq && maxFreq<24)
	{
		freq = maxFreq-12;
	}
	if (24<=maxFreq && maxFreq<48)
	{
		freq = (maxFreq-24)*0.5;
	}
 return freq;
}

//-----------------------------------------------------------------------------------
void AudioInput::smbFft(float *fftBuffer, long fftFrameSize, long sign)
/* 
	FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	time domain data in fftBuffer[0...2*fftFrameSize-1]. The FFT array takes
	and returns the cosine and sine parts in an interleaved manner, ie.
	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
	must be a power of 2. It expects a complex input signal (see footnote 2),
	ie. when working with 'common' audio signals our input signal has to be
	passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
	of the frequencies of interest is in fftBuffer[0...fftFrameSize].
*/
{
	float wr, wi, arg, *p1, *p2, temp;
	float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	long i, bitm, j, le, le2, k;

	for (i = 2; i < 2*fftFrameSize-2; i += 2) {
		for (bitm = 2, j = 0; bitm < 2*fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		if (i < j) {
			p1 = fftBuffer+i; p2 = fftBuffer+j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
	}
	for (k = 0, le = 2; k < (long)(log((long double)fftFrameSize)/(long double)log((float)2.)); k++) {
		le <<= 1;
		le2 = le>>1;
		ur = 1.0;
		ui = 0.0;
		arg = M_PI / (le2>>1);
		wr = cos(arg);
		wi = sign*sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer+j; p1i = p1r+1;
			p2r = p1r+le2; p2i = p2r+1;
			for (i = j; i < 2*fftFrameSize; i += le) {
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			tr = ur*wr - ui*wi;
			ui = ur*wi + ui*wr;
			ur = tr;
		}
	}
}