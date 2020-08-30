#include "cv.c"
#include <GL/glx.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/keysym.h>
#include <assert.h>

static int g_done = 0;
static Display * g_dpy;
static Window g_win = 0;
static XIM g_xim;
static XIC g_xic;
static GLXContext g_ctx;
static Cursor g_cursor;
static int g_nx, g_ny;
static unsigned g_nw, g_nh;

#define EVMASK                                                                 \
        KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |  \
            PointerMotionMask | StructureNotifyMask

static int scr() { return XDefaultScreen(g_dpy); }

static void borders(int show) {
        Atom hatom = XInternAtom(g_dpy, "_MOTIF_WM_HINTS", 1);
        long flags[5];
        flags[0] = 2;    // MWM_HINTS_DECORATIONS
        flags[2] = show;
        flags[1] = flags[3] = flags[4] = 0;
        XChangeProperty(g_dpy, g_win, hatom, hatom, 32, PropModeReplace,
                        (unsigned char *)flags, sizeof(flags) / 4);
        XFlush(g_dpy);
}

static void openwin(int x, int y, unsigned w, unsigned h, int b) {
        XSizeHints hints;
        XSetWindowAttributes swa;
        unsigned long swamask;
        Atom datom;
        swa.event_mask = EVMASK;
        swamask = CWEventMask;
        hints.x = x;
        hints.y = y;
        hints.width = w;
        hints.height = h;
        assert(!g_win);
        g_win =
            XCreateWindow(g_dpy, XRootWindow(g_dpy, scr()), hints.x, hints.y,
                          hints.width, hints.height, 0, CopyFromParent,
                          InputOutput, CopyFromParent, swamask, &swa);
        borders(b);
        hints.flags = USSize | USPosition;
        XSetWMNormalHints(g_dpy, g_win, &hints);
        datom = XInternAtom(g_dpy, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(g_dpy, g_win, &datom, 1);
        XSelectInput(g_dpy, g_win, EVMASK);
        g_xic =
            XCreateIC(g_xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                      XNClientWindow, g_win, XNFocusWindow, g_win, NULL);
        if (g_cursor)
                XDefineCursor(g_dpy, g_win, g_cursor);
        XStoreName(g_dpy, g_win, (const char *)cvInject(CVQ_NAME, 0, 0));
}

static void unmap() {
        glXMakeCurrent(g_dpy, 0, 0);
        XUnmapWindow(g_dpy, g_win);
}

static void closewin() {
        unmap();
        XDestroyIC(g_xic);
        g_xic = 0;
        XDestroyWindow(g_dpy, g_win);
        g_win = 0;
}

static void map() {
        XMapWindow(g_dpy, g_win);
        glXMakeCurrent(g_dpy, g_win, g_ctx);
}

static void savegeom() {
        Window root;
        Window child;
        int nx, ny;
        unsigned border;
        unsigned depth;
        XGetGeometry(g_dpy, g_win, &root, &nx, &ny, &g_nw, &g_nh, &border,
                     &depth);
        XTranslateCoordinates(g_dpy, g_win, root, -nx, -ny, &g_nx, &g_ny,
                              &child);
}

static void defaultcursor() {
        XUndefineCursor(g_dpy, g_win);
        if (g_cursor)
                XFreeCursor(g_dpy, g_cursor);
        g_cursor = 0;
}

static void setcursor(uint8_t * rgba, int16_t hotx, int16_t hoty) {
        XcursorImage * ci = XcursorImageCreate(32, 32);
        unsigned i;
        uint32_t * dst = (uint32_t *)ci->pixels;
        defaultcursor();
        ci->xhot = hotx;
        ci->yhot = hoty;
        for (i = 0; i < 32 * 32; i++)
                dst[i] = 0 + (rgba[i * 4 + 3] << 24) + (rgba[i * 4 + 0] << 16) +
                         (rgba[i * 4 + 1] << 8) + (rgba[i * 4 + 2] << 0);
        g_cursor = XcursorImageLoadCursor(g_dpy, ci);
        XDefineCursor(g_dpy, g_win, g_cursor);
        XcursorImageDestroy(ci);
}

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        static int fullscreen = 0;
        switch (evType(e)) {
        case CVE_QUIT:
                g_done = 1;
                break;
        case CVE_SETCURSOR:
                setcursor((uint8_t *)evArg0(e), evArg1(e) >> 16,
                          evArg1(e) & 0xffff);
                break;
        case CVE_DEFAULTCURSOR:
                defaultcursor();
                break;
        case CVE_WINDOWED:
                if (fullscreen) {
                        closewin();
                        openwin(g_nx, g_ny, g_nw, g_nh, 1);
                        map();
                        fullscreen = 0;
                }
                break;
        case CVE_FULLSCREEN:
                if (!fullscreen) {
                        savegeom();
                        closewin();
                        openwin(0, 0, DisplayWidth(g_dpy, scr()),
                                DisplayHeight(g_dpy, scr()), 0);
                        map();
                        fullscreen = 1;
                }
                break;
        default:
                ret = 0;
        }
        return ret;
}

#ifdef __APPLE__
static cvkey mapkeycode(unsigned k) {
        cvkey ret;
        switch (k) {
        case 61:
                ret = CVK_ESCAPE;
                break;
        case 9:
                ret = CVK_S;
                break;
        case 10:
                ret = CVK_D;
                break;
        case 11:
                ret = CVK_F;
                break;
        case 12:
                ret = CVK_H;
                break;
        case 16:
                ret = CVK_C;
                break;
        case 21:
                ret = CVK_W;
                break;
        case 53:
                ret = CVK_N;
                break;
        default:
                printf("%d\n", k);
                ret = CVK_NONE;
        }
        return ret;
}

#else
cvkey mapkeycode(unsigned k) {
        cvkey ret;
        switch (k) {
        case 9:
                ret = CVK_ESCAPE;
                break;
        case 10:
                ret = CVK_1;
                break;
        case 11:
                ret = CVK_2;
                break;
        case 12:
                ret = CVK_3;
                break;
        case 13:
                ret = CVK_4;
                break;
        case 14:
                ret = CVK_5;
                break;
        case 15:
                ret = CVK_6;
                break;
        case 16:
                ret = CVK_7;
                break;
        case 17:
                ret = CVK_8;
                break;
        case 18:
                ret = CVK_9;
                break;
        case 19:
                ret = CVK_0;
                break;
        case 20:
                ret = CVK_MINUS;
                break;
        case 21:
                ret = CVK_EQUAL;
                break;
        case 22:
                ret = CVK_DELETE;
                break;
        case 23:
                ret = CVK_TAB;
                break;
        case 24:
                ret = CVK_Q;
                break;
        case 25:
                ret = CVK_W;
                break;
        case 26:
                ret = CVK_E;
                break;
        case 27:
                ret = CVK_R;
                break;
        case 28:
                ret = CVK_T;
                break;
        case 29:
                ret = CVK_Y;
                break;
        case 30:
                ret = CVK_U;
                break;
        case 31:
                ret = CVK_I;
                break;
        case 32:
                ret = CVK_O;
                break;
        case 33:
                ret = CVK_P;
                break;
        case 34:
                ret = CVK_LEFTBRACKET;
                break;
        case 35:
                ret = CVK_RIGHTBRACKET;
                break;
        case 36:
                ret = CVK_RETURN;
                break;
        case 37:
                ret = CVK_CONTROL;
                break;
        case 38:
                ret = CVK_A;
                break;
        case 39:
                ret = CVK_S;
                break;
        case 40:
                ret = CVK_D;
                break;
        case 41:
                ret = CVK_F;
                break;
        case 42:
                ret = CVK_G;
                break;
        case 43:
                ret = CVK_H;
                break;
        case 44:
                ret = CVK_J;
                break;
        case 45:
                ret = CVK_K;
                break;
        case 46:
                ret = CVK_L;
                break;
        case 47:
                ret = CVK_SEMICOLON;
                break;
        case 48:
                ret = CVK_QUOTE;
                break;
        case 49:
                ret = CVK_GRAVE;
                break;
        case 50:
                ret = CVK_SHIFT;
                break;
        case 51:
                ret = CVK_BACKSLASH;
                break;
        case 52:
                ret = CVK_Z;
                break;
        case 53:
                ret = CVK_X;
                break;
        case 54:
                ret = CVK_C;
                break;
        case 55:
                ret = CVK_V;
                break;
        case 56:
                ret = CVK_B;
                break;
        case 57:
                ret = CVK_N;
                break;
        case 58:
                ret = CVK_M;
                break;
        case 59:
                ret = CVK_COMMA;
                break;
        case 60:
                ret = CVK_PERIOD;
                break;
        case 61:
                ret = CVK_SLASH;
                break;
        case 62:
                ret = CVK_RIGHTSHIFT;
                break;
        case 63:
                ret = CVK_KEYPADMULTIPLY;
                break;
        case 64:
                ret = CVK_OPTION;
                break;
        case 65:
                ret = CVK_SPACE;
                break;

        case 67:
                ret = CVK_F1;
                break;
        case 68:
                ret = CVK_F2;
                break;
        case 69:
                ret = CVK_F3;
                break;
        case 70:
                ret = CVK_F4;
                break;
        case 71:
                ret = CVK_F5;
                break;
        case 72:
                ret = CVK_F6;
                break;
        case 73:
                ret = CVK_F7;
                break;
        case 74:
                ret = CVK_F8;
                break;
        case 75:
                ret = CVK_F9;
                break;
        case 76:
                ret = CVK_F10;
                break;

        case 79:
                ret = CVK_KEYPAD7;
                break;
        case 80:
                ret = CVK_KEYPAD8;
                break;
        case 81:
                ret = CVK_KEYPAD9;
                break;
        case 82:
                ret = CVK_KEYPADMINUS;
                break;
        case 83:
                ret = CVK_KEYPAD4;
                break;
        case 84:
                ret = CVK_KEYPAD5;
                break;
        case 85:
                ret = CVK_KEYPAD6;
                break;
        case 86:
                ret = CVK_KEYPADPLUS;
                break;
        case 87:
                ret = CVK_KEYPAD1;
                break;
        case 88:
                ret = CVK_KEYPAD2;
                break;
        case 89:
                ret = CVK_KEYPAD3;
                break;
        case 90:
                ret = CVK_KEYPAD0;
                break;
        case 91:
                ret = CVK_KEYPADDECIMAL;
                break;

        case 104:
                ret = CVK_KEYPADENTER;
                break;

        case 106:
                ret = CVK_KEYPADDIVIDE;
                break;

        case 110:
                ret = CVK_HOME;
                break;

        case 113:
                ret = CVK_LEFTARROW;
                break;
        case 114:
                ret = CVK_RIGHTARROW;
                break;
        case 115:
                ret = CVK_END;
                break;
        case 116:
                ret = CVK_DOWNARROW;
                break;
        case 111:
                ret = CVK_UPARROW;
                break;
        case 112:
                ret = CVK_PAGEUP;
                break;

        case 117:
                ret = CVK_PAGEDOWN;
                break;

        case 119:
                ret = CVK_FORWARDDELETE;
                break;

        case 133:
                ret = CVK_COMMAND;
                break;
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
        default:
                ret = CVK_NONE;
        }
        return ret;
}
#endif

static int mapButton(int button) {
        int which;
        switch (button) {
        case Button1:
                which = CVK_MOUSELEFT;
                break;
        case Button2:
                which = CVK_MOUSEMIDDLE;
                break;
        case Button3:
                which = CVK_MOUSERIGHT;
                break;
        case Button4:
                which = CVK_MOUSEWHEELUP;
                break;
        case Button5:
                which = CVK_MOUSEWHEELDOWN;
                break;
        default:
                which = CVK_NONE;
        }
        return which;
}

static int isAutoRepeat(Display * dpy, Window win, XEvent * e) {
        XEvent next;
        XCheckTypedWindowEvent(dpy, win, KeyPress, &next);
        XPutBackEvent(dpy, &next);
        return next.xkey.keycode == e->xkey.keycode &&
               next.xkey.time - e->xkey.time < 2;
}

static void handle(Display * dpy, Window win, XIC xic, XEvent * e) {
        switch (e->type) {
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
        case KeyPress: {
                wchar_t buf[64];
                KeySym ks;
                Status st;
                int i;
                int n;
                cvInject(CVE_DOWN, mapkeycode(e->xkey.keycode), 0);
                n = XwcLookupString(xic, &e->xkey, buf,
                                    sizeof(buf) / sizeof(wchar_t), &ks, &st);
                if (st == XLookupChars || st == XLookupBoth)
                        for (i = 0; i < n; i++)
                                cvInject(CVE_UNICODE, buf[i], 0);
        } break;
        case KeyRelease:
                if (!isAutoRepeat(dpy, win, e))
                        cvInject(CVE_UP, mapkeycode(e->xkey.keycode), 0);

        default:
                break;
        }
}

int cvrun(int argc, char ** argv) {
        int attr[] = { GLX_RGBA,
                       GLX_DOUBLEBUFFER,
                       GLX_RED_SIZE,
                       1,
                       GLX_GREEN_SIZE,
                       1,
                       GLX_BLUE_SIZE,
                       1,
                       GLX_DEPTH_SIZE,
                       1,
                       None };
        XVisualInfo * vi;
        g_dpy = XOpenDisplay(0);
        g_xim = XOpenIM(g_dpy, 0, 0, 0);
        cvInject(CVE_INIT, 0, 0);
        openwin(cvInject(CVQ_XPOS, 0, 0), cvInject(CVQ_YPOS, 0, 0),
                cvInject(CVQ_WIDTH, 0, 0), cvInject(CVQ_HEIGHT, 0, 0), 1);
        vi = glXChooseVisual(g_dpy, scr(), attr);
        g_ctx = glXCreateContext(g_dpy, vi, 0, True);
        XFree(vi);

        map();
        ((int (*)(int))glXGetProcAddress((GLubyte *)"glXSwapIntervalSGI"))(1);
        cvInject(CVE_GLINIT, 0, 0);
        while (!g_done) {
                XEvent e;
                while (XCheckWindowEvent(g_dpy, g_win, EVMASK, &e) ||
                       XCheckTypedWindowEvent(g_dpy, g_win, ClientMessage, &e))
                        handle(g_dpy, g_win, g_xic, &e);
                glXSwapBuffers(g_dpy, g_win);
                cvInject(CVE_UPDATE, 0, 0);
        }
        cvInject(CVE_GLTERM, 0, 0);
        closewin();
        glXDestroyContext(g_dpy, g_ctx);
        XCloseIM(g_xim);
        XCloseDisplay(g_dpy);
        return cvInject(CVE_TERM, 0, 0);
}
