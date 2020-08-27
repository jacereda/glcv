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

#include <stdarg.h>
#if defined _MSC_VER
#include <assert.h>
#else
#include <inttypes.h>
#endif

enum _cveventtype {
        CVE_NONE,
#define Q(x) CVQ_##x,
#include "cvqueries.h"
#undef Q
#define C(x) CVE_##x,
#include "cvevents.h"
#undef C
        CVE_MAX,
};
typedef enum _cveventtype cveventtype;

enum _cvkey {
#define K(x) CVK_##x,
#include "cvkeys.h"
#undef K
};
typedef enum _cvkey cvkey;

// Don't access this struct directly, use ev*() functions
typedef struct _ev {
        uintptr_t type;
        uintptr_t p[2];
} ev;

// Provided by the program
#ifdef CV_EXPLICIT_ENTRY
int cvRun(intptr_t (*handler)(const ev *));
#else
intptr_t event(const ev * e);
#endif

unsigned cvWidth();
unsigned cvHeight();
int cvMouseX();
int cvMouseY();
int cvPressed(cvkey key);
int cvReleased(cvkey key);

void cvSetCursor(const uint8_t * rgba, int hotx, int hoty);
void cvHideCursor();
void cvDefaultCursor();
void cvShowKeyboard();
void cvHideKeyboard();
void cvFullscreen();
void cvWindowed();

void cvReportV(const char * fmt, va_list ap);
void cvReport(const char * fmt, ...);

void cvQuit();

int evType(const ev *);
const char * evName(const ev *);
int evWidth(const ev *);
int evHeight(const ev *);
cvkey evWhich(const ev *);
uint32_t evUnicode(const ev *);
intptr_t evArg0(const ev *);
intptr_t evArg1(const ev *);
int evX(const ev *);
int evY(const ev *);
int evArgC(const ev *);
char ** evArgV(const ev *);
const char * evMethod(const ev *);

intptr_t cvInject(cveventtype, intptr_t, intptr_t);

const char * keyName(cvkey k);
