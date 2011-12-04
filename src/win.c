#include "gs.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wingdi.h>
#include <windowsx.h>


#define dbg gsReport
#define err gsReport

intptr_t osEvent(ev * e) {
	intptr_t ret;
	switch (evType(e)) {
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

static gskey mapkey(int vk) {
	gskey ret;
        switch (vk) {
        case 'A': ret = GSK_A; break;
        case 'S': ret = GSK_S; break;
        case 'D': ret = GSK_D; break;
        case 'F': ret = GSK_F; break;
        case 'H': ret = GSK_H; break;
        case 'G': ret = GSK_G; break;
        case 'Z': ret = GSK_Z; break;
        case 'X': ret = GSK_X; break;
        case 'C': ret = GSK_C; break;
        case 'V': ret = GSK_V; break;
        case 'B': ret = GSK_B; break;
        case 'Q': ret = GSK_Q; break;
        case 'W': ret = GSK_W; break;
        case 'E': ret = GSK_E; break;
        case 'R': ret = GSK_R; break;
        case 'Y': ret = GSK_Y; break;
        case 'T': ret = GSK_T; break;
        case '1': ret = GSK_1; break;
        case '2': ret = GSK_2; break;
        case '3': ret = GSK_3; break;
        case '4': ret = GSK_4; break;
        case '6': ret = GSK_6; break;
        case '5': ret = GSK_5; break;
        case VK_OEM_PLUS: ret = GSK_EQUAL; break;
        case '9': ret = GSK_9; break;
        case '7': ret = GSK_7; break;
        case VK_OEM_MINUS: ret = GSK_MINUS; break;
        case '8': ret = GSK_8; break;
        case '0': ret = GSK_0; break;
        case VK_OEM_6: ret = GSK_RIGHTBRACKET; break;
        case 'O': ret = GSK_O; break;
        case 'U': ret = GSK_U; break;
        case VK_OEM_4: ret = GSK_LEFTBRACKET; break;
        case 'I': ret = GSK_I; break;
        case 'P': ret = GSK_P; break;
        case 'L': ret = GSK_L; break;
        case 'J': ret = GSK_J; break;
        case VK_OEM_7: ret = GSK_QUOTE; break;
        case 'K': ret = GSK_K; break;
        case VK_OEM_1: ret = GSK_SEMICOLON; break;
        case VK_OEM_5: ret = GSK_BACKSLASH; break;
        case VK_OEM_COMMA: ret = GSK_COMMA; break;
        case VK_OEM_2: ret = GSK_SLASH; break;
        case 'N': ret = GSK_N; break;
        case 'M': ret = GSK_M; break;
        case VK_OEM_PERIOD: ret = GSK_PERIOD; break;
        case VK_OEM_3: ret = GSK_GRAVE; break;
        case VK_DECIMAL: ret = GSK_KEYPADDECIMAL; break;
        case VK_MULTIPLY: ret = GSK_KEYPADMULTIPLY; break;
        case VK_ADD: ret = GSK_KEYPADPLUS; break;
//	case 'K'eypadClear: ret = GSK_KEYPADCLEAR; break;
        case VK_DIVIDE: ret = GSK_KEYPADDIVIDE; break;
//	case 'K'eypadEnter: ret = GSK_KEYPADENTER; break;
        case VK_SUBTRACT: ret = GSK_KEYPADMINUS; break;
//	case 'K'eypadEquals: ret = GSK_KEYPADEQUALS; break;
        case VK_NUMPAD0: ret = GSK_KEYPAD0; break;
        case VK_NUMPAD1: ret = GSK_KEYPAD1; break;
        case VK_NUMPAD2: ret = GSK_KEYPAD2; break;
        case VK_NUMPAD3: ret = GSK_KEYPAD3; break;
        case VK_NUMPAD4: ret = GSK_KEYPAD4; break;
        case VK_NUMPAD5: ret = GSK_KEYPAD5; break;
        case VK_NUMPAD6: ret = GSK_KEYPAD6; break;
        case VK_NUMPAD7: ret = GSK_KEYPAD7; break;
        case VK_NUMPAD8: ret = GSK_KEYPAD8; break;
        case VK_NUMPAD9: ret = GSK_KEYPAD9; break;
        case VK_RETURN: ret = GSK_RETURN; break;
        case VK_TAB: ret = GSK_TAB; break;
        case VK_SPACE: ret = GSK_SPACE; break;
        case VK_BACK: ret = GSK_DELETE; break;
        case VK_ESCAPE: ret = GSK_ESCAPE; break;
        case VK_LMENU:
        case VK_MENU: ret = GSK_OPTION; break;
        case VK_LSHIFT:
        case VK_SHIFT: ret = GSK_SHIFT; break;
        case VK_LWIN:
        case VK_RWIN: ret = GSK_COMMAND; break;
        case VK_CAPITAL: ret = GSK_CAPSLOCK; break;
        case VK_CONTROL: ret = GSK_CONTROL; break;
        case VK_RSHIFT: ret = GSK_RIGHTSHIFT; break;
        case VK_RMENU: ret = GSK_RIGHTOPTION; break;
        case VK_RCONTROL: ret = GSK_RIGHTCONTROL; break;
        case VK_F17: ret = GSK_F17; break;
        case VK_VOLUME_UP: ret = GSK_VOLUMEUP; break;
        case VK_VOLUME_DOWN: ret = GSK_VOLUMEDOWN; break;
        case VK_VOLUME_MUTE: ret = GSK_MUTE; break;
        case VK_F18: ret = GSK_F18; break;
        case VK_F19: ret = GSK_F19; break;
        case VK_F20: ret = GSK_F20; break;
        case VK_F5: ret = GSK_F5; break;
        case VK_F6: ret = GSK_F6; break;
        case VK_F7: ret = GSK_F7; break;
        case VK_F3: ret = GSK_F3; break;
        case VK_F8: ret = GSK_F8; break;
        case VK_F9: ret = GSK_F9; break;
        case VK_F11: ret = GSK_F11; break;
        case VK_F13: ret = GSK_F13; break;
        case VK_F16: ret = GSK_F16; break;
        case VK_F14: ret = GSK_F14; break;
        case VK_F10: ret = GSK_F10; break;
        case VK_F12: ret = GSK_F12; break;
        case VK_F15: ret = GSK_F15; break;
        case VK_HELP: ret = GSK_HELP; break;
        case VK_HOME: ret = GSK_HOME; break;
        case VK_PRIOR: ret = GSK_PAGEUP; break;
        case VK_INSERT: ret = GSK_FUNCTION; break;
        case VK_DELETE: ret = GSK_FORWARDDELETE; break;
        case VK_F4: ret = GSK_F4; break;
        case VK_END: ret = GSK_END; break;
        case VK_F2: ret = GSK_F2; break;
        case VK_NEXT: ret = GSK_PAGEDOWN; break;
        case VK_F1: ret = GSK_F1; break;
        case VK_LEFT: ret = GSK_LEFTARROW; break;
        case VK_RIGHT: ret = GSK_RIGHTARROW; break;
        case VK_DOWN: ret = GSK_DOWNARROW; break;
        case VK_UP: ret = GSK_UPARROW; break;
        case VK_SCROLL: ret = GSK_SCROLL; break;
        case VK_NUMLOCK: ret = GSK_NUMLOCK; break;
        case VK_CLEAR: ret = GSK_CLEAR; break;
        case VK_SNAPSHOT: ret = GSK_SYSREQ; break;
        case VK_PAUSE: ret = GSK_PAUSE; break;
        default: ret = GSK_NONE; break;
        }
        return ret;
}

static int onSYSKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flags) {
	gsInject(GSC_DOWN, mapkey(vk), 0);
	return 0;
}

static int onSYSKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flags) {
        gsInject(GSC_UP, mapkey(vk), 0);
	return 0;
}

static int onKEYDOWN(HWND win, UINT vk, BOOL down, int repeats, UINT flags) {
	gsInject(GSC_DOWN, mapkey(vk), 0);
        return 0;
}

static int onKEYUP(HWND win, UINT vk, BOOL down, int repeats, UINT flags) {
	gsInject(GSC_UP, mapkey(vk), 0);
	return 0;
}

static int mouseDown(int which) {
        gsInject(GSC_DOWN, which, 0);        
	return 1;
}

static int mouseUp(int which) {
        gsInject(GSC_UP, which, 0);        
	return 1;
}

static int onLBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flags) {
        return mouseDown(GSK_MOUSELEFT);
}

static int onMBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flags) {
        return mouseDown(GSK_MOUSEMIDDLE);
}

static int onRBUTTONDOWN(HWND win, BOOL dbl, int x, int y, UINT flags) {
        return mouseDown(GSK_MOUSERIGHT);
}

static int onLBUTTONUP(HWND win, int x, int y, UINT flags) {
        return mouseUp(GSK_MOUSELEFT);
}

static int onMBUTTONUP(HWND win, int x, int y, UINT flags) {
        return mouseUp(GSK_MOUSEMIDDLE);
}

static int onRBUTTONUP(HWND win, int x, int y, UINT flags) {
        return mouseUp(GSK_MOUSERIGHT);
}


static int onMOUSEMOVE(HWND win, int x, int y, UINT flags ) {
        gsInject(GSC_MOTION, x, y);
	return 1;
}

static int onSIZE(HWND win, UINT state, int w, int h) {
	gsInject(GSC_RESIZE, w, h);
	gsInject(GSC_GLINIT, 0, 0);
        return 1;
}

static int g_done = 0;

static int onCLOSE(HWND win){
        gsInject(GSC_CLOSE, 0, 0);
	g_done = 1;
	return 0;
}

static unsigned uc2gs(unsigned uc) {
        unsigned ret = uc;
        switch (uc) {
        case 0xd: ret = '\n'; break;
        }
        return ret;
}

static int onCHAR(HWND win, unsigned c, int repeats) {
        gsInject(GSC_UNICODE, uc2gs(c), 0); 
	return 0;
}

static int onPAINT(HWND win) {
	HDC dc;
	gsInject(GSC_UPDATE, 0, 0);
	dc = GetDC(win);
	SwapBuffers(dc);
	ReleaseDC(win, dc);
	return 1;
}

static int onMOUSEWHEEL(HWND win, int x, int y, int z, UINT keys) {
        int which = z >= 0? GSK_MOUSEWHEELUP : GSK_MOUSEWHEELDOWN;
        gsInject(GSC_DOWN, which, 0);
	gsInject(GSC_UP, which, 0);
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
                HANDLE(PAINT);
#undef HANDLE
        default: r = 0;
        }
        if (!r)
                r = DefWindowProcW(win, msg, w, l);
        return r;
}

int main(int argc, char ** argv) {
	HANDLE mod = GetModuleHandle(0);
        WNDCLASSW  wc;
	RECT r;
        HGLRC ctx;
	HWND win;
	HDC dc;
	WCHAR name[256];
	int done = 0;
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
	
        MultiByteToWideChar(CP_UTF8, 0, 
			    (const char *)gsInject(GSQ_NAME, 0, 0),
			    -1, name, sizeof(name));

        ZeroMemory(&wc, sizeof(wc));
	wc.style += CS_OWNDC;
        wc.lpfnWndProc = handle;
        wc.hInstance = mod;
        wc.lpszClassName = name;
        RegisterClassW(&wc);
	GetWindowRect(GetDesktopWindow(), &r);
        win = CreateWindowExW(0, name, name, WS_POPUP, 0, 0,
                              r.right, r.bottom,
                              0, 0, mod, 0);
	dc = GetDC(win);
        SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd); 
	ctx = wglCreateContext(dc);
	gsInject(GSC_INIT, 1, (intptr_t)argv);
        wglMakeCurrent(dc, ctx);	
	((int(*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1);
        ShowWindow(win, SW_MAXIMIZE);
        while (!g_done) {
                MSG msg;
                if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
                        TranslateMessage(&msg);
                        DispatchMessageW(&msg);
                }
		InvalidateRect(win, 0, 0);
		UpdateWindow(win);
        }
        ReleaseDC(win, dc);
	return gsInject(GSC_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   End: **
*/
