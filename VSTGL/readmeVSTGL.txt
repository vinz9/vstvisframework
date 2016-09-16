VSTGL OpenGL Framework
----------------------
- by Niall Moody.

(http://www.niallmoody.com/ndcplugs/vstgl.htm)

Description
-----------
VSTGL is a simple AEffEditor subclass which provides basic opengl support for
VST plugins on Windows and OSX.

Usage
-----
The ExampleEditor class should give you an idea of how to use it.  I've also
included doxygen-generated documentation in the docs folder, which you should
definitely read.  Notably, the code's been refactored a lot, so it's not
backwards-compatible with the previous VSTGL release - make sure you check
out the documentation to see what's changed.

ToDo
----
- Check if the key events are translated properly.
- Add support for shared gl contexts?

ChangeLog
---------
v1.50 - Significant code refactoring, added simple Timer class, support for
	vertical sync and hardware antialiasing, updated for VST 2.4.
v1.00 -	First Release.

License
-------
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
