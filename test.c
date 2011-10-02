#include "gs.h"
#include "gsgl.h"

static void draw() {
        static int i = 0;
        glClearColor((i++ & 0xff) / 255.0f, 0.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
}

intptr_t event(const ev * e) {
	intptr_t ret = 1;
	report("got event %s", evName(e));
	switch (evType(e)) {
	case GS_EVENT_DRAW: draw(); break;
	default: break;
	}
	return ret;
}
