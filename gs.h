/*
  Copyright (c) 2011, Jorge Acereda Maciá
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:
        
  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
        
  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. Neither the name of the author nor the names of its contributors
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
*/
#if defined(_MSC_VER)
#include <assert.h>
#else
#include <inttypes.h>
#endif

typedef enum {
	GS_EVENT_UNKNOWN,
	GS_EVENT_NONE,
	GS_EVENT_RESIZE,
	GS_EVENT_POSITION,
	GS_EVENT_DOWN,
	GS_EVENT_UP,
	GS_EVENT_UNICODE,
	GS_EVENT_MOTION,
	GS_EVENT_CLOSE,
	GS_EVENT_DUMMY=((uintptr_t)~0)
} gseventtype;

typedef enum {
	GS_KEY_NONE = 0x40000000,
	GS_KEY_MOUSEWHEELUP, 
	GS_KEY_MOUSEWHEELDOWN,
	GS_KEY_MOUSELEFT,
	GS_KEY_MOUSERIGHT,
	GS_KEY_MOUSEMIDDLE,
	GS_KEY_A,
	GS_KEY_B,
	GS_KEY_C,
	GS_KEY_D,
	GS_KEY_E,
	GS_KEY_F,
	GS_KEY_G,
	GS_KEY_H,
	GS_KEY_I,
	GS_KEY_J,
	GS_KEY_K,
	GS_KEY_L,
	GS_KEY_M,
	GS_KEY_N,
	GS_KEY_O,
	GS_KEY_P,
	GS_KEY_Q,
	GS_KEY_R,
	GS_KEY_S,
	GS_KEY_T,
	GS_KEY_U,
	GS_KEY_V,
	GS_KEY_W,
	GS_KEY_X,
	GS_KEY_Y,
	GS_KEY_Z,
	GS_KEY_0,
	GS_KEY_1,
	GS_KEY_2,
	GS_KEY_3,
	GS_KEY_4,
	GS_KEY_5,
	GS_KEY_6,
	GS_KEY_7,
	GS_KEY_8,
	GS_KEY_9,
	GS_KEY_EQUAL,
	GS_KEY_MINUS,
	GS_KEY_RIGHTBRACKET,
	GS_KEY_LEFTBRACKET,
	GS_KEY_QUOTE,
	GS_KEY_SEMICOLON,
	GS_KEY_BACKSLASH,
	GS_KEY_COMMA,
	GS_KEY_SLASH,
	GS_KEY_PERIOD,
	GS_KEY_GRAVE,
	GS_KEY_KEYPADDECIMAL,
	GS_KEY_KEYPADMULTIPLY,
	GS_KEY_KEYPADPLUS,
	GS_KEY_KEYPADCLEAR,
	GS_KEY_KEYPADDIVIDE,
	GS_KEY_KEYPADENTER,
	GS_KEY_KEYPADMINUS,
	GS_KEY_KEYPADEQUALS,
	GS_KEY_KEYPAD0,
	GS_KEY_KEYPAD1,
	GS_KEY_KEYPAD2,
	GS_KEY_KEYPAD3,
	GS_KEY_KEYPAD4,
	GS_KEY_KEYPAD5,
	GS_KEY_KEYPAD6,
	GS_KEY_KEYPAD7,
	GS_KEY_KEYPAD8,
	GS_KEY_KEYPAD9,
	GS_KEY_RETURN,
	GS_KEY_TAB,
	GS_KEY_SPACE,
	GS_KEY_DELETE,
	GS_KEY_ESCAPE,
	GS_KEY_COMMAND,
	GS_KEY_SHIFT,
	GS_KEY_CAPSLOCK,
	GS_KEY_OPTION,
	GS_KEY_CONTROL,
	GS_KEY_RIGHTSHIFT,
	GS_KEY_RIGHTOPTION,
	GS_KEY_RIGHTCONTROL,
	GS_KEY_FUNCTION,
	GS_KEY_VOLUMEUP,
	GS_KEY_VOLUMEDOWN,
	GS_KEY_MUTE,
	GS_KEY_F1,
	GS_KEY_F2,
	GS_KEY_F3,
	GS_KEY_F4,
	GS_KEY_F5,
	GS_KEY_F6,
	GS_KEY_F7,
	GS_KEY_F8,
	GS_KEY_F9,
	GS_KEY_F10,
	GS_KEY_F11,
	GS_KEY_F12,
	GS_KEY_F13,
	GS_KEY_F14,
	GS_KEY_F15,
	GS_KEY_F16,
	GS_KEY_F17,
	GS_KEY_F18,
	GS_KEY_F19,
	GS_KEY_F20,
	GS_KEY_HELP,
	GS_KEY_HOME,
	GS_KEY_PAGEUP,
	GS_KEY_FORWARDDELETE,
	GS_KEY_END,
	GS_KEY_PAGEDOWN,
	GS_KEY_LEFTARROW,
	GS_KEY_RIGHTARROW,
	GS_KEY_DOWNARROW,
	GS_KEY_UPARROW,
	GS_KEY_SCROLL,
	GS_KEY_NUMLOCK,
	GS_KEY_CLEAR,
	GS_KEY_SYSREQ,
	GS_KEY_PAUSE,
	GS_KEY_CAMERA,
	GS_KEY_CENTER,
	GS_KEY_AT,
	GS_KEY_SYM,
	GS_KEY_MAX,
	GS_KEY_DUMMY=((uintptr_t)~0)
} gskey;

typedef struct _ev ev;

unsigned gsWidth();
unsigned gsHeight();
int gsMouseX();
int gsMouseY();
int gsPressed(gskey key);
int gsReleased(gskey key);
int gsShowKeyboard();
int gsHideKeyboard();

int evType(const ev *);
int evWidth(const ev *);
int evHeight(const ev *);
gskey evWhich(const ev *);
int evX(const ev *);
int evY(const ev *);
const char * evKeyName(const ev *);

// Provided by the program
int init(int argc, char ** argv);
void glinit();
int term();
int tick();
void draw();
void event(const ev * e);
