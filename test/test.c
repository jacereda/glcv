#include "gs.h"
#include "gsgl.h"

static void init() {
	gsReport("init");
}

static void term() {
	gsReport("term");
}

static void glinit() {
	gsReport("glinit");
}

static void down(gskey k) {
	gsReport("down %s", keyName(k));
}

static void up(gskey k) {
	gsReport("up %s", keyName(k));
	switch (k) {
	case GSK_ESCAPE: gsQuit(); break;
	default: break;
	}
}

static void unicode(uint32_t c) {
	gsReport("unicode %c", c);
}

static void motion(int x, int y) {
	gsReport("motion %d %d", x, y);
}

static void close() {
	gsReport("close");
	gsQuit();
}

static void invoke(const char * s) {
	gsReport("invoked method '%s'", s);
}

static void resize(unsigned w, unsigned h) {
	gsReport("resize %d %d", w, h);
	glViewport(0, 0, w, h);
}

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
	if (t != GSC_UPDATE)
		gsReport("got event %s, %p %p", 
			 evName(e), evArg0(e), evArg1(e));
	switch (t) {
	case GSQ_NAME: ret = (intptr_t)"test"; break;
	case GSQ_XPOS: ret = 50; break;
	case GSQ_YPOS: ret = 50; break;
	case GSQ_WIDTH: ret = 640; break;
	case GSQ_HEIGHT: ret = 480; break;
	case GSQ_BORDERS: ret = 1; break;
	case GSC_INIT: init(); break;
	case GSC_TERM: term(); break;
	case GSC_GLINIT: glinit(); break;
	case GSC_DOWN: down(evWhich(e)); break;
	case GSC_UP: up(evWhich(e)); break;
	case GSC_UNICODE: unicode(evUnicode(e)); break;
	case GSC_MOTION: motion(evX(e), evY(e)); break;
	case GSC_CLOSE: close(); break;
	case GSC_INVOKE: invoke(evMethod(e)); break;
	case GSC_RESIZE: resize(gsWidth(e), gsHeight(e)); break;
	case GSC_UPDATE: update(); break;
	default: ret = 0; break;
	}
	return ret;
}
