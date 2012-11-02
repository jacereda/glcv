#include "gs.c"
#include <GL/glx.h>
#include <X11/keysym.h>

static int g_done = 0;

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case GSE_QUIT: g_done = 1; break;               
        default: ret = 0;
        }
        return ret;
}

#define EVMASK  KeyPressMask | KeyReleaseMask | \
        ButtonPressMask | ButtonReleaseMask |   \
        PointerMotionMask | StructureNotifyMask

gskey mapkeycode(unsigned k) {
        gskey ret;
        switch (k) {
        case 9: ret = GSK_ESCAPE; break;
        case 10: ret = GSK_1; break;
        case 11: ret = GSK_2; break;
        case 12: ret = GSK_3; break;
        case 13: ret = GSK_4; break;
        case 14: ret = GSK_5; break;
        case 15: ret = GSK_6; break;
        case 16: ret = GSK_7; break;
        case 17: ret = GSK_8; break;
        case 18: ret = GSK_9; break;
        case 19: ret = GSK_0; break;
        case 20: ret = GSK_MINUS; break;
        case 21: ret = GSK_EQUAL; break;
        case 22: ret = GSK_DELETE; break;
        case 23: ret = GSK_TAB; break;
        case 24: ret = GSK_Q; break;
        case 25: ret = GSK_W; break;
        case 26: ret = GSK_E; break;
        case 27: ret = GSK_R; break;
        case 28: ret = GSK_T; break;
        case 29: ret = GSK_Y; break;
        case 30: ret = GSK_U; break;
        case 31: ret = GSK_I; break;
        case 32: ret = GSK_O; break;
        case 33: ret = GSK_P; break;
        case 34: ret = GSK_LEFTBRACKET; break;
        case 35: ret = GSK_RIGHTBRACKET; break;
        case 36: ret = GSK_RETURN; break;
        case 37: ret = GSK_CONTROL; break;
        case 38: ret = GSK_A; break;
        case 39: ret = GSK_S; break;
        case 40: ret = GSK_D; break;
        case 41: ret = GSK_F; break;
        case 42: ret = GSK_G; break;
        case 43: ret = GSK_H; break;
        case 44: ret = GSK_J; break;
        case 45: ret = GSK_K; break;
        case 46: ret = GSK_L; break;
        case 47: ret = GSK_SEMICOLON; break;
        case 48: ret = GSK_QUOTE; break;
        case 49: ret = GSK_GRAVE; break;
        case 50: ret = GSK_SHIFT; break;
        case 51: ret = GSK_BACKSLASH; break;
        case 52: ret = GSK_Z; break;
        case 53: ret = GSK_X; break;
        case 54: ret = GSK_C; break;
        case 55: ret = GSK_V; break;
        case 56: ret = GSK_B; break;
        case 57: ret = GSK_N; break;
        case 58: ret = GSK_M; break;
        case 59: ret = GSK_COMMA; break;
        case 60: ret = GSK_PERIOD; break;
        case 61: ret = GSK_SLASH; break;
        case 62: ret = GSK_RIGHTSHIFT; break;
        case 63: ret = GSK_KEYPADMULTIPLY; break;
        case 64: ret = GSK_OPTION; break;
        case 65: ret = GSK_SPACE; break;

        case 67: ret = GSK_F1; break;
        case 68: ret = GSK_F2; break;
        case 69: ret = GSK_F3; break;
        case 70: ret = GSK_F4; break;
        case 71: ret = GSK_F5; break;
        case 72: ret = GSK_F6; break;
        case 73: ret = GSK_F7; break;
        case 74: ret = GSK_F8; break;
        case 75: ret = GSK_F9; break;
        case 76: ret = GSK_F10; break;

        case 79: ret = GSK_KEYPAD7; break;
        case 80: ret = GSK_KEYPAD8; break;
        case 81: ret = GSK_KEYPAD9; break;
        case 82: ret = GSK_KEYPADMINUS; break;
        case 83: ret = GSK_KEYPAD4; break;
        case 84: ret = GSK_KEYPAD5; break;
        case 85: ret = GSK_KEYPAD6; break;
        case 86: ret = GSK_KEYPADPLUS; break;
        case 87: ret = GSK_KEYPAD1; break;
        case 88: ret = GSK_KEYPAD2; break;
        case 89: ret = GSK_KEYPAD3; break;
        case 90: ret = GSK_KEYPAD0; break;
        case 91: ret = GSK_KEYPADDECIMAL; break;

        case 104: ret = GSK_KEYPADENTER; break;

        case 106: ret = GSK_KEYPADDIVIDE; break;

        case 110: ret = GSK_HOME; break;

        case 113: ret = GSK_LEFTARROW; break;
        case 114: ret = GSK_RIGHTARROW; break;
        case 115: ret = GSK_END; break;
        case 116: ret = GSK_DOWNARROW; break;
        case 111: ret = GSK_UPARROW; break;
        case 112: ret = GSK_PAGEUP; break;

        case 117: ret = GSK_PAGEDOWN; break;

        case 119: ret = GSK_FORWARDDELETE; break;

        case 133: ret = GSK_COMMAND; break;
/*
  case XK_Clear: ret = GSK_KEYPADCLEAR; break;
  case XK_KP_Equal: ret = GSK_KEYPADEQUALS; break;
  case XK_Caps_Lock: ret = GSK_CAPSLOCK; break;
  case XK_Alt_R: ret = GSK_RIGHTOPTION; break;
  case XK_Control_R: ret = GSK_RIGHTCONTROL; break;
//      case XK_function: ret = GSK_FUNCTION; break;
case XK_F17: ret = GSK_F17; break;
//      case XK_VolumeUp: ret = GSK_VOLUMEUP; break;
//      case XK_VolumeDown: ret = GSK_VOLUMEDOWN; break;
//      case XK_Mute: ret = GSK_MUTE; break;
case XK_F18: ret = GSK_F18; break;
case XK_F19: ret = GSK_F19; break;
case XK_F20: ret = GSK_F20; break;

case XK_F11: ret = GSK_F11; break;
case XK_F13: ret = GSK_F13; break;
case XK_F16: ret = GSK_F16; break;
case XK_F14: ret = GSK_F14; break;
case XK_F12: ret = GSK_F12; break;
case XK_F15: ret = GSK_F15; break;
case XK_Help: ret = GSK_HELP; break;
*/
        default: ret = GSK_NONE;
        }
        return ret;
}

static int mapButton(int button) {
        int which;
        switch (button) {
        case Button1: which = GSK_MOUSELEFT; break;
        case Button2: which = GSK_MOUSEMIDDLE; break;
        case Button3: which = GSK_MOUSERIGHT; break;
        case Button4: which = GSK_MOUSEWHEELUP; break;
        case Button5: which = GSK_MOUSEWHEELDOWN; break;
        default: which = GSK_NONE;
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
                gsInject(GSE_CLOSE, 0, 0);
                break;
        case ConfigureNotify:
                gsInject(GSE_RESIZE, e->xconfigure.width, e->xconfigure.height);
                gsInject(GSE_GLINIT, 0, 0);
                break;
        case ButtonPress:
                gsInject(GSE_DOWN, mapButton(e->xbutton.button), 0);
                break;
        case ButtonRelease:
                gsInject(GSE_UP, mapButton(e->xbutton.button), 0);
                break;
        case MotionNotify:
                gsInject(GSE_MOTION, e->xmotion.x, e->xmotion.y);
                break;
        case KeyPress:
        {
                wchar_t buf[64];
                KeySym ks;
                Status st;
                int i;
                int n;
                gsInject(GSE_DOWN, mapkeycode(e->xkey.keycode), 0);
                n = XwcLookupString(xic,
                                    &e->xkey, 
                                    buf, 
                                    sizeof(buf) / sizeof(wchar_t),
                                    &ks, 
                                    &st);
                if (st == XLookupChars || st == XLookupBoth) 
                        for (i = 0; i < n; i++)
                                gsInject(GSE_UNICODE, buf[i], 0);
        }
        break;
        case KeyRelease:
                if (!isAutoRepeat(dpy, win, e)) 
                        gsInject(GSE_UP, mapkeycode(e->xkey.keycode), 0);

        default:
                break;
        }
}

int gsrun(int argc, char ** argv) {
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
        hints.x = gsInject(GSQ_XPOS, 0, 0);
        hints.y = gsInject(GSQ_YPOS, 0, 0);
        hints.width = gsInject(GSQ_WIDTH, 0, 0);
        hints.height = gsInject(GSQ_HEIGHT, 0, 0);
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
        hints.flags = USSize | USPosition;
        XSetWMNormalHints(dpy, win, &hints);
        if (!gsInject(GSQ_BORDERS, 0, 0)) {
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
        XUndefineCursor(dpy, win);
        glXMakeCurrent(dpy, win, ctx);
        ((int(*)(int))glXGetProcAddress((GLubyte*)"glXSwapIntervalSGI"))(1);
        while (!g_done) {
                XEvent e;
                if (XCheckWindowEvent(dpy, win, EVMASK, &e)
                    || XCheckTypedWindowEvent(dpy, win, ClientMessage, &e))
                        handle(dpy, win, xic, &e);
                glXSwapBuffers(dpy, win);
                gsInject(GSE_UPDATE, 0, 0);
        }
        glXMakeCurrent(dpy, 0, 0);
        XDestroyIC(xic);
        XUnmapWindow(dpy, win);
        glXDestroyContext(dpy, ctx);
        XDestroyWindow(dpy, win);
        XCloseIM(xim);
        XCloseDisplay(dpy);
        return gsInject(GSE_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
