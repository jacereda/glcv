#include "gs.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <EGL/egl.h>

#define dbg gsReport
#define err gsReport

void gsReportV(const char * fmt, va_list ap) {
	__android_log_vprint(ANDROID_LOG_ERROR, "gs", fmt, ap);
}

static struct android_app* g_app;

static gskey mapkey(int kc) {
	gskey ret;
	switch (kc) {
//	case AKEYCODE_SOFT_LEFT: ret = ; break;
//	case AKEYCODE_SOFT_RIGHT: ret = ; break;
	case AKEYCODE_HOME: ret = GSK_HOME; break;
//	case AKEYCODE_BACK: ret = ; break;
//	case AKEYCODE_CALL: ret = ; break;
//	case AKEYCODE_ENDCALL: ret = ; break;
	case AKEYCODE_0: ret = GSK_0; break;
	case AKEYCODE_1: ret = GSK_1; break;
	case AKEYCODE_2: ret = GSK_2; break;
	case AKEYCODE_3: ret = GSK_3; break;
	case AKEYCODE_4: ret = GSK_4; break;
	case AKEYCODE_5: ret = GSK_5; break;
	case AKEYCODE_6: ret = GSK_6; break;
	case AKEYCODE_7: ret = GSK_7; break;
	case AKEYCODE_8: ret = GSK_8; break;
	case AKEYCODE_9: ret = GSK_9; break;
	case AKEYCODE_STAR: ret = GSK_KEYPADMULTIPLY; break;
//	case AKEYCODE_POUND: ret = GSK_; break;
	case AKEYCODE_DPAD_UP: ret = GSK_UPARROW; break;
	case AKEYCODE_DPAD_DOWN: ret = GSK_DOWNARROW; break;
	case AKEYCODE_DPAD_LEFT: ret = GSK_LEFTARROW; break;
	case AKEYCODE_DPAD_RIGHT: ret = GSK_RIGHTARROW; break;
	case AKEYCODE_DPAD_CENTER: ret = GSK_CENTER; break;
	case AKEYCODE_VOLUME_UP: ret = GSK_VOLUMEUP; break;
	case AKEYCODE_VOLUME_DOWN: ret = GSK_VOLUMEDOWN; break;
//	case AKEYCODE_POWER: ret = GSK_; break;
	case AKEYCODE_CAMERA: ret = GSK_CAMERA; break;
	case AKEYCODE_CLEAR: ret = GSK_KEYPADCLEAR; break;
	case AKEYCODE_A: ret = GSK_A; break;
	case AKEYCODE_B: ret = GSK_B; break;
	case AKEYCODE_C: ret = GSK_C; break;
	case AKEYCODE_D: ret = GSK_D; break;
	case AKEYCODE_E: ret = GSK_E; break;
	case AKEYCODE_F: ret = GSK_F; break;
	case AKEYCODE_G: ret = GSK_G; break;
	case AKEYCODE_H: ret = GSK_H; break;
	case AKEYCODE_I: ret = GSK_I; break;
	case AKEYCODE_J: ret = GSK_J; break;
	case AKEYCODE_K: ret = GSK_K; break;
	case AKEYCODE_L: ret = GSK_L; break;
	case AKEYCODE_M: ret = GSK_M; break;
	case AKEYCODE_N: ret = GSK_N; break;
	case AKEYCODE_O: ret = GSK_O; break;
	case AKEYCODE_P: ret = GSK_P; break;
	case AKEYCODE_Q: ret = GSK_Q; break;
	case AKEYCODE_R: ret = GSK_R; break;
	case AKEYCODE_S: ret = GSK_S; break;
	case AKEYCODE_T: ret = GSK_T; break;
	case AKEYCODE_U: ret = GSK_U; break;
	case AKEYCODE_V: ret = GSK_V; break;
	case AKEYCODE_W: ret = GSK_W; break;
	case AKEYCODE_X: ret = GSK_X; break;
	case AKEYCODE_Y: ret = GSK_Y; break;
	case AKEYCODE_Z: ret = GSK_Z; break;
	case AKEYCODE_COMMA: ret = GSK_COMMA; break;
	case AKEYCODE_PERIOD: ret = GSK_PERIOD; break;
	case AKEYCODE_ALT_LEFT: ret = GSK_OPTION; break;
	case AKEYCODE_ALT_RIGHT: ret = GSK_RIGHTOPTION; break;
	case AKEYCODE_SHIFT_LEFT: ret = GSK_SHIFT; break;
	case AKEYCODE_SHIFT_RIGHT: ret = GSK_RIGHTSHIFT; break;
	case AKEYCODE_TAB: ret = GSK_TAB; break;
	case AKEYCODE_SPACE: ret = GSK_SPACE; break;
	case AKEYCODE_SYM: ret = GSK_SYM; break;
//	case AKEYCODE_EXPLORER: ret = GSK_; break;
//	case AKEYCODE_ENVELOPE: ret = GSK_; break;
	case AKEYCODE_ENTER: ret = GSK_RETURN; break;
	case AKEYCODE_DEL: ret = GSK_DELETE; break;
	case AKEYCODE_GRAVE: ret = GSK_GRAVE; break;
	case AKEYCODE_MINUS: ret = GSK_MINUS; break;
	case AKEYCODE_EQUALS: ret = GSK_EQUAL; break;
	case AKEYCODE_LEFT_BRACKET: ret = GSK_LEFTBRACKET; break;
	case AKEYCODE_RIGHT_BRACKET: ret = GSK_RIGHTBRACKET; break;
	case AKEYCODE_BACKSLASH: ret = GSK_BACKSLASH; break;
	case AKEYCODE_SEMICOLON: ret = GSK_SEMICOLON; break;
	case AKEYCODE_APOSTROPHE: ret = GSK_QUOTE; break;
	case AKEYCODE_SLASH: ret = GSK_SLASH; break;
//	case AKEYCODE_AT: ret = GSK_; break;
//	case AKEYCODE_NUM: ret = GSK_; break;
//	case AKEYCODE_HEADSETHOOK: ret = GSK_; break;
//	case AKEYCODE_FOCUS: ret = GSK_; break;
	case AKEYCODE_PLUS: ret = GSK_KEYPADPLUS; break;
//	case AKEYCODE_MENU: ret = GSK_; break;
//	case AKEYCODE_NOTIFICATION: ret = GSK_; break;
//	case AKEYCODE_SEARCH: ret = GSK_; break;
//	case AKEYCODE_MEDIA_PLAY_PAUSE: ret = GSK_; break;
//	case AKEYCODE_MEDIA_STOP: ret = GSK_; break;
//	case AKEYCODE_MEDIA_NEXT: ret = GSK_; break;
//	case AKEYCODE_MEDIA_PREVIOUS: ret = GSK_; break;
//	case AKEYCODE_MEDIA_REWIND: ret = GSK_; break;
//	case AKEYCODE_MEDIA_FAST_FORWARD: ret = GSK_; break;
	case AKEYCODE_MUTE: ret = GSK_MUTE; break;
	case AKEYCODE_PAGE_UP: ret = GSK_PAGEUP; break;
	case AKEYCODE_PAGE_DOWN: ret = GSK_PAGEDOWN; break;
//	case AKEYCODE_PICTSYMBOLS: ret = GSK_; break;
//	case AKEYCODE_SWITCH_CHARSET: ret = GSK_; break;
//	case AKEYCODE_BUTTON_A: ret = GSK_; break;
//	case AKEYCODE_BUTTON_B: ret = GSK_; break;
//	case AKEYCODE_BUTTON_C: ret = GSK_; break;
//	case AKEYCODE_BUTTON_X: ret = GSK_; break;
//	case AKEYCODE_BUTTON_Y: ret = GSK_; break;
//	case AKEYCODE_BUTTON_Z: ret = GSK_; break;
//	case AKEYCODE_BUTTON_L1: ret = GSK_; break;
//	case AKEYCODE_BUTTON_R1: ret = GSK_; break;
//	case AKEYCODE_BUTTON_L2: ret = GSK_; break;
//	case AKEYCODE_BUTTON_R2: ret = GSK_; break;
//	case AKEYCODE_BUTTON_THUMBL: ret = GSK_; break;
//	case AKEYCODE_BUTTON_THUMBR: ret = GSK_; break;
//	case AKEYCODE_BUTTON_START: ret = GSK_; break;
//	case AKEYCODE_BUTTON_SELECT: ret = GSK_; break;
//	case AKEYCODE_BUTTON_MODE: ret = GSK_; break;
	default:
		ret = GSK_NONE;
	}
	return ret;
}

static int32_t input(struct android_app * app, AInputEvent * e) {
	int handled = 1;
	int kc;
	gskey gskc;
	int action;
	switch (AInputEvent_getType(e)) {
		case AINPUT_EVENT_TYPE_KEY:
			kc = AKeyEvent_getKeyCode(e);
			action = AKeyEvent_getAction(e);
			gskc = mapkey(kc);
			handled = kc != GSK_NONE;
			if (handled) {
				if (action == AKEY_EVENT_ACTION_DOWN)
					gsInject(GSC_DOWN, gskc, 0);
				if (action == AKEY_EVENT_ACTION_UP)
					gsInject(GSC_UP, gskc, 0);
			}
			break;
		case AINPUT_EVENT_TYPE_MOTION:
			gsInject(GSC_MOTION, 
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
		gsInject(GSC_CLOSE, 0, 0);
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
	char * argv[] = {"gsandroid"};
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
	gsInject(GSC_INIT, 1, (intptr_t)argv);
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
	gsInject(GSC_RESIZE, w, h);
	gsInject(GSC_GLINIT, 0, 0);
	app->userData = 0;
	while (!app->userData && !app->destroyRequested) {
		gsInject(GSC_UPDATE, 0, 0);
		if (!eglSwapBuffers(dpy, surf))
			err("swapbuffers failed");
		pollEvent(app);
	}
	if (app->destroyRequested) 
		gsInject(GSC_CLOSE, 0, 0);
	dbg("terminated");
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
   End: **
*/
