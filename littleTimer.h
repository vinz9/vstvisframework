#ifndef LITTLETIMER_H_
#define LITTLETIMER_H_

class littleTimer

{

public:
	littleTimer();
	~littleTimer();

	float getElapsedTime();

  __int64       frequency;									// Timer Frequency
  float         resolution;									// Timer Resolution
  __int64       performance_timer_start;					// Performance Timer Start Value
  __int64       performance_timer_elapsed;					// Performance Timer Elapsed Time

};

#endif