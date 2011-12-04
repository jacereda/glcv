#include "gs.h"
#include "gsgl.h"

static void update() {
        static int i = 128;
	i++;
        glClearColor(((2*i) & 0xff) / 255.0f, 0.f, 1.f, (i & 0xff) / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
}

intptr_t event(const ev * e) {
	intptr_t ret = 1;
	gseventtype t = evType(e);
	switch (t) {
	case GSC_UPDATE: update(); break;
	default: ret = 0; break;
	}
	return ret;
}
