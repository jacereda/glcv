#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "gs.h"
#include "gsgl.h"

#if defined __APPLE__
#define NP_NO_CARBON
#define evType evType2
#include <WebKit/npapi.h>
#include <WebKit/npfunctions.h>
#undef evType
#define OSCALL 
#else
#if defined _WIN32
typedef int bool;
#endif
#include "npapi/npapi.h"
#include "npapi/npfunctions.h"
#endif

#if defined(__GNUC__) && !defined(_WIN32_)
#define EXPORTED \
	extern __attribute__((visibility("default"))) __attribute__((used))
#else
#define EXPORTED extern __declspec(dllexport)
#endif

typedef void (*method)(const char *);

static NPNetscapeFuncs * s_browser = 0;
static NPObject * s_so = 0;
static NPP s_ins;
static char s_plgname[256];

static void osinit(NPNetscapeFuncs * browser, NPP i);
static void osterm();
static void osglinit(NPWindow *);
static NPError osevent(void * ve);
static NPError osgetval(NPP i, NPPVariable var, void * v);

void gsReportV(const char *fmt, va_list ap) {
#if defined(_WIN32)
	FILE *out = fopen("\\gs.log", "a");
#else
	FILE *out = fopen("/tmp/gs.log", "a");
#endif
	if(out) {
		vfprintf(out, fmt, ap);
		fputs("\n", out);
		fclose(out);
	}
}

#define debug gsReport

static bool hasmethod(NPObject* obj, NPIdentifier methodid) {
	return 1;
}

static bool invoke(NPObject* obj, NPIdentifier methodid, 
		   const NPVariant *args, uint32_t nargs, NPVariant *res) {
        debug("invoke");
        int ret;
	if (nargs == 1 && NPVARIANT_IS_STRING(args[0])) {
                NPString s = NPVARIANT_TO_STRING(args[0]);
                char buf[1024];
                debug("len: %d", s.UTF8Length);
                memcpy(buf, s.UTF8Characters, s.UTF8Length);
                buf[s.UTF8Length] = 0;
                gsInject(GSC_INVOKE, (intptr_t)buf, 0);
                ret = 1;
        } else {
		debug("no such method");
		s_browser->setexception(obj, "no such method");
                ret = 0;
	}
	return ret;
}

static bool invokedefault(NPObject *obj, 
			  const NPVariant *args, uint32_t nargs, 
			  NPVariant *res) {
	debug("invokedefault");
	return 0;
}

static bool hasproperty(NPObject *obj, NPIdentifier prop) {
	debug("hasproperty");
	return 0;
}

static bool getproperty(NPObject *obj, NPIdentifier prop, NPVariant *res) {
	debug("getproperty");
	return 0;
}

static bool construct (NPObject *obj, const NPVariant *args, uint32_t argc, NPVariant *res) {
        debug("construct");
        return 0;
}

static NPClass scriptable = {
	NP_CLASS_STRUCT_VERSION,
	0, // allocate
	0, // deallocate
	0, // invalidate
	hasmethod, 
	invoke,
	invokedefault,
	hasproperty,
	getproperty,
	0, // setproperty
	0, // removeproperty
        0, // enumerate
        construct, // construct
};


static NPError nnew(NPMIMEType type, NPP i,
		    uint16_t mode, int16_t argc, char* argn[],
		    char* argv[], NPSavedData* saved) {
        int ok;
        debug("nnew");
        ok = gsInject(GSC_INIT, argc, (intptr_t)argv);
        if (ok) {
                unsigned j;
                snprintf(s_plgname, sizeof s_plgname - 1, "%s", argv[0]);
                debug("new");
                for (j = 0; j < argc; j++)
                        debug("%s -  %s", argn[j], argv[j]);
                s_ins = i;
                osinit(s_browser, i);
        }
	return ok? NPERR_NO_ERROR : NPERR_GENERIC_ERROR;
}

static NPError setwindow(NPP i, NPWindow* w) {
        debug("setwindow");
        osglinit(w);
	return NPERR_NO_ERROR;
}


static NPError destroy(NPP i, NPSavedData **save) {
	debug("destroy");
	gsInject(GSC_CLOSE, 0, 0);
        gsInject(GSC_TERM, 0, 0);
        osterm();
	if(s_so)
		s_browser->releaseobject(s_so);
	return NPERR_NO_ERROR;
}

static NPError getvalue(NPP i, NPPVariable variable, void *v) {
	NPError ret = NPERR_NO_ERROR;
	debug("getvalue");
	switch(variable) {
	default: 
		debug("getval default"); 
		ret = osgetval(i, variable, v);
		break;
	case NPPVpluginNameString: 
		debug("getval pluginname"); 
		*(char **)v = "plugin name";
		break;
	case NPPVpluginDescriptionString:
		debug("getval plugindesc"); 
		*(char **)v = "plugin description";
		break;
	case NPPVpluginScriptableNPObject:
		debug("getval scriptable");
		if(!s_so)
			s_so = s_browser->createobject(i, &scriptable);
		s_browser->retainobject(s_so);
		*(void**)v = s_so;
		break;
	case NPPVpluginWindowBool:
		*(int*)v = 1;
		break;
	}
	return ret;
}


static NPError hevent(NPP i, void * ve) {
	return osevent(ve);
}

EXPORTED NPError OSCALL NP_GetEntryPoints(NPPluginFuncs* f) {
	debug("getentrypoints %p", f);
	f->size = sizeof (NPPluginFuncs);
	f->version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	f->newp = nnew;
	f->setwindow = setwindow;
	f->destroy = destroy;
	f->getvalue = getvalue;
	f->event = hevent;
	debug("/getentrypoints %p", f);
	return NPERR_NO_ERROR;
}

EXPORTED NPError OSCALL NP_Initialize(NPNetscapeFuncs* f
#if defined XP_UNIX && !defined XP_MACOSX
                                      ,NPPluginFuncs * funcs
#endif
        ) {
	debug("initialize");
	s_browser = f;
	if(!f) {
                debug("invalid functable");
		return NPERR_INVALID_FUNCTABLE_ERROR;
        }
	if(((f->version & 0xff00) >> 8) > NP_VERSION_MAJOR) {
                debug("incompatible");
                return NPERR_INCOMPATIBLE_VERSION_ERROR;
        }
#if defined XP_UNIX && !defined XP_MACOSX
        NP_GetEntryPoints(funcs);
#endif
	return NPERR_NO_ERROR;
}

EXPORTED NPError OSCALL NP_Shutdown() {
	debug("shutdown");
	return NPERR_NO_ERROR;
}

EXPORTED char * NP_GetMIMEDescription(void) {
        static char buf[256];
        const char * name;
        debug("getmime");
        name = (const char *)gsInject(GSQ_NAME, 0, 0);
        if (!name)
                name = "unknown";
        snprintf(buf, sizeof(buf), 
                 "application/%s::xx@foo.bar", name);
        buf[sizeof(buf) - 1] = 0;
        debug("getmimedesc %s", buf);
        return buf;
}

EXPORTED NPError OSCALL NP_GetValue(
#if defined(XP_UNIX)
        void *
#else
        NPP
#endif
        npp, 
                                    NPPVariable variable, void *val) {
	debug("npgetvalue");
	return getvalue(npp, variable, val);
}

int gsShowKeyboard() {
        return 1;
}

int gsHideKeyboard() {
        return 1;
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
