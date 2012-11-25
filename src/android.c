#define CV_NO_MAIN
#include "cv.c"
#include <stdlib.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>

#define dbg cvReport
#define err cvReport

static void report(const char * name, const char * s) {
        __android_log_print(ANDROID_LOG_ERROR, name, s);
}

intptr_t osEvent(ev * e) {
        intptr_t ret;
        switch (evType(e)) {
        case CVQ_LOGGER: ret = (intptr_t)report; break;
        default: ret = 0;
        }
        return ret;
}
static struct android_app* g_app;

static cvkey mapkey(int kc) {
        cvkey ret;
        switch (kc) {
//      case AKEYCODE_SOFT_LEFT: ret = ; break;
//      case AKEYCODE_SOFT_RIGHT: ret = ; break;
        case AKEYCODE_HOME: ret = CVK_HOME; break;
//      case AKEYCODE_BACK: ret = ; break;
//      case AKEYCODE_CALL: ret = ; break;
//      case AKEYCODE_ENDCALL: ret = ; break;
        case AKEYCODE_0: ret = CVK_0; break;
        case AKEYCODE_1: ret = CVK_1; break;
        case AKEYCODE_2: ret = CVK_2; break;
        case AKEYCODE_3: ret = CVK_3; break;
        case AKEYCODE_4: ret = CVK_4; break;
        case AKEYCODE_5: ret = CVK_5; break;
        case AKEYCODE_6: ret = CVK_6; break;
        case AKEYCODE_7: ret = CVK_7; break;
        case AKEYCODE_8: ret = CVK_8; break;
        case AKEYCODE_9: ret = CVK_9; break;
        case AKEYCODE_STAR: ret = CVK_KEYPADMULTIPLY; break;
//      case AKEYCODE_POUND: ret = CVK_; break;
        case AKEYCODE_DPAD_UP: ret = CVK_UPARROW; break;
        case AKEYCODE_DPAD_DOWN: ret = CVK_DOWNARROW; break;
        case AKEYCODE_DPAD_LEFT: ret = CVK_LEFTARROW; break;
        case AKEYCODE_DPAD_RIGHT: ret = CVK_RIGHTARROW; break;
        case AKEYCODE_DPAD_CENTER: ret = CVK_CENTER; break;
        case AKEYCODE_VOLUME_UP: ret = CVK_VOLUMEUP; break;
        case AKEYCODE_VOLUME_DOWN: ret = CVK_VOLUMEDOWN; break;
//      case AKEYCODE_POWER: ret = CVK_; break;
        case AKEYCODE_CAMERA: ret = CVK_CAMERA; break;
        case AKEYCODE_CLEAR: ret = CVK_KEYPADCLEAR; break;
        case AKEYCODE_A: ret = CVK_A; break;
        case AKEYCODE_B: ret = CVK_B; break;
        case AKEYCODE_C: ret = CVK_C; break;
        case AKEYCODE_D: ret = CVK_D; break;
        case AKEYCODE_E: ret = CVK_E; break;
        case AKEYCODE_F: ret = CVK_F; break;
        case AKEYCODE_G: ret = CVK_G; break;
        case AKEYCODE_H: ret = CVK_H; break;
        case AKEYCODE_I: ret = CVK_I; break;
        case AKEYCODE_J: ret = CVK_J; break;
        case AKEYCODE_K: ret = CVK_K; break;
        case AKEYCODE_L: ret = CVK_L; break;
        case AKEYCODE_M: ret = CVK_M; break;
        case AKEYCODE_N: ret = CVK_N; break;
        case AKEYCODE_O: ret = CVK_O; break;
        case AKEYCODE_P: ret = CVK_P; break;
        case AKEYCODE_Q: ret = CVK_Q; break;
        case AKEYCODE_R: ret = CVK_R; break;
        case AKEYCODE_S: ret = CVK_S; break;
        case AKEYCODE_T: ret = CVK_T; break;
        case AKEYCODE_U: ret = CVK_U; break;
        case AKEYCODE_V: ret = CVK_V; break;
        case AKEYCODE_W: ret = CVK_W; break;
        case AKEYCODE_X: ret = CVK_X; break;
        case AKEYCODE_Y: ret = CVK_Y; break;
        case AKEYCODE_Z: ret = CVK_Z; break;
        case AKEYCODE_COMMA: ret = CVK_COMMA; break;
        case AKEYCODE_PERIOD: ret = CVK_PERIOD; break;
        case AKEYCODE_ALT_LEFT: ret = CVK_OPTION; break;
        case AKEYCODE_ALT_RIGHT: ret = CVK_RIGHTOPTION; break;
        case AKEYCODE_SHIFT_LEFT: ret = CVK_SHIFT; break;
        case AKEYCODE_SHIFT_RIGHT: ret = CVK_RIGHTSHIFT; break;
        case AKEYCODE_TAB: ret = CVK_TAB; break;
        case AKEYCODE_SPACE: ret = CVK_SPACE; break;
        case AKEYCODE_SYM: ret = CVK_SYM; break;
//      case AKEYCODE_EXPLORER: ret = CVK_; break;
//      case AKEYCODE_ENVELOPE: ret = CVK_; break;
        case AKEYCODE_ENTER: ret = CVK_RETURN; break;
        case AKEYCODE_DEL: ret = CVK_DELETE; break;
        case AKEYCODE_GRAVE: ret = CVK_GRAVE; break;
        case AKEYCODE_MINUS: ret = CVK_MINUS; break;
        case AKEYCODE_EQUALS: ret = CVK_EQUAL; break;
        case AKEYCODE_LEFT_BRACKET: ret = CVK_LEFTBRACKET; break;
        case AKEYCODE_RIGHT_BRACKET: ret = CVK_RIGHTBRACKET; break;
        case AKEYCODE_BACKSLASH: ret = CVK_BACKSLASH; break;
        case AKEYCODE_SEMICOLON: ret = CVK_SEMICOLON; break;
        case AKEYCODE_APOSTROPHE: ret = CVK_QUOTE; break;
        case AKEYCODE_SLASH: ret = CVK_SLASH; break;
//      case AKEYCODE_AT: ret = CVK_; break;
//      case AKEYCODE_NUM: ret = CVK_; break;
//      case AKEYCODE_HEADSETHOOK: ret = CVK_; break;
//      case AKEYCODE_FOCUS: ret = CVK_; break;
        case AKEYCODE_PLUS: ret = CVK_KEYPADPLUS; break;
//      case AKEYCODE_MENU: ret = CVK_; break;
//      case AKEYCODE_NOTIFICATION: ret = CVK_; break;
//      case AKEYCODE_SEARCH: ret = CVK_; break;
//      case AKEYCODE_MEDIA_PLAY_PAUSE: ret = CVK_; break;
//      case AKEYCODE_MEDIA_STOP: ret = CVK_; break;
//      case AKEYCODE_MEDIA_NEXT: ret = CVK_; break;
//      case AKEYCODE_MEDIA_PREVIOUS: ret = CVK_; break;
//      case AKEYCODE_MEDIA_REWIND: ret = CVK_; break;
//      case AKEYCODE_MEDIA_FAST_FORWARD: ret = CVK_; break;
        case AKEYCODE_MUTE: ret = CVK_MUTE; break;
        case AKEYCODE_PAGE_UP: ret = CVK_PAGEUP; break;
        case AKEYCODE_PAGE_DOWN: ret = CVK_PAGEDOWN; break;
//      case AKEYCODE_PICTSYMBOLS: ret = CVK_; break;
//      case AKEYCODE_SWITCH_CHARSET: ret = CVK_; break;
//      case AKEYCODE_BUTTON_A: ret = CVK_; break;
//      case AKEYCODE_BUTTON_B: ret = CVK_; break;
//      case AKEYCODE_BUTTON_C: ret = CVK_; break;
//      case AKEYCODE_BUTTON_X: ret = CVK_; break;
//      case AKEYCODE_BUTTON_Y: ret = CVK_; break;
//      case AKEYCODE_BUTTON_Z: ret = CVK_; break;
//      case AKEYCODE_BUTTON_L1: ret = CVK_; break;
//      case AKEYCODE_BUTTON_R1: ret = CVK_; break;
//      case AKEYCODE_BUTTON_L2: ret = CVK_; break;
//      case AKEYCODE_BUTTON_R2: ret = CVK_; break;
//      case AKEYCODE_BUTTON_THUMBL: ret = CVK_; break;
//      case AKEYCODE_BUTTON_THUMBR: ret = CVK_; break;
//      case AKEYCODE_BUTTON_START: ret = CVK_; break;
//      case AKEYCODE_BUTTON_SELECT: ret = CVK_; break;
//      case AKEYCODE_BUTTON_MODE: ret = CVK_; break;
        default:
                ret = CVK_NONE;
        }
        return ret;
}

static int32_t input(struct android_app * app, AInputEvent * e) {
        int handled = 1;
        int kc;
        cvkey cvkc;
        int action;
        switch (AInputEvent_getType(e)) {
        case AINPUT_EVENT_TYPE_KEY:
                kc = AKeyEvent_getKeyCode(e);
                action = AKeyEvent_getAction(e);
                cvkc = mapkey(kc);
                handled = kc != CVK_NONE;
                if (handled) {
                        if (action == AKEY_EVENT_ACTION_DOWN)
                                cvInject(CVE_DOWN, cvkc, 0);
                        if (action == AKEY_EVENT_ACTION_UP)
                                cvInject(CVE_UP, cvkc, 0);
                }
                break;
        case AINPUT_EVENT_TYPE_MOTION:
                cvInject(CVE_MOTION, 
                         AMotionEvent_getX(e, 0), AMotionEvent_getY(e, 0));
                break;
        default:
                handled = 0;
        }
        return handled;
}

static void handle(struct android_app* app, int32_t cmd) {
        switch (cmd) {
        case APP_CMD_INIT_WINDOW:
                app->userData = (void*)1;
                break;
        case APP_CMD_TERM_WINDOW:
                dbg("got term window");
                cvInject(CVE_CLOSE, 0, 0);
                app->userData = (void*)1;
                break;
        }
}

static void pollEvent(struct android_app * app) {
        int ident;
        int events;
        struct android_poll_source* source;
        ident=ALooper_pollAll(0, 0, &events, (void**)&source);
        if (ident >= 0 && source)
                source->process(app, source);
}

__attribute__((visibility("default"))) __attribute__((used))
void android_main(struct android_app* app) {
        char * argv[] = {"cvandroid"};
        const EGLint attr[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NONE
        };
        EGLint ncfg;
        EGLint fmt;
        EGLint w;
        EGLint h;
        EGLConfig cfg;
        EGLDisplay dpy;
        EGLSurface surf;
        EGLContext ctx;
        app_dummy();
        app->userData = 0;
        app->onAppCmd = handle;
        app->onInputEvent = input;
        dbg("wait\n");
        while (!app->userData)
                pollEvent(app);
        g_app = app;
        cvInject(CVE_INIT, 1, (intptr_t)argv);
        dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (!eglInitialize(dpy, 0, 0))
                err("initialize display");
        eglChooseConfig(dpy, attr, &cfg, 1, &ncfg);
        eglGetConfigAttrib(dpy, cfg, EGL_NATIVE_VISUAL_ID, &fmt);
        ANativeWindow_setBuffersGeometry(app->window, 0, 0, fmt);
        surf = eglCreateWindowSurface(dpy, cfg, app->window, 0);        
        ctx = eglCreateContext(dpy, cfg, 0, 0);
        eglMakeCurrent(dpy, surf, surf, ctx);
        eglQuerySurface(dpy, surf, EGL_WIDTH, &w);
        eglQuerySurface(dpy, surf, EGL_HEIGHT, &h);
        if (!eglSwapInterval(dpy, 1))
                err("setting swap interval");
        cvInject(CVE_RESIZE, w, h);
        cvInject(CVE_GLINIT, 0, 0);
        app->userData = 0;
        while (!app->userData && !app->destroyRequested) {
                cvInject(CVE_UPDATE, 0, 0);
                if (!eglSwapBuffers(dpy, surf))
                        err("swapbuffers failed");
                pollEvent(app);
        }
        if (app->destroyRequested) 
                cvInject(CVE_CLOSE, 0, 0);
        dbg("terminated");
        cvInject(CVE_GLTERM, 0, 0);
        if (!eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT))
                err("clearing current");
        if (!eglDestroyContext(dpy, ctx))
                err("destroying context");
        if (!eglDestroySurface(dpy, surf))
                err("destroying surface");
        if (!eglTerminate(dpy))
                err("terminating");
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
