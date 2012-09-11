//	VSTGLEditor.h - Editor window for a VST plugin using OpenGL to handle all
//					the drawing.
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

#ifndef VSTGLEDITOR_H_
#define VSTGLEDITOR_H_

#include "aeffeditor.h"

#ifdef WIN32
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#elif MACX
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <AGL/agl.h>
	#include <Carbon/Carbon.h>
#endif

///	Editor window for VST plugins, using OpenGL to handle all the drawing.
/*!
	This is a fairly basic implementation - it'll set up an opengl context
	for you, and provides a couple of options (vertical sync, antialiasing)
	for that context, and the usual mouse and key event methods.

	It's derived from AEffEditor, so that class' methods are available, but
	note that VSTGLEditor subclasses should not use the open() and close()
	methods - to make the interface a bit more object-oriented, and hopefully,
	easier to use, subclasses should use guiOpen() and guiClose() instead.
	VSTGLEditor uses open() and close() to set up and clean up various
	platform-specific variables, and call the aforementioned methods when
	it's safe to do so.  This means you no longer have to put the ugly
	VSTGLEditor::open(ptr); line at the top of your open() method.

	If you're upgrading from the previous version of VSTGL, please make sure
	you move your open() and close() code to guiOpen() and guiClose(),
	respectively.  The other change is that idle() is no longer hardwired to
	update the gui - it's now up to your subclass to decide if you want to
	do it that way (I've included a simple message-based timer class to
	provide a better update frequency as an alternative).  To update the
	opengl context, call refreshGraphics() - this will make sure the context's
	set up, call draw(), and then swap the buffers to display your changes
	on screen.

	\sa Timer

	\todo Shared contexts?
	\todo Implement simple Mouse class (so we can hide and show the cursor,
	mainly).
	\todo Test key events.
 */
class VSTGLEditor : public AEffEditor
{
  public:
	///	Enum containing various flags to set certain properties of the window.
	typedef enum
	{
		DefaultFlags = 0,	///<Specifies a default opengl window.
		WaitForVerticalSync,///<Specifies that drawing may only take place between the monitor's vertical refreshes.
		Antialias2x,		///<Specifies the drawing should use 2x hardware antialiasing if the graphics card supports it.
		Antialias4x,		///<Specifies the drawing should use 4x hardware antialiasing if the graphics card supports it.
		Antialias6x			///<Specifies the drawing should use 6x hardware antialiasing if the graphics card supports it.
	} WindowFlags;

	///	Constructor.
	/*!
		\param effect Pointer to the plugin class.
		\param flags An OR-ed combination of flags to set certain attributes
		of the opengl window.  The antialias flags are mutually exclusive.
		Note: I have observed very high cpu usage on my windows laptop if I
		have multiple VSTGL windows open with the WaitForVerticalSync flag
		set.  I suspect it's probably a graphics card issue (mine's an ATI
		Mobility Radeon 9600/9700 series apparently), but not having another
		windows machine to test on I don't know for sure - it works fine on
		my macmini.  Also note that not all graphics cards can support
		hardware antialiasing - it might be safest to use 2x or 4x just in
		case.

		\sa WindowFlags
	 */
	VSTGLEditor(AudioEffect *effect, WindowFlags flags=DefaultFlags);
	///	Destructor.
	virtual ~VSTGLEditor();

	///	Returns the size of the editor, so the host can provide the correct-sized window.
	bool getRect(ERect **rect) {*rect = &_rect; return true;};
	///	Called by the host once the window's been created, so we can set stuff up in it.
	/*!
		We hide this from subclasses, because we need to do platform-specific
		stuff in it.  Subclasses can use guiOpen() to be informed of the
		editor's window is opened.

		DO NOT ATTEMPT TO OVERRIDE THIS!!!!  (The compiler probably won't
		throw up an error, but the opengl context will not be set up,
		resulting in a blank window, and possibly a crash.)
	 */
	bool open(void *ptr);
	///	Called when the editor's window is opened.
	virtual void guiOpen() = 0;
	///	Called by the host just before the window is destroyed/closed.
	/*!
		We hide this from subclasses, because we need to do platform-specific
		stuff in it.  Subclasses can use guiClose() to be informed of the
		editor's window being closed.

		DO NOT ATTEMPT TO OVERRIDE THIS!!!!  (The compiler probably won't
		throw up an error, but the opengl context will not be cleaned up,
		which could be very bad.)
	 */
	void close();
	///	Called when the editor's window is closed.
	virtual void guiClose() = 0;

	///	Called when there's a MouseDown event.
	/*!
		The buttons are numbered 1=left, 2=right, 3=middle.
	 */
	virtual void onMouseDown(int button, int x, int y) {};
	///	Called when there's a MouseMove event.
	virtual void onMouseMove(int x, int y) {};
	///	Called when there's a MouseUp event.
	/*!
		The buttons are numbered 1=left, 2=right, 3=middle.
	 */
	virtual void onMouseUp(int button, int x, int y) {};
	///	Called when there's a MouseWheel event.
	virtual void onMouseWheel(int wheel, int x, int y) {};
	///	Called when there's a KeyDown event (GL because the VST headers already use onKeyDown).
	/*!
		\todo test whether the key mapping's correct.
	 */
	virtual void onGLKeyDown(const VstKeyCode& key) {};
	///	Called when there's a KeyUp event (GL because the VST headers already use onKeyUp).
	/*!
		\todo test whether the key mapping's correct.
	 */
	virtual void onGLKeyUp(const VstKeyCode& key) {};

	///	This is the method where everything gets drawn.
	/*!
		It's into here that you'll want to put your drawing code in
		subclasses.
	 */
	virtual void draw() = 0;
	///	This method wraps draw().
	/*!
		This method performs necessary setup and cleanup for a draw()
		operation - what it does is call setupContext(), draw(),
		swapBuffers().  If you want to update your graphics repeatedly, the
		simplest thing to do is just stick this in idle().
	 */
	void refreshGraphics();

	///	You'd call this from your plugin if you wanted to update the gui when your parameters change.
	/*!
		\todo Is it really necessary to have this in VSTGLEditor?
	 */
	virtual void setParameter(int index, float value) {};

	///	Returns the x position of the window/context.
	int getX() const {return _rect.left;};
	///	Returns the y position of the window/context.
	int getY() const {return _rect.top;};
	///	Returns the width of the window/context.
	int getWidth() const {return _rect.right-_rect.left;};
	///	Returns the height of the window/context.
	int getHeight() const {return _rect.bottom-_rect.top;};

	HWND tempHWnd;
	HDC dc;
#ifdef WIN32
	///	Windows: Message loop - we use this to intercept mouse messages, among other things.
	static LONG WINAPI GLWndProc(HWND hwnd,
								 UINT message,
								 WPARAM wParam,
								 LPARAM lParam);
#endif
#ifdef MACX
	///	OSX: Message loop - we use this to intercept mouse messages, among other things.
	static pascal OSStatus macEventHandler(EventHandlerCallRef handler,
										   EventRef event,
										   void *userData);

	///	OSX: Used to update our context's bounds if the host changes them.
	void updateBounds(int x, int y);
	///	OSX: Returns the boundsX value.
	int getBoundsX() const {return boundsX;};
	///	OSX: Returns the boundsY value.
	int getBoundsY() const {return boundsY;};

	///	OSX: Returns our WindowRef.
	WindowRef getWindowRef() const {return window;};
#endif
  protected:
	///	Helper method, needs to be called before any drawing takes place.
	void setupContext();
	///	Helper method, needs to be called after drawing has taken place.
	void swapBuffers();

	///	Call this to set your window's bounds in subclass constructors.
	void setRect(int x, int y, int width, int height);
  private:
	///	Used to create the actual window when open() is called.
	/*!
		Only necessary on Windows (and only really for Tracktion, which does
		things kind of strange).
	 */
	void createWindow();

	///	Helper method:  Used to setup Vertical Sync.
	/*!
		Checks if Vertical Sync is supported, and switches it on if it is.
		This method is only called if the user specified WaitForVerticalSync
		as one of the flags to the class' constructor.
	 */
	void setupVSync();
	///	Helper method: Tries to set up hardware antialiasing.
	/*!
		Checks if hardware antialiasing is supported, and switches it on if
		it is.  This method is only called if the user specified one of the
		Antialias WindowFlags in the class' constructor.

		Because of the different way the OS' work, this only sets up
		antialiasing on Windows - for OSX it's done in open().
	 */
	void setupAntialiasing();

#ifdef WIN32
	///	Windows: Windows rendering context.
	HGLRC glRenderingContext;
	///	Windows: Holds a handle to the window we created in createWindow().
	
	///	Windows: Device context for our window.
	//HDC dc;
	///	Windows: The pixel format we want to use for the window.
	PIXELFORMATDESCRIPTOR pixelformat;
#endif
#ifdef MACX
	///	OSX: OS X rendering context.
	AGLContext context;
	///	OSX: The pixel format we used.
	/*!
		\todo Should make this cross-platform, and allow subclasses to set
		it(?).
	 */
	AGLPixelFormat pixels;
	///	OSX: Saved in setupContext(), to be used in swapBuffers().
	GrafPtr port;
	///	OSX: Reference to our event handler.
	EventHandlerRef eventHandlerRef;
	///	Our window.
	WindowRef window;
	///	OSX: Our context's actual bounds (x).
	int boundsX;
	///	OSX: Our context's actual bounds (y).
	int boundsY;

	///	OSX: The control class reference for our dummy HIView.
	ControlDefSpec controlSpec;
	///	OSX: Id for our dummy HI class.
	CFStringRef classId;
	///	OSX: Our dummy HIView (necessary for VST 2.4).
	ControlRef controlRef;
#endif

	///	The rect our opengl context is contained within.
	ERect _rect;

	///	Whether or not to use Vertical Sync.
	bool useVSync;
	///	The level of antialiasing to use.
	int antialiasing;
};

#ifdef WIN32
//Why aren't these in a system header?
#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C
#define WGL_SAMPLE_BUFFERS_ARB		   0x2041
#define WGL_SAMPLES_ARB				   0x2042
#endif

//Documentation Main Page.
/*!
	\mainpage
	VSTGL is a simple AEffEditor subclass which provides basic opengl support
	for VST plugins on Windows and OSX.

	For this release I've refactored the code substantially, so you'll have to
	alter any code you had that used the previous version - it should be a lot
	nicer now though.  Make sure you read the VSTGLEditor documentation to
	see what's changed.  Major additions include a simple message-based timer,
	so you don't have to rely on the host's (generally rubbish, in my
	experience) idle() implementation for screen updates, and support for
	vertical sync and hardware antialiasing.

	This version is also compatible with VST 2.4 (while remaining
	backward-compatible with VST 2.3).

	- Niall Moody (04/03/2006).
 */

#endif
