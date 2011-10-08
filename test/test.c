#include "gs.h"
#include "gsgl.h"

static void draw() {
        static int i = 128;
	i++;
	glViewport(0, 0, gsWidth(), gsHeight());
        glClearColor(((2*i) & 0xff) / 255.0f, 0.f, 1.f, (i & 0xff) / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
}

static void invoke(const char * s) {
	gsReport("invoked method '%s'", s);
}

intptr_t event(const ev * e) {
	intptr_t ret = 1;
	gsReport("got event %s, %p %p", evName(e), evArg0(e), evArg1(e));
	switch (evType(e)) {
	case GSQ_NAME: ret = (intptr_t)"test"; break;
	case GSC_UPDATE: draw(); break;
	case GSC_INVOKE: invoke(evMethod(e)); break;
	default: ret = 0; break;
	}
	return ret;
}
