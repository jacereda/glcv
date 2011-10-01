#include "gs.h"
#include "gsgl.h"

static int s_running = 1;
int init(int argc, char ** argv) {
	return 1;
}

void glinit() {
}

int term() {
	return 1;
}

int tick() {
	return s_running;
}

void draw() {
        static int i = 0;
        glClearColor((i++ & 0xff) / 255.0f, 0.f, 1.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
}

void event(const ev * e) {
	switch (evType(e)) {
	case GS_EVENT_CLOSE:
		s_running = 0;
		break;
	default: break;
	}
}
