GLCV
====

## 1) Purpose
  GLCV is a small canvas/event handling library that provides a thin
  layer abstracting the details of configuring the window and handling
  input. Sort of a simple GLUT/SDL. No API is provided for
  sound/fonts/widgets, there are good solutions out there for those and
  freedom of choice is a good thing.

## 2) Building

### 2.1) IOS
  The following command should build the library and the examples and
  install them in the IOS simulator directory.
    ./mk-ios

### 2.2) Android
  The following command should build the library and the examples and
  install them in the Android emulator. The emulator must be running.
    ./mk-android

### 2.3) NPAPI
  The following command should build the library and the examples and
  install them in the user's plugins directory.
    ./mk-npapi
  After building, try opening [file:testplugin.html]

### 2.4) Xlib
  To compile the test application, use this command
    ./mk-xlib

### 2.5) Cocoa
  To compile the test application, use this command
    ./mk-cocoa

### 2.6) Windows
  To compile the test application, use this command
    mk-win

## 3) API

### 3.1) Functions

#### 3.1.1) Application hook
    intptr_t event(const ev * e);
  This is the event handling routine implemented by your application.

#### 3.1.2) Application control
    void cvQuit();
  Call this to terminate your application.

#### 3.1.3) Window properties
    unsigned cvWidth();
  Current window width
    unsigned cvHeight();
  Current window height
    Mouse location
    int cvMouseX();
  Current X mouse position
    int cvMouseY();
  Current Y mouse position

#### 3.1.4) Keyboard
    int cvPressed(cvkey key);
  True if key is currently pressed
    int cvReleased(cvkey key);
  True if key has just been released
    int cvShowKeyboard();
  On platforms that support it, show the on-screen keyboard
    int cvHideKeyboard();
  On platforms that support it, hide the on-screen keyboard

#### 3.1.5) Logging
    void cvReportV(const char * fmt, va_list ap);
  Report a message (depending on the platform, it might go to a file).
    void cvReport(const char * fmt, ...);
  Report a message (depending on the platform, it might go to a file).

#### 3.1.6) Events
    int evType(const ev *);
  Event type.
    const char * evName(const ev *);
  Event name.
    int evWidth(const ev *);
  Width for CVE_RESIZE events.
    int evHeight(const ev *);
  Width for CVE_RESIZE events.
    cvkey evWhich(const ev *);
  Keycode associated to CVE_DOWN/CVE_UP events.
    int evX(const ev *);
  Mouse X position for CVE_DOWN/CVE_UP events.
    int evY(const ev *);
  Mouse Y position for CVE_DOWN/CVE_UP events.
    int evArgC(const ev *);
  Argument count for CVE_INIT events.
    char ** evArgV(const ev *);
  Argument array for CVE_INIT events.
    intptr_t evArg0(const ev *);
  First event argument, mostly internal.
    intptr_t evArg1(const ev *);
  Second event argument, mostly internal.
    intptr_t cvInject(cveventtype, intptr_t, intptr_t);
  Inject an event, mostly internal.
    const char * keyName(cvkey);
  Key name for CVE_DOWN/CVE_UP events.

### 3.2) Events

#### 3.2.1) CVE_NONE
  Null event, must be ignored.

#### 3.2.2) CVE_INIT
  Called upon initialization, no GL operations should be performed at this
stage.

#### 3.2.3) CVE_TERM
  Called before terminating the application.

#### 3.2.4) CVE_GLINIT
  The OpenGL context has been established. Might be called several times.

#### 3.2.5) CVE_GLTERM
  The OpenGL context is going to be destroyed. Might be called several times.

#### 3.2.6) CVE_UPDATE
  This is your update routine, should be called at ~60Hz.

#### 3.2.7) CVE_RESIZE
  Called when the window is resized. Might be called several times.

#### 3.2.8) CVE_DOWN
  A key/button has been pressed.

#### 3.2.9) CVE_UP
  A key/button has been released.

#### 3.2.10) CVE_UNICODE
  Called when the user enters text.

#### 3.2.11) CVE_MOTION
  The mouse moved.

#### 3.2.12) CVE_CLOSE
  The application is about to close.

#### 3.2.13) CVE_INVOKE
  Invoke method. The argument is available as evMethod().

### 3.3) Queries

#### 3.3.1) CVQ_NAME
  Return the desired application name.

#### 3.3.2) CVQ_LOGGER
  Function to be used for logging. The returned function prototype should be:
    void report(const char * appname, const char * string);

#### 3.3.3) CVQ_XPOS
  Return the desired X position. This is just a hint and can be ignored
  on some platforms.

#### 3.3.4) CVQ_YPOS
  Return the desired Y position. This is just a hint and can be ignored
  on some platforms.

#### 3.3.5) CVQ_WIDTH
  Return the desired width. Note that this is a hint and can be ignored
  on some platforms. Your code should use the width reported via
  CVE_RESIZE or ask explicitly with cvWidth().
  If this query isn't handled, the window will cover the whole desktop.

#### 3.3.6) CVQ_HEIGHT
  Return the desired height. Note that this is a hint and can be ignored
  on some platforms. Your code should use the height reported via
  CVE_RESIZE or ask explicitly with cvHeight().

#### 3.3.7) CVQ_BORDERS
  Should the window have borders?

## 4) Examples

### 4.1) Simple example that just draws
    #include "cv.h"
    #include "cvgl.h"
    static void update() {
            static int i = 128;
            i++;
            glClearColor(((2*i) & 0xff) / 255.0f, 0.f, 1.f, (i & 0xff) /
          255.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
    }
    intptr_t event(const ev * e) {
            intptr_t ret = 1;
            cveventtype t = evType(e);
            switch (t) {
            case CVE_UPDATE: update(); break;
            default: ret = 0; break;
            }
            return ret;
    }
    /* 
       Local variables: **
       c-file-style: "bsd" **
       c-basic-offset: 8 **
       indent-tabs-mode: nil **
       End: **

  * /


### 4.2) Complete example handling all queries/events
    #include "cv.h"
    #include "cvgl.h"
    static void init() {
            cvReport("init");
    }
    static void term() {
            cvReport("term");
    }
    static void glinit() {
            cvReport("glinit");
    }
    static void down(cvkey k) {
            cvReport("down %s", keyName(k));
    }
    static void up(cvkey k) {
            cvReport("up %s", keyName(k));
            switch (k) {
            case CVK_ESCAPE: cvQuit(); break;
            default: break;
            }
    }
    static void unicode(uint32_t c) {
            cvReport("unicode %c", c);
    }
    static void motion(int x, int y) {
            cvReport("motion %d %d", x, y);
    }
    static void close() {
            cvReport("close");
            cvQuit();
    }
    static void invoke(const char * s) {
            cvReport("invoked method '%s'", s);
    }
    static void resize(unsigned w, unsigned h) {
            cvReport("resize %d %d", w, h);
            glViewport(0, 0, w, h);
    }
    static void update() {
            static int i = 128;
            i++;
            glClearColor(((2*i) & 0xff) / 255.0f, 0.f, 1.f, (i & 0xff) /
          255.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFlush();
    }
    intptr_t event(const ev * e) {
            intptr_t ret = 1;
            cveventtype t = evType(e);
            if (t != CVE_UPDATE)
                    cvReport("got event %s, %p %p", 
                             evName(e), evArg0(e), evArg1(e));
            switch (t) {
            case CVQ_NAME: ret = (intptr_t)"test"; break;
            case CVQ_XPOS: ret = 50; break;
            case CVQ_YPOS: ret = 50; break;
            case CVQ_WIDTH: ret = 640; break;
            case CVQ_HEIGHT: ret = 480; break;
            case CVQ_BORDERS: ret = 1; break;
            case CVE_INIT: init(); break;
            case CVE_TERM: term(); break;
            case CVE_GLINIT: glinit(); break;
            case CVE_DOWN: down(evWhich(e)); break;
            case CVE_UP: up(evWhich(e)); break;
            case CVE_UNICODE: unicode(evUnicode(e)); break;
            case CVE_MOTION: motion(evX(e), evY(e)); break;
            case CVE_CLOSE: close(); break;
            case CVE_INVOKE: invoke(evMethod(e)); break;
            case CVE_RESIZE: resize(cvWidth(e), cvHeight(e)); break;
            case CVE_UPDATE: update(); break;
            default: ret = 0; break;
            }
            return ret;
    }
    /* 
       Local variables: **
       c-file-style: "bsd" **
       c-basic-offset: 8 **
       indent-tabs-mode: nil **
       End: **

  * /


## 5) License
    Copyright (c) 2011-2012, Jorge Acereda Maciá
    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in
       the documentation and/or other materials provided with the
       distribution.
Neither the name of the author nor the names of its contributors
       may be used to endorse or promote products derived from this
       software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
