                                 GLGS
                                 ====

Author: Jorge Acereda
Date: 2011-10-02 Sun


Table of Contents
=================
1 Purpose 
2 Building 
    2.1 IOS 
    2.2 NPAPI 


1 Purpose 
~~~~~~~~~~
GLGS is a small screen/event handling library that provides a thin
layer abstracting the details of configuring the screen and handling
input. Sort of a simple GLUT/SDL.

2 Building 
~~~~~~~~~~~
As of now, the build scripts only work on Mac OS X.

2.1 IOS 
========
The following command should build the library and the examples and install them in the IOS simulator directory.

  ./mk-ios

2.2 NPAPI 
==========
The following command should build the library and the examples and install them in the user's plugins directory.

  ./mk-npapi
After building, try opening [file:testplugin.html]
