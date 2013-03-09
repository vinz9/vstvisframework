//	VSTGLTimer.h - Simple message-based timer class.
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

#ifndef VSTGLTIMER_H_
#define VSTGLTIMER_H_

#ifdef WIN32
#include <windows.h>
#include <map>
#elif MACX
#include <Carbon/Carbon.h>
#endif

//----------------------------------------------------------------------------
///	Simple message-based timer class.
/*!
	By Timer, I mean it repeatedly calls timerCallback(), every interval
	milliseconds.

	Note that this relies on the operating system's simple message-based
	timers, and therefore is not going to be absolutely accurate - generally
	a 10msec interval is the best you can hope for, and it may fluctuate if
	the event queue is particularly busy.
 */
class Timer
{
  public:
	///	Constructor.
	/*!
		\param interval The time (in milliseconds) between calls to
		timerCallback.  Generally 10 msecs is the shortest interval that is
		realistically possible.
	 */
	Timer(int interval);
	///	Destructor.
	/*!
		Will call stop() if the timer is still running.
	 */
	virtual ~Timer();

	///	Call this to start the timer running.
	/*!
		\return True if it was successful, false otherwise.
	 */
	bool start();
	///	Call this to stop the timer running.
	void stop();
	///	Returns whether or not the timer is currently running.
	bool isRunning() const {return running;};

	///	Called at every interval.
	/*!
		Override this with the code you want called periodically.
	 */
	virtual void timerCallback() = 0;

#ifdef WIN32
	///	Windows: Windows callback function, calls timerCallback().
	static void  __stdcall timerProc(HWND hWnd,
									 unsigned int uMsg,
									 unsigned int idEvent,
									 unsigned long dwTime);
#elif MACX
	///	OSX: OSX callback function, calls timerCallback().
	static pascal void timerProc(EventLoopTimerRef theTimer,
									void *userData);
#endif
  private:
#ifdef WIN32
	///	Windows: Timer ID, used to kill the timer in the stop() method.
	int timerId;
#elif MACX
	///	OSX: Pointer to the timer.
	EventLoopTimerRef timer;
#endif

	///	The interval the timer is running at.
	int timerInterval;
	///	Whether or not the timer is currently running.
	bool running;
};

//----------------------------------------------------------------------------
#ifdef WIN32
///	Windows-only:  Used to work out which Timer::timerCallback to call for a given timerId.
class TimerSingleton
{
  public:
	///	Returns the single instance of TimerSingleton.
	static TimerSingleton& getInstance();

	///	Timer calls this to register a timerId and Timer instance with us.
	/*!
		\param timer Pointer to the timer instance in question.
		\param timerId The id for the timer instance in question.
	 */
	void registerTimer(Timer *timer, int timerId);
	///	Removes the timerId for a specific instance of Timer.
	void unRegisterTimer(int timerId);

	///	Called from Timer::timerProc() to determine which Timer::timerCallback() to call.
	/*!
		\retval Pointer to the correct Timer if successful, 0 otherwise.
	 */
	Timer *getTimer(int timerId) const;

	///	Stupid MSVC6 not letting me use a private destructor!!!!
	~TimerSingleton();
  private:
	///	Constructor.
	TimerSingleton();

	///	map holding all the currently running timers.
	std::map<int, Timer *> timers;
};
#endif

#endif
