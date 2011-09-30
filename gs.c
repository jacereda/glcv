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
#include "gs.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_EVENTS 256
#define MAX_PRESSED 256

static unsigned s_w = 0;
static unsigned s_h = 0;
static int s_mx = 0;
static int s_my = 0;
static unsigned char s_pressed[MAX_PRESSED/8] = {0};
static unsigned char s_ppressed[MAX_PRESSED/8] = {0};

static __inline void bitset(unsigned char * b, unsigned bit) {
        b[bit>>3] |= 1 << (bit & 7);
}

static __inline void bitclear(unsigned char * b, unsigned bit) {
        b[bit>>3] &= ~(1 << (bit & 7));
}

static __inline void bitassign(unsigned char * b, unsigned bit, int val) {
	if (val)
		bitset(b, bit);
	else
		bitclear(b, bit);
}

static __inline int bittest(const unsigned char * b, unsigned bit) {
        return b[bit>>3] & (1 << (bit & 7));
}

unsigned gsWidth() {
	return s_w;
}

unsigned gsHeight() {
	return s_h;
}

int gsMouseX() {
	return s_mx;
}

int gsMouseY() {
	return s_my;
}

static unsigned char * bitarrayFor(unsigned char * p, gskey * k) {
        int good = *k >= GS_KEY_NONE && *k < GS_KEY_MAX;
        assert(good);
        *k -= GS_KEY_NONE;
        return good? p : 0;
}

static void press(gskey k) {
        unsigned char * ba = bitarrayFor(s_pressed, &k);
        if (ba)
                bitset(ba, k);
}

static void release(gskey k) {
        unsigned char * ba = bitarrayFor(s_pressed, &k);
        if (ba)
                bitclear(ba, k);
}

int gsPressed(gskey k) {
        const unsigned char * ba = bitarrayFor(s_pressed, &k);
        return ba && bittest(ba, k);
}

int gsReleased(gskey k) {
        gskey pk = k;
        const unsigned char * ba = bitarrayFor(s_pressed, &k);
        const unsigned char * pba = bitarrayFor(s_ppressed, &pk);
        return ba && !bittest(ba, k) && bittest(pba, k);
}

int evType(const ev * e) {
	return e->type;
}

int evWidth(const ev * e) {
	return e->p[0];
}

int evHeight(const ev * e) {
	return e->p[1];
}

gskey evWhich(const ev * e) {
	return e->p[0];
}

int evX(const ev * e) {
	return e->p[0];
}

int evY(const ev * e) {
	return e->p[1];
}

const char * evKeyName(const ev * e) {
	const char * n = 0;
	static char buf[32] = {0};
        gskey k = evWhich(e);
	switch (k) {
	case GS_KEY_NONE: n = "NONE"; break;
	case GS_KEY_MOUSEWHEELUP: n = "MOUSEWHEELUP"; break;
	case GS_KEY_MOUSEWHEELDOWN: n = "MOUSEWHEELDOWN"; break;
	case GS_KEY_MOUSELEFT: n = "MOUSELEFT"; break;
	case GS_KEY_MOUSEMIDDLE: n = "MOUSEMIDDLE"; break;
	case GS_KEY_MOUSERIGHT: n = "MOUSERIGHT"; break;
	case GS_KEY_A: n = "A"; break;
	case GS_KEY_S: n = "S"; break;
	case GS_KEY_D: n = "D"; break;
	case GS_KEY_F: n = "F"; break;
	case GS_KEY_H: n = "H"; break;
	case GS_KEY_G: n = "G"; break;
	case GS_KEY_Z: n = "Z"; break;
	case GS_KEY_X: n = "X"; break;
	case GS_KEY_C: n = "C"; break;
	case GS_KEY_V: n = "V"; break;
	case GS_KEY_B: n = "B"; break;
	case GS_KEY_Q: n = "Q"; break;
	case GS_KEY_W: n = "W"; break;
	case GS_KEY_E: n = "E"; break;
	case GS_KEY_R: n = "R"; break;
	case GS_KEY_Y: n = "Y"; break;
	case GS_KEY_T: n = "T"; break;
	case GS_KEY_1: n = "1"; break;
	case GS_KEY_2: n = "2"; break;
	case GS_KEY_3: n = "3"; break;
	case GS_KEY_4: n = "4"; break;
	case GS_KEY_6: n = "6"; break;
	case GS_KEY_5: n = "5"; break;
	case GS_KEY_EQUAL: n = "EQUAL"; break;
	case GS_KEY_9: n = "9"; break;
	case GS_KEY_7: n = "7"; break;
	case GS_KEY_MINUS: n = "MINUS"; break;
	case GS_KEY_8: n = "8"; break;
	case GS_KEY_0: n = "0"; break;
	case GS_KEY_RIGHTBRACKET: n = "RIGHTBRACKET"; break;
	case GS_KEY_O: n = "O"; break;
	case GS_KEY_U: n = "U"; break;
	case GS_KEY_LEFTBRACKET: n = "LEFTBRACKET"; break;
	case GS_KEY_I: n = "I"; break;
	case GS_KEY_P: n = "P"; break;
	case GS_KEY_L: n = "L"; break;
	case GS_KEY_J: n = "J"; break;
	case GS_KEY_QUOTE: n = "QUOTE"; break;
	case GS_KEY_K: n = "K"; break;
	case GS_KEY_SEMICOLON: n = "SEMICOLON"; break;
	case GS_KEY_BACKSLASH: n = "BACKSLASH"; break;
	case GS_KEY_COMMA: n = "COMMA"; break;
	case GS_KEY_SLASH: n = "SLASH"; break;
	case GS_KEY_N: n = "N"; break;
	case GS_KEY_M: n = "M"; break;
	case GS_KEY_PERIOD: n = "PERIOD"; break;
	case GS_KEY_GRAVE: n = "GRAVE"; break;
	case GS_KEY_KEYPADDECIMAL: n = "KEYPADDECIMAL"; break;
	case GS_KEY_KEYPADMULTIPLY: n = "KEYPADMULTIPLY"; break;
	case GS_KEY_KEYPADPLUS: n = "KEYPADPLUS"; break;
	case GS_KEY_KEYPADCLEAR: n = "KEYPADCLEAR"; break;
	case GS_KEY_KEYPADDIVIDE: n = "KEYPADDIVIDE"; break;
	case GS_KEY_KEYPADENTER: n = "KEYPADENTER"; break;
	case GS_KEY_KEYPADMINUS: n = "KEYPADMINUS"; break;
	case GS_KEY_KEYPADEQUALS: n = "KEYPADEQUALS"; break;
	case GS_KEY_KEYPAD0: n = "KEYPAD0"; break;
	case GS_KEY_KEYPAD1: n = "KEYPAD1"; break;
	case GS_KEY_KEYPAD2: n = "KEYPAD2"; break;
	case GS_KEY_KEYPAD3: n = "KEYPAD3"; break;
	case GS_KEY_KEYPAD4: n = "KEYPAD4"; break;
	case GS_KEY_KEYPAD5: n = "KEYPAD5"; break;
	case GS_KEY_KEYPAD6: n = "KEYPAD6"; break;
	case GS_KEY_KEYPAD7: n = "KEYPAD7"; break;
	case GS_KEY_KEYPAD8: n = "KEYPAD8"; break;
	case GS_KEY_KEYPAD9: n = "KEYPAD9"; break;
	case GS_KEY_RETURN: n = "RETURN"; break;
	case GS_KEY_TAB: n = "TAB"; break;
	case GS_KEY_SPACE: n = "SPACE"; break;
	case GS_KEY_DELETE: n = "DELETE"; break;
	case GS_KEY_ESCAPE: n = "ESCAPE"; break;
	case GS_KEY_COMMAND: n = "COMMAND"; break;
	case GS_KEY_SHIFT: n = "SHIFT"; break;
	case GS_KEY_CAPSLOCK: n = "CAPSLOCK"; break;
	case GS_KEY_OPTION: n = "OPTION"; break;
	case GS_KEY_CONTROL: n = "CONTROL"; break;
	case GS_KEY_RIGHTSHIFT: n = "RIGHTSHIFT"; break;
	case GS_KEY_RIGHTOPTION: n = "RIGHTOPTION"; break;
	case GS_KEY_RIGHTCONTROL: n = "RIGHTCONTROL"; break;
	case GS_KEY_FUNCTION: n = "FUNCTION"; break;
	case GS_KEY_F17: n = "F17"; break;
	case GS_KEY_VOLUMEUP: n = "VOLUMEUP"; break;
	case GS_KEY_VOLUMEDOWN: n = "VOLUMEDOWN"; break;
	case GS_KEY_MUTE: n = "MUTE"; break;
	case GS_KEY_F18: n = "F18"; break;
	case GS_KEY_F19: n = "F19"; break;
	case GS_KEY_F20: n = "F20"; break;
	case GS_KEY_F5: n = "F5"; break;
	case GS_KEY_F6: n = "F6"; break;
	case GS_KEY_F7: n = "F7"; break;
	case GS_KEY_F3: n = "F3"; break;
	case GS_KEY_F8: n = "F8"; break;
	case GS_KEY_F9: n = "F9"; break;
	case GS_KEY_F11: n = "F11"; break;
	case GS_KEY_F13: n = "F13"; break;
	case GS_KEY_F16: n = "F16"; break;
	case GS_KEY_F14: n = "F14"; break;
	case GS_KEY_F10: n = "F10"; break;
	case GS_KEY_F12: n = "F12"; break;
	case GS_KEY_F15: n = "F15"; break;
	case GS_KEY_HELP: n = "HELP"; break;
	case GS_KEY_HOME: n = "HOME"; break;
	case GS_KEY_PAGEUP: n = "PAGEUP"; break;
	case GS_KEY_FORWARDDELETE: n = "FORWARDDELETE"; break;
	case GS_KEY_F4: n = "F4"; break;
	case GS_KEY_END: n = "END"; break;
	case GS_KEY_F2: n = "F2"; break;
	case GS_KEY_PAGEDOWN: n = "PAGEDOWN"; break;
	case GS_KEY_F1: n = "F1"; break;
	case GS_KEY_LEFTARROW: n = "LEFTARROW"; break;
	case GS_KEY_RIGHTARROW: n = "RIGHTARROW"; break;
	case GS_KEY_DOWNARROW: n = "DOWNARROW"; break;
	case GS_KEY_UPARROW: n = "UPARROW"; break;
        case GS_KEY_SCROLL: n = "SCROLL"; break;
        case GS_KEY_NUMLOCK: n = "NUMLOCK"; break;
        case GS_KEY_CLEAR: n = "CLEAR"; break;
        case GS_KEY_SYSREQ: n = "SYSREQ"; break;
        case GS_KEY_PAUSE: n = "PAUSE"; break;
        case GS_KEY_CAMERA: n = "CAMERA"; break;
        case GS_KEY_CENTER: n = "CENTER"; break;
        case GS_KEY_AT: n = "AT"; break;
        case GS_KEY_SYM: n = "SYM"; break;
	default: n = 0; break;
        }
        if (n)
                snprintf(buf, sizeof(buf), "GS_KEY_%s", n);
        else if (k >= 32 && k < 127)
                snprintf(buf, sizeof(buf), "%c", (unsigned)k);                
        else
                snprintf(buf, sizeof(buf), "0x%x", (unsigned)k);
	return buf;
}

static void got(int type, intptr_t p1, intptr_t p2) {
	ev e;
        e.type = type;
        e.p[0] = p1;
        e.p[1] = p2;
        switch (evType(&e)) {   
	case GS_EVENT_RESIZE:
		s_w = evWidth(&e);
		s_h = evHeight(&e);
		break;
	case GS_EVENT_MOTION:
		s_mx = evX(&e);
		s_my = evY(&e);
		break;
	case GS_EVENT_DOWN:
		press(evWhich(&e));
		break;
	case GS_EVENT_UP:
		release(evWhich(&e));
	};
	event(&e);
}
