=========================================================
How to build this with the Microsoft compiler (msvc)
=========================================================
---------------------------------------------------------
A rushed, aggravation-fueled guide by Alexandre Gauthier
---------------------------------------------------------

Prerequisites
===============

* Install Python_ 2.X

* Install Scons_ (``easy_install scons`` should do the trick if you have
  distutils installed. If that fails to add 'scons' to your PATH or 
  PYTHONPATH correctly, install from website and save yourself the
  headaches.

* If you do not have it installed yet, install the Windows SDK_:
  It comes with the compilers and relevant header files you might need.

* Get SDL and relevant projects for msvc (i.e. the *-VC.zip files).
  SDL, SDL_image, SDL_mixer, SDL_ttf... extract in one location, i.e.
  `C:\SDL` and ensure relevant headers end up in C:\SDL\include, and that
  libraries end up in c:\SDL\lib, this is rather important.

Building
=========

1. Launch the Windows SDK command prompt

2. Set environment to match your liking, for instance:
   ``setenv /release /x86 /xp``
   would set up a release, 32 bits build targetting Windows XP.
   This is the build I am testing with.

3. Set environment variables
   From your configured command prompt, add the SDL libs to the include
   and lib paths:
   ``set INCLUDE=C:\SDL\include;%INCLUDE%``
   ``set LIB=C:\SDL\lib;%LIB%``

4. Still in the Windows SDK command prompt, build the project with
   ``scons``

Running the resulting binary
=============================

Either copy ``bacondemo.exe`` where the necessary SDL (and modules) dll
files may be found, or copy the dlls to the project root.

Notes
======

Some notes I compiled:

msvc and C99
-------------
Haha, no. Microsoft supports some sort of partial, retarded subset of the
C90 standard, which allows (among other things) for C++ style comments.

However, it still requires you to declare everything at the top of a
function. Among other things. This means I had to hack at Daniel's C99
code and I'm afraid I might have fuglified things a bit in the process.

stdbool.h
----------
Nope. C99 baby. It's not on windows. I made a mess of preprocessor macros
to have it use a stand in I stole from Apple's WebKit tree. It may be
found in ``compat/stdbool.h``.

GL/glext.h
-----------
This is meant to be used from the OpenGL reference website. Most unix
and unix-like systems seem to include it because the OpenGL extensions
are fairly prolific and their use is widespread. I have included the latest
(at time of writing) version from the OpenGL reference library, as
``compat/glext.h``, for convenience.

Bugs
=====
It's full of subtle ones. See the issue tracker, as always.

.. _Python: http://www.python.org
.. _Scons: http://www.scons.org/download.php
.. _SDK: http://msdn.microsoft.com/en-us/windows/bb980924
