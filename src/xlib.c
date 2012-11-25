#include "cv.c"
#include <GL/glx.h>
#include <X11/keysym.h>

static int g_done = 0;

static Display * g_dpy;
static Window g_win;

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case CVE_QUIT: g_done = 1; break;               
        case CVE_HIDECURSOR: XUndefineCursor(g_dpy, g_win); break;
        case CVE_SHOWCURSOR: break; // XXX
        default: ret = 0;
        }
        return ret;
}

#define EVMASK  KeyPressMask | KeyReleaseMask | \
        ButtonPressMask | ButtonReleaseMask |   \
        PointerMotionMask | StructureNotifyMask

cvkey mapkeycode(unsigned k) {
        cvkey ret;
        switch (k) {
        case 9: ret = CVK_ESCAPE; break;
        case 10: ret = CVK_1; break;
        case 11: ret = CVK_2; break;
        case 12: ret = CVK_3; break;
        case 13: ret = CVK_4; break;
        case 14: ret = CVK_5; break;
        case 15: ret = CVK_6; break;
        case 16: ret = CVK_7; break;
        case 17: ret = CVK_8; break;
        case 18: ret = CVK_9; break;
        case 19: ret = CVK_0; break;
        case 20: ret = CVK_MINUS; break;
        case 21: ret = CVK_EQUAL; break;
        case 22: ret = CVK_DELETE; break;
        case 23: ret = CVK_TAB; break;
        case 24: ret = CVK_Q; break;
        case 25: ret = CVK_W; break;
        case 26: ret = CVK_E; break;
        case 27: ret = CVK_R; break;
        case 28: ret = CVK_T; break;
        case 29: ret = CVK_Y; break;
        case 30: ret = CVK_U; break;
        case 31: ret = CVK_I; break;
        case 32: ret = CVK_O; break;
        case 33: ret = CVK_P; break;
        case 34: ret = CVK_LEFTBRACKET; break;
        case 35: ret = CVK_RIGHTBRACKET; break;
        case 36: ret = CVK_RETURN; break;
        case 37: ret = CVK_CONTROL; break;
        case 38: ret = CVK_A; break;
        case 39: ret = CVK_S; break;
        case 40: ret = CVK_D; break;
        case 41: ret = CVK_F; break;
        case 42: ret = CVK_G; break;
        case 43: ret = CVK_H; break;
        case 44: ret = CVK_J; break;
        case 45: ret = CVK_K; break;
        case 46: ret = CVK_L; break;
        case 47: ret = CVK_SEMICOLON; break;
        case 48: ret = CVK_QUOTE; break;
        case 49: ret = CVK_GRAVE; break;
        case 50: ret = CVK_SHIFT; break;
        case 51: ret = CVK_BACKSLASH; break;
        case 52: ret = CVK_Z; break;
        case 53: ret = CVK_X; break;
        case 54: ret = CVK_C; break;
        case 55: ret = CVK_V; break;
        case 56: ret = CVK_B; break;
        case 57: ret = CVK_N; break;
        case 58: ret = CVK_M; break;
        case 59: ret = CVK_COMMA; break;
        case 60: ret = CVK_PERIOD; break;
        case 61: ret = CVK_SLASH; break;
        case 62: ret = CVK_RIGHTSHIFT; break;
        case 63: ret = CVK_KEYPADMULTIPLY; break;
        case 64: ret = CVK_OPTION; break;
        case 65: ret = CVK_SPACE; break;

        case 67: ret = CVK_F1; break;
        case 68: ret = CVK_F2; break;
        case 69: ret = CVK_F3; break;
        case 70: ret = CVK_F4; break;
        case 71: ret = CVK_F5; break;
        case 72: ret = CVK_F6; break;
        case 73: ret = CVK_F7; break;
        case 74: ret = CVK_F8; break;
        case 75: ret = CVK_F9; break;
        case 76: ret = CVK_F10; break;

        case 79: ret = CVK_KEYPAD7; break;
        case 80: ret = CVK_KEYPAD8; break;
        case 81: ret = CVK_KEYPAD9; break;
        case 82: ret = CVK_KEYPADMINUS; break;
        case 83: ret = CVK_KEYPAD4; break;
        case 84: ret = CVK_KEYPAD5; break;
        case 85: ret = CVK_KEYPAD6; break;
        case 86: ret = CVK_KEYPADPLUS; break;
        case 87: ret = CVK_KEYPAD1; break;
        case 88: ret = CVK_KEYPAD2; break;
        case 89: ret = CVK_KEYPAD3; break;
        case 90: ret = CVK_KEYPAD0; break;
        case 91: ret = CVK_KEYPADDECIMAL; break;

        case 104: ret = CVK_KEYPADENTER; break;

        case 106: ret = CVK_KEYPADDIVIDE; break;

        case 110: ret = CVK_HOME; break;

        case 113: ret = CVK_LEFTARROW; break;
        case 114: ret = CVK_RIGHTARROW; break;
        case 115: ret = CVK_END; break;
        case 116: ret = CVK_DOWNARROW; break;
        case 111: ret = CVK_UPARROW; break;
        case 112: ret = CVK_PAGEUP; break;

        case 117: ret = CVK_PAGEDOWN; break;

        case 119: ret = CVK_FORWARDDELETE; break;

        case 133: ret = CVK_COMMAND; break;
/*
  case XK_Clear: ret = CVK_KEYPADCLEAR; break;
  case XK_KP_Equal: ret = CVK_KEYPADEQUALS; break;
  case XK_Caps_Lock: ret = CVK_CAPSLOCK; break;
  case XK_Alt_R: ret = CVK_RIGHTOPTION; break;
  case XK_Control_R: ret = CVK_RIGHTCONTROL; break;
//      case XK_function: ret = CVK_FUNCTION; break;
case XK_F17: ret = CVK_F17; break;
//      case XK_VolumeUp: ret = CVK_VOLUMEUP; break;
//      case XK_VolumeDown: ret = CVK_VOLUMEDOWN; break;
//      case XK_Mute: ret = CVK_MUTE; break;
case XK_F18: ret = CVK_F18; break;
case XK_F19: ret = CVK_F19; break;
case XK_F20: ret = CVK_F20; break;

case XK_F11: ret = CVK_F11; break;
case XK_F13: ret = CVK_F13; break;
case XK_F16: ret = CVK_F16; break;
case XK_F14: ret = CVK_F14; break;
case XK_F12: ret = CVK_F12; break;
case XK_F15: ret = CVK_F15; break;
case XK_Help: ret = CVK_HELP; break;
*/
        default: ret = CVK_NONE;
        }
        return ret;
}

static int mapButton(int button) {
        int which;
        switch (button) {
        case Button1: which = CVK_MOUSELEFT; break;
        case Button2: which = CVK_MOUSEMIDDLE; break;
        case Button3: which = CVK_MOUSERIGHT; break;
        case Button4: which = CVK_MOUSEWHEELUP; break;
        case Button5: which = CVK_MOUSEWHEELDOWN; break;
        default: which = CVK_NONE;
        }
        return which;
}

static int isAutoRepeat(Display * dpy, Window win, XEvent * e) {
        XEvent next;
        XCheckTypedWindowEvent(dpy, win, KeyPress, &next);
        XPutBackEvent(dpy, &next);
        return next.xkey.keycode == e->xkey.keycode
                && next.xkey.time - e->xkey.time < 2;
}

static void handle(Display * dpy, Window win, XIC xic, XEvent * e) {
        switch(e->type) {
        case ClientMessage:
                cvInject(CVE_CLOSE, 0, 0);
                break;
        case ConfigureNotify:
                cvInject(CVE_RESIZE, e->xconfigure.width, e->xconfigure.height);
                break;
        case ButtonPress:
                cvInject(CVE_DOWN, mapButton(e->xbutton.button), 0);
                break;
        case ButtonRelease:
                cvInject(CVE_UP, mapButton(e->xbutton.button), 0);
                break;
        case MotionNotify:
                cvInject(CVE_MOTION, e->xmotion.x, e->xmotion.y);
                break;
        case KeyPress:
        {
                wchar_t buf[64];
                KeySym ks;
                Status st;
                int i;
                int n;
                cvInject(CVE_DOWN, mapkeycode(e->xkey.keycode), 0);
                n = XwcLookupString(xic,
                                    &e->xkey, 
                                    buf, 
                                    sizeof(buf) / sizeof(wchar_t),
                                    &ks, 
                                    &st);
                if (st == XLookupChars || st == XLookupBoth) 
                        for (i = 0; i < n; i++)
                                cvInject(CVE_UNICODE, buf[i], 0);
        }
        break;
        case KeyRelease:
                if (!isAutoRepeat(dpy, win, e)) 
                        cvInject(CVE_UP, mapkeycode(e->xkey.keycode), 0);

        default:
                break;
        }
}

int cvrun(int argc, char ** argv) {
        Display * dpy;
        Window win;
        int scr;
        GLXContext ctx;
        XIM xim;
        XIC xic;
        XSetWindowAttributes swa; 
        unsigned long swamask;
        Atom datom;
        int attr[] = {
                GLX_RGBA,
                GLX_DOUBLEBUFFER,
                GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DEPTH_SIZE, 1,
                None};
        XVisualInfo * vi;
        XSizeHints hints;
        dpy = XOpenDisplay(0);
        xim = XOpenIM(dpy, 0, 0, 0);
        swa.event_mask = EVMASK;
        swamask = CWEventMask;
        scr = XDefaultScreen(dpy);
        cvInject(CVE_INIT, 0, 0);
        hints.x = cvInject(CVQ_XPOS, 0, 0);
        hints.y = cvInject(CVQ_YPOS, 0, 0);
        hints.width = cvInject(CVQ_WIDTH, 0, 0);
        hints.height = cvInject(CVQ_HEIGHT, 0, 0);
        if (hints.width == -1) {
                hints.x = 0;
                hints.y = 0;
                hints.width = DisplayWidth(dpy, scr);
                hints.height = DisplayHeight(dpy, scr);
        }
        win = XCreateWindow(dpy, XRootWindow(dpy, scr),
                            hints.x, hints.y, hints.width, hints.height,
                            0, CopyFromParent,
                            InputOutput, 
                            CopyFromParent,
                            swamask, &swa);
        g_dpy = dpy;
        g_win = win;
        hints.flags = USSize | USPosition;
        XSetWMNormalHints(dpy, win, &hints);
        if (!cvInject(CVQ_BORDERS, 0, 0)) {
                Atom hatom = XInternAtom(dpy, "_MOTIF_WM_HINTS", 1);
                long flags[5] = {0};
                //mwm.flags = MWM_HINTS_DECORATIONS;
                flags[0] = 2; flags[1] = 0; flags[2] = 0;
                flags[3] = 0; flags[4] = 0;
                XChangeProperty(dpy, win, hatom, hatom, 
                                32, PropModeReplace,
                                (unsigned char*)flags, sizeof(flags) / 4);
        }
        datom = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(dpy, win, &datom, 1);
        XSelectInput(dpy, win, EVMASK);
        xic = XCreateIC(xim,
                        XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                        XNClientWindow, win,
                        XNFocusWindow, win,
                        NULL);
        vi = glXChooseVisual(dpy, scr, attr);
        ctx = glXCreateContext(dpy, vi, 0, True);
        XFree(vi);
        XMapWindow(dpy, win);
        glXMakeCurrent(dpy, win, ctx);
        ((int(*)(int))glXGetProcAddress((GLubyte*)"glXSwapIntervalSGI"))(1);
        cvInject(CVE_GLINIT, 0, 0);
        while (!g_done) {
                XEvent e;
                if (XCheckWindowEvent(dpy, win, EVMASK, &e)
                    || XCheckTypedWindowEvent(dpy, win, ClientMessage, &e))
                        handle(dpy, win, xic, &e);
                glXSwapBuffers(dpy, win);
                cvInject(CVE_UPDATE, 0, 0);
        }
        cvInject(CVE_GLTERM, 0, 0);
        glXMakeCurrent(dpy, 0, 0);
        XDestroyIC(xic);
        XUnmapWindow(dpy, win);
        glXDestroyContext(dpy, ctx);
        XDestroyWindow(dpy, win);
        XCloseIM(xim);
        XCloseDisplay(dpy);
        return cvInject(CVE_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
