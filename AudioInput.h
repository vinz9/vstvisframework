#ifndef AUDIOINPUT_H_
#define AUDIOINPUT_H_

#define BUFFERSIZE 2048
#define M_PI 3.14159265358979323846

#include <windows.h>

typedef struct {
    float cutoff;
    float a0;
    float b1;
    float b2;
    float x1;
    float x2;
} NOTCH_FILTER;

class AudioInput
{
	public:

	AudioInput();
	AudioInput(float sRate);
	~AudioInput();

	float sampleRate;

	float* buffer;

	float level;
	float* audiobars;
	float* temp_bars;
	float* magnitude;
	float halfLife;

	//C3 : 12
	//C4 : 24
	int maxFreq;
	float maxMagni;
	int getNote();

	int nof_bands;

	void peak();
	void fft();

	
	void initFilters(int nbands);
	void processFilters();

	bool hasBeenTriggered();
	bool isBeingTriggered();
	void updateTrigger();

	private:

	bool activated;
	bool activation;
	NOTCH_FILTER * bp_filter[32];

	void smbFft(float *fftBuffer, long fftFrameSize, long sign);

	float processNotch(NOTCH_FILTER * l, float x0);
	NOTCH_FILTER *  initNotch(float cutoff);
};

#endif