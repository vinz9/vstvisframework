#include "littleTimer.h"
#include <windows.h>

littleTimer::littleTimer()
{
	QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);
	QueryPerformanceCounter((LARGE_INTEGER *) &performance_timer_start);
	resolution	= (float) (((double)1.0f)/((double)frequency));
}

littleTimer::~littleTimer()
{

}

float littleTimer::getElapsedTime()
{
	__int64 time;

	QueryPerformanceCounter((LARGE_INTEGER *) &time);
	return ( (float) ( time - performance_timer_start) * resolution)*1.0f;
}
