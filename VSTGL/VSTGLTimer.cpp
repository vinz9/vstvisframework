//	VSTGLTimer.cpp - Simple message-based timer class.
//	--------------------------------------------------------------------------
//	Copyright (c) 2005-2006 Niall Moody
//	
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the "Software"),
//	to deal in the Software without restriction, including without limitation
//	the rights to use, copy, modify, merge, publish, distribute, sublicense,
//	and/or sell copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//	DEALINGS IN THE SOFTWARE.
//	--------------------------------------------------------------------------

#include "VSTGLTimer.h"

#ifdef WIN32
using namespace std;
#endif

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
Timer::Timer(int interval):
timerInterval(interval),
running(false)
{
	
}

//----------------------------------------------------------------------------
Timer::~Timer()
{
	if(running)
		stop();
}

//----------------------------------------------------------------------------
bool Timer::start()
{
	bool retval = false;

	if(running)
		return false;

#ifdef WIN32
	timerId = SetTimer(NULL, NULL, timerInterval ,timerProc);
	if(timerId)
	{
		retval = true;

		//Register with TimerSingleton.
		TimerSingleton::getInstance().registerTimer(this, timerId);
	}
#elif MACX
	OSStatus err;

	err = InstallEventLoopTimer(GetCurrentEventLoop(),
								1,
								(static_cast<double>(timerInterval)/1000.0),
								NewEventLoopTimerUPP(timerProc),
								this,
								&timer);
	if(err == noErr)
		retval = true;
#endif

	if(retval)
		running = true;

	return retval;
}

//----------------------------------------------------------------------------
void Timer::stop()
{
	if(running)
	{
#ifdef WIN32
		KillTimer(NULL, timerId);
		TimerSingleton::getInstance().unRegisterTimer(timerId);
#elif MACX
		RemoveEventLoopTimer(timer);
#endif
		running = false;
	}
}

#ifdef WIN32
//----------------------------------------------------------------------------
void  __stdcall Timer::timerProc(HWND hWnd,
								 unsigned int uMsg,
								 unsigned int idEvent,
								 unsigned long dwTime)
{
	Timer *tim = TimerSingleton::getInstance().getTimer(idEvent);

	if(tim)
		tim->timerCallback();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TimerSingleton& TimerSingleton::getInstance()
{
	static TimerSingleton tim;

	return tim;
}

//----------------------------------------------------------------------------
void TimerSingleton::registerTimer(Timer *timer, int timerId)
{
	timers.insert(make_pair(timerId, timer));
}

//----------------------------------------------------------------------------
void TimerSingleton::unRegisterTimer(int timerId)
{
	timers.erase(timerId);
}

//----------------------------------------------------------------------------
Timer *TimerSingleton::getTimer(int timerId) const
{
	map<int, Timer *>::const_iterator it;
	it = timers.find(timerId);

	if(it != timers.end())
		return it->second;
	else
		return 0;
}

//----------------------------------------------------------------------------
TimerSingleton::~TimerSingleton()
{
	
}

//----------------------------------------------------------------------------
TimerSingleton::TimerSingleton()
{
	
}

#elif MACX
//----------------------------------------------------------------------------
pascal void Timer::timerProc(EventLoopTimerRef theTimer, void *userData)
{
	Timer *tim = static_cast<Timer *>(userData);

	if(tim)
		tim->timerCallback();
}
#endif
