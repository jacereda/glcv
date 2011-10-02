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

#include <stdarg.h>
#if defined(_MSC_VER)
#include <assert.h>
#else
#include <inttypes.h>
#endif

typedef enum {
#define E(x) GSE_##x,
#include "gsev.h"
#undef E
} gseventtype;

typedef enum {
#define K(x) GSK_##x,
#include "gskey.h"
#undef K
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
const char * gsResPath();

intptr_t gsInject(gseventtype, intptr_t, intptr_t);
void gsReportV(const char * fmt, va_list ap);
void gsReport(const char * fmt, ...);

int evType(const ev *);
const char * evName(const ev *);
int evWidth(const ev *);
int evHeight(const ev *);
gskey evWhich(const ev *);
intptr_t evArg0(const ev *);
intptr_t evArg1(const ev *);
int evX(const ev *);
int evY(const ev *);
int evArgC(const ev *);
char ** evArgV(const ev *);
const char * evKeyName(const ev *);

// Provided by the program
intptr_t event(const ev * e);

#if defined(__GNUC__) && !defined(_WIN32_)
#define EXPORTED \
	extern __attribute__((visibility("default"))) __attribute__((used))
#else
#define EXPORTED extern __declspec(dllexport)
#endif
