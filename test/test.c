#include "cv.h"
#include "cvgl.h"

static void init() {
        cvReport("init");
}

static void term() {
        cvReport("term");
}

static void glinit() {
        cvReport("glinit");
}

static void down(cvkey k) {
        cvReport("down %s", keyName(k));
        if (k == CVK_MOUSELEFT && cvMouseY() < 10) 
                cvShowKeyboard();
}

static void up(cvkey k) {
        cvReport("up %s", keyName(k));
        switch (k) {
        case CVK_ESCAPE: cvQuit(); break;
        case CVK_H: cvHideCursor(); break;
        case CVK_S: cvShowCursor(); break;
        case CVK_RETURN: cvHideKeyboard(); break;
        default: break;
        }
}

static void unicode(uint32_t c) {
        cvReport("unicode %c", c);
}

static void motion(int x, int y) {
        cvReport("motion %d %d", x, y);
}

static void close() {
        cvReport("close");
        cvQuit();
}

static void invoke(const char * s) {
        cvReport("invoked method '%s'", s);
}

static void resize(unsigned w, unsigned h) {
        cvReport("resize %d %d", w, h);
        glViewport(0, 0, w, h);
}

static void update() {
        static int i = 128;
        i++;
        glClearColor(((2*i) & 0xff) / 255.0f, 0.f, 1.f, (i & 0xff) / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFlush();
        if (cvPressed(CVK_A))
                cvReport("A pressed");
        if (cvReleased(CVK_A))
                cvReport("A released");
}

intptr_t event(const ev * e) {
        intptr_t ret = 1;
        cveventtype t = evType(e);
        if (t != CVE_UPDATE)
                cvReport("got event %s, %p %p", 
                         evName(e), evArg0(e), evArg1(e));
        switch (t) {
        case CVQ_NAME: ret = (intptr_t)"test"; break;
        case CVQ_XPOS: ret = 50; break;
        case CVQ_YPOS: ret = 50; break;
        case CVQ_WIDTH: ret = 640; break;
        case CVQ_HEIGHT: ret = 480; break;
        case CVQ_BORDERS: ret = 1; break;
        case CVE_INIT: init(); break;
        case CVE_TERM: term(); break;
        case CVE_GLINIT: glinit(); break;
        case CVE_DOWN: down(evWhich(e)); break;
        case CVE_UP: up(evWhich(e)); break;
        case CVE_UNICODE: unicode(evUnicode(e)); break;
        case CVE_MOTION: motion(evX(e), evY(e)); break;
        case CVE_CLOSE: close(); break;
        case CVE_INVOKE: invoke(evMethod(e)); break;
        case CVE_RESIZE: resize(evWidth(e), evHeight(e)); break;
        case CVE_UPDATE: update(); break;
        default: ret = 0; break;
        }
        return ret;
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
