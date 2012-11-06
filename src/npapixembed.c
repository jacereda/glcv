#include "npapi.c"
#include <stdlib.h>
#include <X11/X.h>
#include <GL/glx.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

static GtkWidget * g_plug = 0;
static Display g_dpy = 0;
static GLXContext * g_ctx = 0;
static gluint g_to = 0;
static Window g_w;

static XVisualInfo * chooseVisual(Display * dpy, int screen) {
        int att[64];
        int * p = att;
        *p++ = GLX_RGBA;
        *p++ = GLX_DOUBLEBUFFER;
        *p++ = GLX_RED_SIZE; *p++ = 1;
        *p++ = GLX_GREEN_SIZE; *p++ = 1;
        *p++ = GLX_BLUE_SIZE; *p++ = 1;
        *p++ = GLX_DEPTH_SIZE; *p++ = 1;
        *p++ = None;
        return glXChooseVisual(dpy, screen, att);
}

static unsigned mapbutton(unsigned b) {
        unsigned ret;
        switch (b) {
        default: ret = GSK_NONE; break;
        case 1: ret = GSK_MOUSELEFT; break;
        case 2: ret = GSK_MOUSEMIDDLE; break;
        case 3: ret = GSK_MOUSERIGHT; break;
        }
        return ret;
}

static gboolean event(GtkWidget * wid, GdkEvent * ev, gpointer data) {
        extern unsigned mapkeycode(unsigned);
        ins * o = (ins*)data;
        int ret = TRUE;
        switch (ev->type) {
        case GDK_KEY_PRESS: {
                GdkEventKey * kev = (GdkEventKey*)ev;
                cvInject(CVE_DOWN, mapkeycode(kev->hardware_keycode), 0);
                break;
        }
        case GDK_KEY_RELEASE: {
                GdkEventKey * kev = (GdkEventKey*)ev;
                cvInject(CVE_UP, mapkeycode(kev->hardware_keycode), 0);
                break;
        }
        case GDK_BUTTON_PRESS: {
                GdkEventButton * bev = (GdkEventButton*)ev;
                gtk_widget_grab_focus(g_plug);
                got(w, CVE_DOWN, mapbutton(bev->button), 0);
                break;
        }
        case GDK_BUTTON_RELEASE: {
                GdkEventButton * bev = (GdkEventButton*)ev;
                got(w, CVE_UP, mapbutton(bev->button), 0);
                break;
        }
        case GDK_MOTION_NOTIFY: {
                GdkEventMotion * mev = (GdkEventMotion*)ev;
                got(w, CVE_MOTION, mev->x, mev->y);
                break;
        }
        default: ret = FALSE; break;
        }
        return ret;
}

static gboolean update(gpointer data) {
        cvInject(CVE_UPDATE);
        glXSwapBuffers(g_dpy, g_w);
        return 1;
}

static void osglinit(NPWindow * w) {
        NPSetWindowCallbackStruct * info = 
                (NPSetWindowCallbackStruct*)win->ws_info;
        XVisualInfo * vinfo;
        osterm();
        g_plug = gtk_plug_new((GdkNativeWindow)(win->window));
        GTK_WIDGET_SET_FLACV(GTK_WIDGET(g_plug), GTK_CAN_FOCUS);
        gtk_widget_add_events(g_plug, 0
                              | GDK_BUTTON_PRESS_MASK 
                              | GDK_BUTTON_RELEASE_MASK
                              | GDK_KEY_PRESS_MASK
                              | GDK_KEY_RELEASE_MASK
                              | GDK_POINTER_MOTION_MASK
                );
        g_signal_connect(G_OBJECT(g_plug), "event", 
                         G_CALLBACK(event), o);
        gtk_widget_show_all(g_plug);
        gtk_widget_grab_focus(g_plug);
        g_dpy = info->display;
        g_w = gtk_plug_get_id(GTK_PLUG(g_plug));
        g_to = g_timeout_add(10, update, o);
        vinfo = chooseVisual(g_dpy, 0);
        g_ctx = glXCreateContext(g_dpy, vinfo, 0, True);
        XFree(vinfo);
        glXMakeCurrent(g_dpy, g_w, g_ctx);
        cvInject(CVE_RESIZE, w->width, w->height);
        cvInject(CVE_GLINIT, 0, 0);
}

void osterm(ins * o) {
        if (g_to)
                g_source_remove(g_to);
        g_to = 0;
        if (g_dpy)
                glXMakeCurrent(g_dpy, 0, 0);
        g_dpy = 0;
        if (g_ctx)
                glXDestroyContext(g_ctx, 0);
        g_ctx = 0;
        if (g_plug)
                gtk_widget_destroy(g_plug);
        g_plug = 0;
}

static NPError getvalue(NPP i, NPPVariable var, void * v) {
        NPError ret = NPERR_NO_ERROR;
        debug("os getvalue"); 
        switch(var) {
        case NPPVpluginNeedsXEmbed:
                *(int*)v = 1;
                break;
        default: 
                debug("os getval default"); 
                ret = NPERR_GENERIC_ERROR; 
                break;
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

