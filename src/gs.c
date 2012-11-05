/*
  Copyright (c) 2011-2012, Jorge Acereda Maciá
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

extern int gsrun(int, char**);

#if defined GS_EXPLICIT_ENTRY
intptr_t (*event)(const ev * e);
int gsRun(intptr_t (*handler)(const ev *)) {
        event = handler;
        return gsrun(0, 0);
}
#endif
#if !defined GS_NO_MAIN
int main(int argc, char ** argv) {
        return gsrun(argc, argv);
}
#endif


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
        int good = *k >= GSK_NONE && *k < GSK_MAX;
        assert(good);
        *k -= GSK_NONE;
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
        default: // Falls to GSE_NONE
#define Q(x) case GSQ_##x: n = #x; break;
#include "gsqueries.h"
#undef Q
#define C(x) case GSE_##x: n = #x; break;
#include "gsevents.h"
#undef C
        }
        mysnprintf(buf, sizeof(buf), "GSE_%s", n);
        return buf;
}

intptr_t evArg0(const ev * e) {
        return e->p[0];
}

intptr_t evArg1(const ev * e) {
        return e->p[1];
}

int evWidth(const ev * e) {
        return evArg0(e);
}

int evHeight(const ev * e) {
        return evArg1(e);
}

gskey evWhich(const ev * e) {
        return evArg0(e);
}

uint32_t evUnicode(const ev * e) {
        return evArg0(e);
}

int evX(const ev * e) {
        return evArg0(e);
}

int evY(const ev * e) {
        return evArg1(e);
}

int evArgC(const ev * e) {
        return evArg0(e);
}

char ** evArgV(const ev * e) {
        return (char**) evArg1(e);
}

const char * evMethod(const ev * e) {
        return (char*)evArg0(e);
}

const char * keyName(gskey k) {
        const char * n = 0;
        static char buf[32];
        switch (k) {
#define K(x) case GSK_##x: n = #x; break;
#include "gskeys.h"
#undef K
        default: n = 0; break;
        }
        if (n)
                mysnprintf(buf, sizeof(buf), "%s", n);
        else if (k >= 32 && k < 127)
                mysnprintf(buf, sizeof(buf), "%c", (unsigned)k);                
        else
                mysnprintf(buf, sizeof(buf), "0x%x", (unsigned)k);
        return buf;
}

static void defaultlog(const char * name, const char * s) {
        char path[256]; 
        FILE *out;
        snprintf(path, sizeof(path), 
#if defined _WIN32
                 "%s.log"
#else
                 "/tmp/%s.log"
#endif
                 , name);
        out = fopen(path, "a");
        if(out) {
                fprintf(out, "%s", s);
                fclose(out);
        }
}

intptr_t gsInject(gseventtype type, intptr_t p1, intptr_t p2) {
        extern intptr_t osEvent(ev *);
        ev e;
        intptr_t ret;
        e.type = type;
        e.p[0] = p1;
        e.p[1] = p2;
        switch (evType(&e)) {   
        case GSE_RESIZE:
                s_w = evWidth(&e);
                s_h = evHeight(&e);
                break;
        case GSE_MOTION:
                s_mx = evX(&e);
                s_my = evY(&e);
                break;
        case GSE_DOWN:
                press(evWhich(&e));
                break;
        case GSE_UP:
                release(evWhich(&e));
                break;
        };
        ret = event(&e);
        if (!ret)
                ret = osEvent(&e);
        if (!ret) 
                switch (evType(&e)) {
                case GSQ_NAME:
                        ret = (intptr_t)"Unknown"; 
                        break;
                case GSQ_LOGGER:
                        ret = (intptr_t)defaultlog;
                        break;
                case GSQ_XPOS:
                case GSQ_YPOS:
                        ret = 0;
                        break;
                case GSQ_WIDTH:
                case GSQ_HEIGHT:
                        ret = -1;
                        break;
                case GSE_DOWN:
                        if (evWhich(&e) == GSK_ESCAPE)
                                gsQuit();
                        break;
                case GSE_CLOSE:
                        gsQuit();
                        break;
                }
        return ret;
}

void gsReport(const char * fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        gsReportV(fmt, ap);
        va_end(ap);
}


typedef void (*logger_t)(const char *, const char *);

void gsReportV(const char *fmt, va_list ap) {
        static logger_t logger = 0;
        static const char * name = 0;
        int good = logger != (logger_t)1;
        if (!logger && good) {
                logger = (logger_t)1;
                name = (const char *)gsInject(GSQ_NAME, 0, 0);
                logger = (logger_t)gsInject(GSQ_LOGGER, 0, 0);
        }
        if (good) {
                char b[1024];
                size_t s = 0;
                s += vsnprintf(b + s, sizeof(b) - s, fmt, ap);
                s += snprintf(b + s, sizeof(b) - s, "\n");
                logger(name, b);
        }
}

void gsQuit() {
        gsInject(GSE_QUIT, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
