PM Model Viewer

This project allows viewing most of the model files from the games:
- Paper Mario: The Thousand Year Door
- Super Paper Mario

To build:
Run the included make file (MingW + MSYS) or import the source files into
an IDE of your choice and build from there.  This project has dependencies
on SFML-1.5, which are not included due to license conflict.  Download
the SFML-1.5 SDK and set the include and library paths as appropriate to
your system.

To run:
drag and drop one of the model files onto the built EXE.  Make sure
that the model's corresponding texture file (if it exists) is in the same
directory.  Texture files end in a dash (-).

Keyboard commands:
Arrow Keys: Translate, Rotate, or Zoom depending on the current modes
T: Switch to translation
R: Switch to Rotate
Z: Switch to Zoom
C: Toggle between manipulating the World and Camera

This project is released under the MIT license.

Sample input files are copyright Nintendo.

---------------------------------------------------------------------

Copyright (c) 2009 Justin Aquadro

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.