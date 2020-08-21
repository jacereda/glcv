#include "npapi.c"
#include <assert.h>
#include <windows.h>
#include <windowsx.h>

#if !defined HANDLE_WM_MOUSEWHEEL
#define HANDLE_WM_MOUSEWHEEL(hwnd,wParam,lParam,fn) ((fn)((hwnd),(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam),(int)(short)HIWORD(wParam),(UINT)(short)LOWORD(wParam)),0L)
#endif

static WNDPROC s_oldproc = 0;
static HGLRC s_rc = 0;
static HANDLE s_win = 0;

static inline BOOL _chk(int line, BOOL b) {
#if !defined NDEBUG
        int err = GetLastError();
        if (err)
                cvReport("%d: got error %d ret %d", line, err, b);
#endif
        return b;
}

#define chk(x) _chk(__LINE__, x)

static void setPF(HDC dc) {
        int pf;
        PIXELFORMATDESCRIPTOR pfd = { 
                sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
                1,                     // version number 
                PFD_DRAW_TO_WINDOW |   // support window 
                PFD_SUPPORT_OPENGL |   // support OpenGL 
                PFD_DOUBLEBUFFER,      // double buffered 
                PFD_TYPE_RGBA,         // RGBA type 
                24,                    // 24-bit color depth 
                0, 0, 0, 0, 0, 0,      // color bits ignored 
                0,                     // no alpha buffer 
                0,                     // shift bit ignored 
                0,                     // no accumulation buffer 
                0, 0, 0, 0,            // accum bits ignored 
                32,                    // 32-bit z-buffer 
                0,                     // no stencil buffer 
                0,                     // no auxiliary buffer 
                PFD_MAIN_PLANE,        // main layer 
                0,                     // reserved 
                0, 0, 0                // layer masks ignored 
        }; 
        pf = ChoosePixelFormat(dc, &pfd);
        cvReport("pf: %d", pf);
        chk(SetPixelFormat(dc, pf, &pfd)); 
}

static unsigned uc2aw(unsigned uc) {
        unsigned ret = uc;
        switch (uc) {
        case 0xd: ret = '\n'; break;
        }
        return ret;
}

static unsigned mapkeycode(unsigned vk) {
        int ret;
        switch (vk) {
        case 'A': ret = CVK_A; break;
        case 'S': ret = CVK_S; break;
        case 'D': ret = CVK_D; break;
        case 'F': ret = CVK_F; break;
        case 'H': ret = CVK_H; break;
        case 'G': ret = CVK_G; break;
        case 'Z': ret = CVK_Z; break;
        case 'X': ret = CVK_X; break;
        case 'C': ret = CVK_C; break;
        case 'V': ret = CVK_V; break;
        case 'B': ret = CVK_B; break;
        case 'Q': ret = CVK_Q; break;
        case 'W': ret = CVK_W; break;
        case 'E': ret = CVK_E; break;
        case 'R': ret = CVK_R; break;
        case 'Y': ret = CVK_Y; break;
        case 'T': ret = CVK_T; break;
        case '1': ret = CVK_1; break;
        case '2': ret = CVK_2; break;
        case '3': ret = CVK_3; break;
        case '4': ret = CVK_4; break;
        case '6': ret = CVK_6; break;
        case '5': ret = CVK_5; break;
        case VK_OEM_PLUS: ret = CVK_EQUAL; break;
        case '9': ret = CVK_9; break;
        case '7': ret = CVK_7; break;
        case VK_OEM_MINUS: ret = CVK_MINUS; break;
        case '8': ret = CVK_8; break;
        case '0': ret = CVK_0; break;
        case VK_OEM_6: ret = CVK_RIGHTBRACKET; break;
        case 'O': ret = CVK_O; break;
        case 'U': ret = CVK_U; break;
        case VK_OEM_4: ret = CVK_LEFTBRACKET; break;
        case 'I': ret = CVK_I; break;
        case 'P': ret = CVK_P; break;
        case 'L': ret = CVK_L; break;
        case 'J': ret = CVK_J; break;
        case VK_OEM_7: ret = CVK_QUOTE; break;
        case 'K': ret = CVK_K; break;
        case VK_OEM_1: ret = CVK_SEMICOLON; break;
        case VK_OEM_5: ret = CVK_BACKSLASH; break;
        case VK_OEM_COMMA: ret = CVK_COMMA; break;
        case VK_OEM_2: ret = CVK_SLASH; break;
        case 'N': ret = CVK_N; break;
        case 'M': ret = CVK_M; break;
        case VK_OEM_PERIOD: ret = CVK_PERIOD; break;
        case VK_OEM_3: ret = CVK_GRAVE; break;
        case VK_DECIMAL: ret = CVK_KEYPADDECIMAL; break;
        case VK_MULTIPLY: ret = CVK_KEYPADMULTIPLY; break;
        case VK_ADD: ret = CVK_KEYPADPLUS; break;
//      case 'K'eypadClear: ret = CVK_KEYPADCLEAR; break;
        case VK_DIVIDE: ret = CVK_KEYPADDIVIDE; break;
//      case 'K'eypadEnter: ret = CVK_KEYPADENTER; break;
        case VK_SUBTRACT: ret = CVK_KEYPADMINUS; break;
//      case 'K'eypadEquals: ret = CVK_KEYPADEQUALS; break;
        case VK_NUMPAD0: ret = CVK_KEYPAD0; break;
        case VK_NUMPAD1: ret = CVK_KEYPAD1; break;
        case VK_NUMPAD2: ret = CVK_KEYPAD2; break;
        case VK_NUMPAD3: ret = CVK_KEYPAD3; break;
        case VK_NUMPAD4: ret = CVK_KEYPAD4; break;
        case VK_NUMPAD5: ret = CVK_KEYPAD5; break;
        case VK_NUMPAD6: ret = CVK_KEYPAD6; break;
        case VK_NUMPAD7: ret = CVK_KEYPAD7; break;
        case VK_NUMPAD8: ret = CVK_KEYPAD8; break;
        case VK_NUMPAD9: ret = CVK_KEYPAD9; break;
        case VK_RETURN: ret = CVK_RETURN; break;
        case VK_TAB: ret = CVK_TAB; break;
        case VK_SPACE: ret = CVK_SPACE; break;
        case VK_BACK: ret = CVK_DELETE; break;
        case VK_ESCAPE: ret = CVK_ESCAPE; break;
        case VK_LMENU:
        case VK_MENU: ret = CVK_OPTION; break;
        case VK_LSHIFT:
        case VK_SHIFT: ret = CVK_SHIFT; break;
        case VK_LWIN:
        case VK_RWIN: ret = CVK_COMMAND; break;
        case VK_CAPITAL: ret = CVK_CAPSLOCK; break;
        case VK_CONTROL: ret = CVK_CONTROL; break;
        case VK_RSHIFT: ret = CVK_RIGHTSHIFT; break;
        case VK_RMENU: ret = CVK_RIGHTOPTION; break;
        case VK_RCONTROL: ret = CVK_RIGHTCONTROL; break;
        case VK_F17: ret = CVK_F17; break;
        case VK_VOLUME_UP: ret = CVK_VOLUMEUP; break;
        case VK_VOLUME_DOWN: ret = CVK_VOLUMEDOWN; break;
        case VK_VOLUME_MUTE: ret = CVK_MUTE; break;
        case VK_F18: ret = CVK_F18; break;
        case VK_F19: ret = CVK_F19; break;
        case VK_F20: ret = CVK_F20; break;
        case VK_F5: ret = CVK_F5; break;
        case VK_F6: ret = CVK_F6; break;
        case VK_F7: ret = CVK_F7; break;
        case VK_F3: ret = CVK_F3; break;
        case VK_F8: ret = CVK_F8; break;
        case VK_F9: ret = CVK_F9; break;
        case VK_F11: ret = CVK_F11; break;
        case VK_F13: ret = CVK_F13; break;
        case VK_F16: ret = CVK_F16; break;
        case VK_F14: ret = CVK_F14; break;
        case VK_F10: ret = CVK_F10; break;
        case VK_F12: ret = CVK_F12; break;
        case VK_F15: ret = CVK_F15; break;
        case VK_HELP: ret = CVK_HELP; break;
        case VK_HOME: ret = CVK_HOME; break;
        case VK_PRIOR: ret = CVK_PAGEUP; break;
        case VK_INSERT: ret = CVK_FUNCTION; break;
        case VK_DELETE: ret = CVK_FORWARDDELETE; break;
        case VK_F4: ret = CVK_F4; break;
        case VK_END: ret = CVK_END; break;
        case VK_F2: ret = CVK_F2; break;
        case VK_NEXT: ret = CVK_PAGEDOWN; break;
        case VK_F1: ret = CVK_F1; break;
        case VK_LEFT: ret = CVK_LEFTARROW; break;
        case VK_RIGHT: ret = CVK_RIGHTARROW; break;
        case VK_DOWN: ret = CVK_DOWNARROW; break;
        case VK_UP: ret = CVK_UPARROW; break;
        case VK_SCROLL: ret = CVK_SCROLL; break;
        case VK_NUMLOCK: ret = CVK_NUMLOCK; break;
        case VK_CLEAR: ret = CVK_CLEAR; break;
        case VK_SNAPSHOT: ret = CVK_SYSREQ; break;
        case VK_PAUSE: ret = CVK_PAUSE; break;
        default: ret = CVK_NONE; break;
        }
        return ret;
}

static int wgot(HWND win, cveventtype evt, intptr_t a0, intptr_t a1) {
        int handled = win == s_win;
        if (handled)
                cvInject(evt, a0, a1);
        else
                cvReport("not handled: win: %p s_win: %p", win, s_win);
        return handled;
}

static int onPAINT(HWND win) {
        int handled = s_win == win;
        if (handled) {
//              PAINTSTRUCT ps;
//              HDC dc = BeginPaint(win, &ps);
                HDC dc = GetDC(win);
                cvReport("Painting");
//              chk(wglMakeCurrent(s_dc, s_rc));
                cvInject(CVE_UPDATE, 0, 0);
                chk(SwapBuffers(dc));
                ReleaseDC(win, dc);
//              chk(EndPaint(win, &ps));
        }
        return handled;
}

static int onTIMER(HWND win, UINT i) {
        chk(PostMessage(win, WM_PAINT, 0, 0));
        return 1;
}

static int onMOUSEMOVE(HWND win, int x, int y, UINT flacv ) {
        return wgot(win, CVE_MOTION, x, y);
}

static int onSIZE(HWND win, UINT state, int w, int h) {
        return wgot(win, CVE_RESIZE, w, h);
}

static int onCLOSE(HWND win) {
        return wgot(win, CVE_CLOSE, 0, 0);
}
static int onSYSKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        return wgot(win, CVE_DOWN, mapkeycode(vk), 0);
}

static int onSYSKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        return wgot(win, CVE_UP, mapkeycode(vk), 0);
}

static int onKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        return wgot(win, CVE_DOWN, mapkeycode(vk), 0);
}

static int onKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        return wgot(win, CVE_UP, mapkeycode(vk), 0);
}

static int mouseDown(HWND win, int which) {
        SetCapture(win);
        return wgot(win, CVE_DOWN, which, 0);        
}

static int mouseUp(HWND win, int which) {
        int ret = wgot(win, CVE_UP, which, 0);        
        ReleaseCapture();
        return ret;
}

static int onLBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(win, CVK_MOUSELEFT);
}

static int onMBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(win, CVK_MOUSEMIDDLE);
}

static int onRBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(win, CVK_MOUSERIGHT);
}

static int onLBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(win, CVK_MOUSELEFT);
}

static int onMBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(win, CVK_MOUSEMIDDLE);
}

static int onRBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(win, CVK_MOUSERIGHT);
}

static int onMOUSEWHEEL(HWND win, int x, int y, int z, UINT keys) {
        int which = z >= 0? CVK_MOUSEWHEELUP : CVK_MOUSEWHEELDOWN;
        return wgot(win, CVE_DOWN, which, 0)
                && wgot(win, CVE_UP, which, 0);
}

static int onCHAR(HWND win, unsigned c, int repeats) {
        return wgot(win, CVE_UNICODE, uc2aw(c), 0); 
}

static int onDESTROY(HWND win) {
        chk(KillTimer(win, 1));
        return 1;
}


static LONG WINAPI handle(HWND win, UINT msg, WPARAM w, LPARAM l) {
        LONG r;
        switch (msg) {
#define HANDLE(x) case WM_##x: cvReport("han " #x); r = HANDLE_WM_##x(win, w, l, on##x); break
                HANDLE(TIMER);
                HANDLE(PAINT);
                HANDLE(MOUSEMOVE);
                HANDLE(SIZE);
                HANDLE(KEYDOWN);
                HANDLE(SYSKEYDOWN);
                HANDLE(SYSKEYUP);
                HANDLE(CHAR);
                HANDLE(KEYUP);
                HANDLE(LBUTTONDOWN);
                HANDLE(RBUTTONDOWN);
                HANDLE(MBUTTONDOWN);
                HANDLE(LBUTTONUP);
                HANDLE(RBUTTONUP);
                HANDLE(MBUTTONUP);
                HANDLE(MOUSEWHEEL);
                HANDLE(DESTROY);
                HANDLE(CLOSE);
#undef HANDLE
        case WM_IME_STARTCOMPOSITION: {
                HIMC imc = ImmGetContext(win);
                COMPOSITIONFORM cf;
                cf.dwStyle = CFS_POINT;
                cf.ptCurrentPos.x = cvMouseX();
                cf.ptCurrentPos.y = cvMouseY();
                ImmSetCompositionWindow(imc, &cf);
                ImmReleaseContext(win, imc);
                r = 1;
        }
        break;
        case WM_IME_COMPOSITION: {
                if(l & GCS_RESULTSTR){
                        unsigned short str[4096];
                        unsigned len, i; 
                        HIMC imc = ImmGetContext(win);
                        HDC dc = GetDC(win);
                        len = ImmGetCompositionString(imc, GCS_RESULTSTR, str, sizeof(str));
                        len >>= 1;
                        for (i = 0; i < len; i++)
                                wgot(win, CVE_UNICODE, str[i], 0); 
                        ImmReleaseContext(win, imc);
                        chk(ReleaseDC(win, dc));
                }
                r = 0;
        }
        break;
        default: r = 0;
        }
        if (!r)
                r = DefWindowProcW(win, msg, w, l);
//        assert(!GetLastError());
        return r;
}

static void osglinit(NPWindow * w) {
        HDC dc;
        osterm();
        BOOL (APIENTRY *wglSwapInterval)(int);
        s_win = w->window;
        cvReport("win: %p", s_win);
        s_oldproc = SubclassWindow(s_win, handle);
        cvReport("old: %p", s_oldproc);
        dc = GetDC(s_win);
        cvReport("dc: %p", dc);
        setPF(dc);
        s_rc = wglCreateContext(dc);
        cvReport("ct: %p", s_rc);
        chk(wglMakeCurrent(dc, s_rc));
        wglSwapInterval = (void*)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapInterval)
                chk(wglSwapInterval(1));
        cvInject(CVE_RESIZE, w->width, w->height);
        cvInject(CVE_GLINIT, 0, 0);
        chk(ReleaseDC(s_win, dc));
        chk(SetTimer(s_win, 10, 10, 0));
        chk(PostMessage(s_win, WM_PAINT, 0, 0));
}

static void osinit(NPNetscapeFuncs * browser, NPP i) {
}

static void osterm() {
        if (s_win)
                KillTimer(s_win, 10);

        chk(wglMakeCurrent(0, 0));
        if (s_rc)
                chk(wglDeleteContext(s_rc));
        s_rc = 0;
        if (s_oldproc)
                (void)SubclassWindow(s_win, s_oldproc);
        s_oldproc = 0;
        s_win = 0;
        
}

static NPError osgetval(NPP i, NPPVariable var, void * v) {
        NPError ret;
        debug("os getvalue"); 
        switch(var) {
        default: 
                debug("os getval default"); 
                ret = NPERR_GENERIC_ERROR; 
                break;
        }
        return ret;
} 

static NPError osevent(void * ve) {
        return NPERR_NO_ERROR;
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
