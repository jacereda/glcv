#include "cv.c"
#include "xdg-shell.c"
#include "xdg-shell.h"
#include <EGL/egl.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-egl.h>
#include <xkbcommon/xkbcommon.h>

#ifdef __linux__
#include <linux/input.h>
#endif

static int g_done = 0;
static struct wl_compositor * g_comp;
static struct xdg_wm_base * g_xwb;
static struct xdg_toplevel * g_xdgtl;
static struct xkb_state * g_xkbst;

static void nop() {}

static void button(void * d, struct wl_pointer * p, uint32_t sn, uint32_t t,
                   uint32_t btn, uint32_t st) {
        cveventtype et;
        cvkey k;
        switch (st) {
        case WL_POINTER_BUTTON_STATE_PRESSED:
                et = CVE_DOWN;
                break;
        case WL_POINTER_BUTTON_STATE_RELEASED:
                et = CVE_UP;
                break;
        default:
                et = CVE_NONE;
        }
        switch (btn) {
        case BTN_LEFT:
                k = CVK_MOUSELEFT;
                break;
        case BTN_RIGHT:
                k = CVK_MOUSERIGHT;
                break;
        case BTN_MIDDLE:
                k = CVK_MOUSEMIDDLE;
                break;
        case BTN_GEAR_DOWN:
                k = CVK_MOUSEWHEELDOWN;
                break;
        case BTN_GEAR_UP:
                k = CVK_MOUSEWHEELUP;
                break;
        default:
                k = CVK_NONE;
                break;
        }
        cvInject(et, k, 0);
}

static void motion(void * d, struct wl_pointer * p, uint32_t t, wl_fixed_t x,
                   wl_fixed_t y) {
        cvInject(CVE_MOTION, wl_fixed_to_int(x), wl_fixed_to_int(y));
}

static cvkey mapkeycode(unsigned k) {
        cvkey ret;
        switch (k) {
        case KEY_ESC:
                ret = CVK_ESCAPE;
                break;
        case KEY_1:
                ret = CVK_1;
                break;
        case KEY_2:
                ret = CVK_2;
                break;
        case KEY_3:
                ret = CVK_3;
                break;
        case KEY_4:
                ret = CVK_4;
                break;
        case KEY_5:
                ret = CVK_5;
                break;
        case KEY_6:
                ret = CVK_6;
                break;
        case KEY_7:
                ret = CVK_7;
                break;
        case KEY_8:
                ret = CVK_8;
                break;
        case KEY_9:
                ret = CVK_9;
                break;
        case KEY_0:
                ret = CVK_0;
                break;
        case KEY_MINUS:
                ret = CVK_MINUS;
                break;
        case KEY_EQUAL:
                ret = CVK_EQUAL;
                break;
        case KEY_BACKSPACE:
                ret = CVK_DELETE;
                break;
        case KEY_TAB:
                ret = CVK_TAB;
                break;
        case KEY_Q:
                ret = CVK_Q;
                break;
        case KEY_W:
                ret = CVK_W;
                break;
        case KEY_E:
                ret = CVK_E;
                break;
        case KEY_R:
                ret = CVK_R;
                break;
        case KEY_T:
                ret = CVK_T;
                break;
        case KEY_Y:
                ret = CVK_Y;
                break;
        case KEY_U:
                ret = CVK_U;
                break;
        case KEY_I:
                ret = CVK_I;
                break;
        case KEY_O:
                ret = CVK_O;
                break;
        case KEY_P:
                ret = CVK_P;
                break;
        case KEY_LEFTBRACE:
                ret = CVK_LEFTBRACKET;
                break;
        case KEY_RIGHTBRACE:
                ret = CVK_RIGHTBRACKET;
                break;
        case KEY_ENTER:
                ret = CVK_RETURN;
                break;
        case KEY_LEFTCTRL:
                ret = CVK_CONTROL;
                break;
        case KEY_A:
                ret = CVK_A;
                break;
        case KEY_S:
                ret = CVK_S;
                break;
        case KEY_D:
                ret = CVK_D;
                break;
        case KEY_F:
                ret = CVK_F;
                break;
        case KEY_G:
                ret = CVK_G;
                break;
        case KEY_H:
                ret = CVK_H;
                break;
        case KEY_J:
                ret = CVK_J;
                break;
        case KEY_K:
                ret = CVK_K;
                break;
        case KEY_L:
                ret = CVK_L;
                break;
        case KEY_SEMICOLON:
                ret = CVK_SEMICOLON;
                break;
        case KEY_APOSTROPHE:
                ret = CVK_QUOTE;
                break;
        case KEY_GRAVE:
                ret = CVK_GRAVE;
                break;
        case KEY_LEFTSHIFT:
                ret = CVK_SHIFT;
                break;
        case KEY_BACKSLASH:
                ret = CVK_BACKSLASH;
                break;
        case KEY_Z:
                ret = CVK_Z;
                break;
        case KEY_X:
                ret = CVK_X;
                break;
        case KEY_C:
                ret = CVK_C;
                break;
        case KEY_V:
                ret = CVK_V;
                break;
        case KEY_B:
                ret = CVK_B;
                break;
        case KEY_N:
                ret = CVK_N;
                break;
        case KEY_M:
                ret = CVK_M;
                break;
        case KEY_COMMA:
                ret = CVK_COMMA;
                break;
        case KEY_DOT:
                ret = CVK_PERIOD;
                break;
        case KEY_SLASH:
                ret = CVK_SLASH;
                break;
        case KEY_RIGHTSHIFT:
                ret = CVK_RIGHTSHIFT;
                break;
        case KEY_KPASTERISK:
                ret = CVK_KEYPADMULTIPLY;
                break;
        case KEY_OPTION:
                ret = CVK_OPTION;
                break;
        case KEY_SPACE:
                ret = CVK_SPACE;
                break;
        case KEY_F1:
                ret = CVK_F1;
                break;
        case KEY_F2:
                ret = CVK_F2;
                break;
        case KEY_F3:
                ret = CVK_F3;
                break;
        case KEY_F4:
                ret = CVK_F4;
                break;
        case KEY_F5:
                ret = CVK_F5;
                break;
        case KEY_F6:
                ret = CVK_F6;
                break;
        case KEY_F7:
                ret = CVK_F7;
                break;
        case KEY_F8:
                ret = CVK_F8;
                break;
        case KEY_F9:
                ret = CVK_F9;
                break;
        case KEY_F10:
                ret = CVK_F10;
                break;
        case KEY_KP7:
                ret = CVK_KEYPAD7;
                break;
        case KEY_KP8:
                ret = CVK_KEYPAD8;
                break;
        case KEY_KP9:
                ret = CVK_KEYPAD9;
                break;
        case KEY_KPMINUS:
                ret = CVK_KEYPADMINUS;
                break;
        case KEY_KP4:
                ret = CVK_KEYPAD4;
                break;
        case KEY_KP5:
                ret = CVK_KEYPAD5;
                break;
        case KEY_KP6:
                ret = CVK_KEYPAD6;
                break;
        case KEY_KPPLUS:
                ret = CVK_KEYPADPLUS;
                break;
        case KEY_KP1:
                ret = CVK_KEYPAD1;
                break;
        case KEY_KP2:
                ret = CVK_KEYPAD2;
                break;
        case KEY_KP3:
                ret = CVK_KEYPAD3;
                break;
        case KEY_KP0:
                ret = CVK_KEYPAD0;
                break;
        case KEY_KPDOT:
                ret = CVK_KEYPADDECIMAL;
                break;
        case KEY_KPENTER:
                ret = CVK_KEYPADENTER;
                break;
        case KEY_KPSLASH:
                ret = CVK_KEYPADDIVIDE;
                break;
        case KEY_HOME:
                ret = CVK_HOME;
                break;
        case KEY_LEFT:
                ret = CVK_LEFTARROW;
                break;
        case KEY_RIGHT:
                ret = CVK_RIGHTARROW;
                break;
        case KEY_END:
                ret = CVK_END;
                break;
        case KEY_DOWN:
                ret = CVK_DOWNARROW;
                break;
        case KEY_UP:
                ret = CVK_UPARROW;
                break;
        case KEY_PAGEUP:
                ret = CVK_PAGEUP;
                break;
        case KEY_PAGEDOWN:
                ret = CVK_PAGEDOWN;
                break;
        case KEY_DELETE:
                ret = CVK_FORWARDDELETE;
                break;
        case KEY_LEFTMETA:
                ret = CVK_COMMAND;
                break;
        case KEY_CAPSLOCK:
                ret = CVK_CAPSLOCK;
                break;
        case KEY_RIGHTCTRL:
                ret = CVK_RIGHTCONTROL;
                break;
        case KEY_KPEQUAL:
                ret = CVK_KEYPADEQUALS;
                break;
        case KEY_RIGHTALT:
                ret = CVK_RIGHTOPTION;
                break;
        case KEY_VOLUMEDOWN:
                ret = CVK_VOLUMEDOWN;
                break;
        case KEY_MUTE:
                ret = CVK_MUTE;
                break;
        case KEY_VOLUMEUP:
                ret = CVK_VOLUMEUP;
                break;
        case KEY_F17:
                ret = CVK_F17;
                break;
        case KEY_F18:
                ret = CVK_F18;
                break;
        case KEY_F19:
                ret = CVK_F19;
                break;
        case KEY_F20:
                ret = CVK_F20;
                break;
        case KEY_F11:
                ret = CVK_F11;
                break;
        case KEY_F13:
                ret = CVK_F13;
                break;
        case KEY_F16:
                ret = CVK_F16;
                break;
        case KEY_F14:
                ret = CVK_F14;
                break;
        case KEY_F12:
                ret = CVK_F12;
                break;
        case KEY_F15:
                ret = CVK_F15;
                break;
        case KEY_HELP:
                ret = CVK_HELP;
                break;
        case KEY_FN:
                ret = CVK_FUNCTION;
                break;
        case KEY_CLEAR:
                ret = CVK_KEYPADCLEAR;
                break;
        default:
                ret = CVK_NONE;
        }
        return ret;
}

static void key(void * d, struct wl_keyboard * k, uint32_t sn, uint32_t t,
                uint32_t kc, uint32_t st) {
        cvkey ck = mapkeycode(kc);
        cveventtype et;
        uint32_t u;
        switch (st) {
        case WL_KEYBOARD_KEY_STATE_PRESSED:
                et = CVE_DOWN;
                break;
        case WL_KEYBOARD_KEY_STATE_RELEASED:
                et = CVE_UP;
                break;
        default:
                et = CVE_NONE;
        }
        cvInject(et, ck, 0);
        u = xkb_state_key_get_utf32(
            g_xkbst, st == WL_KEYBOARD_KEY_STATE_PRESSED ? kc + 8 : 0);
        if (u)
                cvInject(CVE_UNICODE, u, 0);
}

static void keymap(void * d, struct wl_keyboard * wk, uint32_t fmt, int32_t fd,
                   uint32_t sz) {
        struct xkb_context * xkbctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        char * mem = mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
        struct xkb_keymap * xkbkm =
            xkb_keymap_new_from_string(xkbctx, mem, XKB_KEYMAP_FORMAT_TEXT_V1,
                                       XKB_KEYMAP_COMPILE_NO_FLAGS);
        xkb_context_unref(xkbctx);
        munmap(mem, sz);
        close(fd);
        assert(!g_xkbst);
        g_xkbst = xkb_state_new(xkbkm);
        xkb_keymap_unref(xkbkm);
}

static void repeat_info(void * d, struct wl_keyboard * wk, int32_t rate,
                        int32_t delay) {
        // TODO
}

static void scapabilities(void * d, struct wl_seat * seat, uint32_t cap) {
        if (cap & WL_SEAT_CAPABILITY_POINTER) {
                struct wl_pointer * p = wl_seat_get_pointer(seat);
                static const struct wl_pointer_listener pl = {
                        .enter = nop,
                        .leave = nop,
                        .motion = motion,
                        .button = button,
                        .axis = nop,
                        .frame = nop,
                        .axis_source = nop,
                        .axis_stop = nop,
                        .axis_discrete = nop,
                };
                wl_pointer_add_listener(p, &pl, seat);
        }
        if (cap & WL_SEAT_CAPABILITY_KEYBOARD) {
                struct wl_keyboard * k = wl_seat_get_keyboard(seat);
                static const struct wl_keyboard_listener kl = {
                        .keymap = keymap,
                        .enter = nop,
                        .leave = nop,
                        .key = key,
                        .modifiers = nop,
                        .repeat_info = repeat_info,
                };
                wl_keyboard_add_listener(k, &kl, seat);
        }
}

static void global(void * d, struct wl_registry * reg, uint32_t nm,
                   const char * iface, uint32_t ver) {
#define IS(x)   0 == strcmp(iface, (x).name)
#define BIND(x) wl_registry_bind(reg, nm, &x, 1)
        if (0)
                ;
        else if (IS(xdg_wm_base_interface))
                g_xwb = BIND(xdg_wm_base_interface);
        else if (IS(wl_compositor_interface))
                g_comp = BIND(wl_compositor_interface);
        else if (IS(wl_seat_interface)) {
                struct wl_seat * seat = BIND(wl_seat_interface);
                static const struct wl_seat_listener sl = {
                        .capabilities = scapabilities,
                };
                wl_seat_add_listener(seat, &sl, NULL);
        }
#undef BIND
#undef IS
}

static void sconfigure(void * d, struct xdg_surface * s, uint32_t sn) {
        xdg_surface_ack_configure(s, sn);
        //    wl_surface_commit(s);
}

static void tlconfigure(void * d, struct xdg_toplevel * xtl, int32_t w,
                        int32_t h, struct wl_array * states) {
        struct wl_egl_window * win = (struct wl_egl_window *)d;
        wl_egl_window_resize(win, w, h, 0, 0);
        cvInject(CVE_RESIZE, w, h);
}

static void tlclose(void * d, struct xdg_toplevel * tl) {
        cvInject(CVE_CLOSE, 0, 0);
}

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case CVE_QUIT:
                g_done = 1;
                break;
        case CVE_SETCURSOR:
                /* setcursor((uint8_t *)evArg0(e), evArg1(e) >> 16, */
                /*           evArg1(e) & 0xffff); */
                break;
        case CVE_DEFAULTCURSOR:
                /* defaultcursor(); */
                break;
        case CVE_WINDOWED:
                xdg_toplevel_unset_fullscreen(g_xdgtl);
                break;
        case CVE_FULLSCREEN:
                xdg_toplevel_set_fullscreen(g_xdgtl, NULL);
                break;
        default:
                ret = 0;
        }
        return ret;
}

int cvrun(int argc, char ** argv) {
        struct wl_display * wldpy = wl_display_connect(NULL);
        struct wl_registry * wlreg = wl_display_get_registry(wldpy);

        static const struct wl_registry_listener wrl = {
                .global = global,
        };
        wl_registry_add_listener(wlreg, &wrl, NULL);
        wl_display_roundtrip(wldpy);

        cvInject(CVE_INIT, argc, (intptr_t)argv);

        struct wl_surface * wlsurf = wl_compositor_create_surface(g_comp);

        struct xdg_surface * xdgsurf =
            xdg_wm_base_get_xdg_surface(g_xwb, wlsurf);
        static const struct xdg_surface_listener xsl = {
                .configure = sconfigure,
        };
        xdg_surface_add_listener(xdgsurf, &xsl, NULL);

        g_xdgtl = xdg_surface_get_toplevel(xdgsurf);
        static const struct xdg_toplevel_listener xtl = {
                .configure = tlconfigure,
                .close = tlclose,
        };

        wl_surface_commit(wlsurf);

        EGLDisplay egldpy = eglGetDisplay((EGLNativeDisplayType)wldpy);
        EGLint major, minor;
        eglInitialize(egldpy, &major, &minor);
        EGLint attrs[] = {
                EGL_SURFACE_TYPE,
                EGL_WINDOW_BIT,
                EGL_RED_SIZE,
                8,
                EGL_GREEN_SIZE,
                8,
                EGL_BLUE_SIZE,
                8,
                EGL_ALPHA_SIZE,
                8,
                EGL_DEPTH_SIZE,
                24,
                /* EGL_RENDERABLE_TYPE, */
                /* EGL_OPENGL_ES2_BIT, */
                EGL_NONE,
        };

        EGLConfig eglcfg[1];
        EGLint n = 0;
        eglChooseConfig(egldpy, attrs, eglcfg, 1, &n);
        EGLint cattr[] = {
                EGL_CONTEXT_CLIENT_VERSION,
                2,
                EGL_NONE,
        };
        EGLContext eglctx =
            eglCreateContext(egldpy, eglcfg[0], EGL_NO_CONTEXT, cattr);

        struct wl_egl_window * eglwin = wl_egl_window_create(
            wlsurf, cvInject(CVQ_WIDTH, 0, 0), cvInject(CVQ_HEIGHT, 0, 0));
        xdg_toplevel_add_listener(g_xdgtl, &xtl, eglwin);

        EGLSurface eglsurf = eglCreateWindowSurface(
            egldpy, eglcfg[0], (EGLNativeWindowType)eglwin, NULL);
        eglMakeCurrent(egldpy, eglsurf, eglsurf, eglctx);

        cvInject(CVE_GLINIT, 0, 0);
        while (wl_display_dispatch(wldpy) != -1 && !g_done) {
                eglSwapBuffers(egldpy, eglsurf);
                cvInject(CVE_UPDATE, 0, 0);
        }
        cvInject(CVE_GLTERM, 0, 0);

        xdg_toplevel_destroy(g_xdgtl);
        xdg_surface_destroy(xdgsurf);
        wl_surface_destroy(wlsurf);

        return cvInject(CVE_TERM, 0, 0);
}
