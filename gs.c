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

struct _ev {
	uintptr_t type;
	uintptr_t p[2];
};

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

static void mysnprintf(char * buf, size_t sz, const char * fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sz, fmt, ap);
	va_end(ap);
	buf[sz-1] = 0;
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

const char * evName(const ev * e) {
	const char * n;
	static char buf[32];
	switch (evType(e)) {
	default: // Falls to GS_EVENT_NONE
#define E(x) case GS_EVENT_##x: n = #x; break;
#include "gsev.h"
#undef E
	}
	mysnprintf(buf, sizeof(buf), "GS_EVENT_%s", n);
	return buf;
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

int evArgC(const ev * e) {
	return e->p[0];
}

char ** evArgV(const ev * e) {
	return (char**) e->p[1];
}

const char * evKeyName(const ev * e) {
	const char * n = 0;
	static char buf[32];
        gskey k = evWhich(e);
	switch (k) {
#define K(x) case GS_KEY_##x: n = #x; break;
#include "gskey.h"
#undef K
	default: n = 0; break;
        }
        if (n)
                mysnprintf(buf, sizeof(buf), "GS_KEY_%s", n);
        else if (k >= 32 && k < 127)
                mysnprintf(buf, sizeof(buf), "%c", (unsigned)k);                
        else
                mysnprintf(buf, sizeof(buf), "0x%x", (unsigned)k);
	return buf;
}

intptr_t gsInject(gseventtype type, intptr_t p1, intptr_t p2) {
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
		break;
	};
	return event(&e);
}

void gsReport(const char * fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	gsReportV(fmt, ap);
	va_end(ap);
}
