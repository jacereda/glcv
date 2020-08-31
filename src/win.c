#include "cv.c"
#include <stdlib.h>
#include <windows.h>
#include <wingdi.h>
#include <windowsx.h>
#include <dwmapi.h>


#if !defined HANDLE_WM_MOUSEWHEEL
#define HANDLE_WM_MOUSEWHEEL(hwnd,wParam,lParam,fn) ((fn)((hwnd),(int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam),(int)(short)HIWORD(wParam),(UINT)(short)LOWORD(wParam)),0L)
#endif

#define dbg cvReport
#define err cvReport

#define WINDOWED_STYLE WS_CLIPSIBLINGS | WS_CLIPCHILDREN \
                 | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME
#define FULLSCREEN_STYLE WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP

static int g_done = 0;
static HWND g_win;
static HCURSOR g_cursor = 0;
static void delcursor() {
        if (g_cursor) 
                DestroyIcon(g_cursor);
        g_cursor = 0;
}

static void setcursor(uint8_t * rgba, int16_t hotx, int16_t hoty) {
        HDC dc;
        HBITMAP bm;
        ICONINFO ii;
        BITMAPV5HEADER bh;
        int i;
        uint8_t * bits;
        ZeroMemory(&bh, sizeof(bh));
        bh.bV5Size = sizeof(bh);
        bh.bV5Width = 32;
        bh.bV5Height = -32;
        bh.bV5Planes = 1;
        bh.bV5BitCount = 32;
        bh.bV5Compression = BI_RGB;
        dc = GetDC(g_win);
        bm = CreateDIBSection(dc, (BITMAPINFO*)&bh, DIB_RGB_COLORS, (void **)&bits, 0, 0);
        ReleaseDC(g_win, dc);
        for (i = 0; i < 32*32; i++) {
                bits[4*i+0] = rgba[4*i+2];
                bits[4*i+1] = rgba[4*i+1];
                bits[4*i+2] = rgba[4*i+0];
                bits[4*i+3] = rgba[4*i+3];
        }
        if (bits[3] == 0)
                bits[3] = 1; // workaround for vbox
        ii.fIcon = FALSE;
        ii.xHotspot = hotx;
        ii.yHotspot = hoty;
        ii.hbmColor = bm;
        ii.hbmMask = CreateBitmap(32, 32, 1, 1, 0); 
        g_cursor = CreateIconIndirect(&ii);
        SetCursor(g_cursor);
        DeleteObject(bm);
        DeleteObject(ii.hbmMask);
}

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case CVE_QUIT: g_done = 1; break;               
        case CVE_SETCURSOR: 
                delcursor();
                setcursor((uint8_t*)evArg0(e), 
                          evArg1(e) >> 16, evArg1(e) & 0xffff); 
                break;
        case CVE_DEFAULTCURSOR: 
                delcursor();
                SetCursor(LoadCursor(0, IDC_ARROW));
                break;
        case CVE_FULLSCREEN: 
                SetWindowLong(g_win, GWL_STYLE, FULLSCREEN_STYLE);
                ShowWindow(g_win, SW_SHOWMAXIMIZED);
                SetWindowPos(g_win, HWND_TOPMOST, 0, 0, 0, 0,
                             SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);
                break;
        case CVE_WINDOWED:
                ShowWindow(g_win, SW_SHOWNORMAL);
                SetWindowLong(g_win, GWL_STYLE, WINDOWED_STYLE);
                SetWindowPos(g_win, HWND_TOPMOST, 0, 0, 0, 0,
                             SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_SHOWWINDOW);
                break;
        default: ret = 0;
        }
        return ret;
}

#if !defined MAPVK_VK_TO_CHAR
#define MAPVK_VK_TO_CHAR 2
#endif

#if !defined VK_OEM_PLUS
#define VK_OEM_PLUS 0xbb
#endif

#if !defined VK_OEM_COMMA
#define VK_OEM_COMMA 0xbc
#endif

#if !defined VK_OEM_MINUS
#define VK_OEM_MINUS 0xbd
#endif

#if !defined VK_OEM_PERIOD
#define VK_OEM_PERIOD 0xbe
#endif

#if !defined VK_VOLUME_MUTE
#define VK_VOLUME_MUTE 0xad
#endif

#if !defined VK_VOLUME_DOWN
#define VK_VOLUME_DOWN 0xae
#endif

#if !defined VK_VOLUME_UP
#define VK_VOLUME_UP 0xaf
#endif

static cvkey mapkey(int vk) {
        cvkey ret;
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

static int onSYSKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        cvInject(CVE_DOWN, mapkey(vk), 0);
        return 0;
}

static int onSYSKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        cvInject(CVE_UP, mapkey(vk), 0);
        return 0;
}

static int onKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        cvInject(CVE_DOWN, mapkey(vk), 0);
        return 0;
}

static int onKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flacv) {
        cvInject(CVE_UP, mapkey(vk), 0);
        return 0;
}

static int mouseDown(int which) {
        cvInject(CVE_DOWN, which, 0);        
        return 1;
}

static int mouseUp(int which) {
        cvInject(CVE_UP, which, 0);        
        return 1;
}

static int onLBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(CVK_MOUSELEFT);
}

static int onMBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(CVK_MOUSEMIDDLE);
}

static int onRBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flacv) {
        return mouseDown(CVK_MOUSERIGHT);
}

static int onLBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(CVK_MOUSELEFT);
}

static int onMBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(CVK_MOUSEMIDDLE);
}

static int onRBUTTONUP(HWND win, int x, int y, UINT flacv) {
        return mouseUp(CVK_MOUSERIGHT);
}


static int onMOUSEMOVE(HWND win, int x, int y, UINT flacv ) {
        cvInject(CVE_MOTION, x, y);
        return 1;
}

static int onSIZE(HWND win, UINT state, int w, int h) {
        cvInject(CVE_RESIZE, w, h);
        return 1;
}

static int onCLOSE(HWND win){
        cvInject(CVE_CLOSE, 0, 0);
        return 0;
}

static int onSETCURSOR(HWND win, HWND wcur, UINT ht, UINT msg) {
        if (g_cursor)
                SetCursor(g_cursor);
        return g_cursor != 0;
}

static unsigned uc2cv(unsigned uc) {
        unsigned ret = uc;
        switch (uc) {
        case 0xd: ret = '\n'; break;
        }
        return ret;
}

static int onCHAR(HWND win, unsigned c, int repeats) {
        cvInject(CVE_UNICODE, uc2cv(c), 0); 
        return 0;
}

static int onPAINT(HWND win) {
        HDC dc;
        cvInject(CVE_UPDATE, 0, 0);
        dc = GetDC(win);
        SwapBuffers(dc);
        ReleaseDC(win, dc);
        return 1;
}

static int onMOUSEWHEEL(HWND win, int x, int y, int z, UINT keys) {
        int which = z >= 0? CVK_MOUSEWHEELUP : CVK_MOUSEWHEELDOWN;
        cvInject(CVE_DOWN, which, 0);
        cvInject(CVE_UP, which, 0);
        return 1;
}

static LRESULT WINAPI handle(HWND win, UINT msg, WPARAM w, LPARAM l)  {
        LRESULT r;
        switch (msg) {
#define HANDLE(x) case WM_##x: r = HANDLE_WM_##x(win, w, l, on##x); break
                HANDLE(MOUSEMOVE);
                HANDLE(SIZE);
                HANDLE(CLOSE);
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
                HANDLE(SETCURSOR);
                HANDLE(PAINT);
#undef HANDLE
        default: r = 0;
        }
        if (!r)
                r = DefWindowProcW(win, msg, w, l);
        return r;
}

int cvrun(int argc, char ** argv) {
        HANDLE mod = GetModuleHandle(0);
        WNDCLASSW  wc;
        HGLRC ctx;
        HWND win;
        HDC dc;
        WCHAR name[256];
        RECT r;
        PIXELFORMATDESCRIPTOR pfd = { 
                sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
                1,                     // version number 
                PFD_DRAW_TO_WINDOW |   // support window 
                PFD_SUPPORT_OPENGL |   // support OpenGL
                PFD_DOUBLEBUFFER,      // double buffered 
                PFD_TYPE_RGBA,         // RGBA type 
                32,                    // 32-bit color depth 
                0, 0, 0, 0, 0, 0,      // color bits ignored 
                8,                     // 8-bit alpha buffer 
                0,                     // shift bit ignored 
                0,                     // no accumulation buffer 
                0, 0, 0, 0,            // accum bits ignored 
                24,                    // 24-bit z-buffer 
                0,                     // no stencil buffer 
                0,                     // not auxiliary buffer 
                PFD_MAIN_PLANE,        // main layer 
                0,                     // reserved 
                0, 0, 0                // layer masks ignored 
        };
        DWORD exstyle = WS_EX_APPWINDOW;
        DWORD style = WINDOWED_STYLE;
        MultiByteToWideChar(CP_UTF8, 0, 
                            (const char *)cvInject(CVQ_NAME, 0, 0),
                            -1, name, sizeof(name));

        ZeroMemory(&wc, sizeof(wc));
        wc.style += CS_OWNDC;
        wc.lpfnWndProc = handle;
        wc.hInstance = mod;
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.lpszClassName = name;
        RegisterClassW(&wc);
        cvInject(CVE_INIT, 1, (intptr_t)argv);
        r.left = cvInject(CVQ_XPOS, 0, 0);
        r.top = cvInject(CVQ_YPOS, 0, 0);
        r.right = r.left + cvInject(CVQ_WIDTH, 0, 0);
        r.bottom = r.top + cvInject(CVQ_HEIGHT, 0, 0);
        AdjustWindowRect(&r, style, FALSE);
        win = CreateWindowExW(exstyle, name, name, style, 
                              r.left, r.top,
                              r.right - r.left, r.bottom - r.top,
                              0, 0, mod, 0);
        DWM_BLURBEHIND bb = {0};
        HRGN blur = CreateRectRgn(0,0,-1,-1);
        bb.dwFlags = DWM_BB_ENABLE|DWM_BB_BLURREGION;
        bb.fEnable = TRUE;
        bb.hRgnBlur = blur; 
        DwmEnableBlurBehindWindow(win, &bb);
        DeleteObject(blur);
        g_win = win;
        dc = GetDC(win);
        SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
        ctx = wglCreateContext(dc);
        wglMakeCurrent(dc, ctx);
        cvInject(CVE_GLINIT, 0, 0);
        ((int(APIENTRY*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1);
        ShowWindow(win, SW_SHOWNORMAL);
        while (!g_done) {
                MSG msg;
                if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                }
                InvalidateRect(win, 0, 0);
                UpdateWindow(win);
        }
        cvInject(CVE_GLTERM, 0, 0);
        ReleaseDC(win, dc);
        return cvInject(CVE_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
