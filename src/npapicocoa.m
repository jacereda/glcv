#include "npapi.c"
#include <dlfcn.h>
#include <QuartzCore/QuartzCore.h>
#include <Carbon/Carbon.h> // For keycodes

@interface Layer : CAOpenGLLayer {
}

- (void)drawInCGLContext:(CGLContextObj)ct
	     pixelFormat:(CGLPixelFormatObj)pf
	    forLayerTime:(CFTimeInterval)lt
	     displayTime:(const CVTimeStamp *)dt;
@end

static Layer * s_l = 0;

static void osinit(NPNetscapeFuncs * browser, NPP i) {
        browser->setvalue(i, NPPVpluginEventModel, 
			  (void *)NPEventModelCocoa);
	browser->setvalue(i, NPNVpluginDrawingModel, 
			  (void *)NPDrawingModelCoreAnimation);
	s_l = [[Layer layer] retain];	
	s_l.asynchronous = NO;
}

static void osglinit() {
	s_l.asynchronous = YES;
	[s_l setNeedsDisplay];
}

static void osterm() {
	[s_l release];
        s_l = 0;
}

static unsigned mapkeycode(unsigned k) {
	unsigned ret;
	switch (k) {
	case kVK_ANSI_A: ret = GS_KEY_A; break;
	case kVK_ANSI_S: ret = GS_KEY_S; break;
	case kVK_ANSI_D: ret = GS_KEY_D; break;
	case kVK_ANSI_F: ret = GS_KEY_F; break;
	case kVK_ANSI_H: ret = GS_KEY_H; break;
	case kVK_ANSI_G: ret = GS_KEY_G; break;
	case kVK_ANSI_Z: ret = GS_KEY_Z; break;
	case kVK_ANSI_X: ret = GS_KEY_X; break;
	case kVK_ANSI_C: ret = GS_KEY_C; break;
	case kVK_ANSI_V: ret = GS_KEY_V; break;
	case kVK_ANSI_B: ret = GS_KEY_B; break;
	case kVK_ANSI_Q: ret = GS_KEY_Q; break;
	case kVK_ANSI_W: ret = GS_KEY_W; break;
	case kVK_ANSI_E: ret = GS_KEY_E; break;
	case kVK_ANSI_R: ret = GS_KEY_R; break;
	case kVK_ANSI_Y: ret = GS_KEY_Y; break;
	case kVK_ANSI_T: ret = GS_KEY_T; break;
	case kVK_ANSI_1: ret = GS_KEY_1; break;
	case kVK_ANSI_2: ret = GS_KEY_2; break;
	case kVK_ANSI_3: ret = GS_KEY_3; break;
	case kVK_ANSI_4: ret = GS_KEY_4; break;
	case kVK_ANSI_6: ret = GS_KEY_6; break;
	case kVK_ANSI_5: ret = GS_KEY_5; break;
	case kVK_ANSI_Equal: ret = GS_KEY_EQUAL; break;
	case kVK_ANSI_9: ret = GS_KEY_9; break;
	case kVK_ANSI_7: ret = GS_KEY_7; break;
	case kVK_ANSI_Minus: ret = GS_KEY_MINUS; break;
	case kVK_ANSI_8: ret = GS_KEY_8; break;
	case kVK_ANSI_0: ret = GS_KEY_0; break;
	case kVK_ANSI_RightBracket: ret = GS_KEY_RIGHTBRACKET; break;
	case kVK_ANSI_O: ret = GS_KEY_O; break;
	case kVK_ANSI_U: ret = GS_KEY_U; break;
	case kVK_ANSI_LeftBracket: ret = GS_KEY_LEFTBRACKET; break;
	case kVK_ANSI_I: ret = GS_KEY_I; break;
	case kVK_ANSI_P: ret = GS_KEY_P; break;
	case kVK_ANSI_L: ret = GS_KEY_L; break;
	case kVK_ANSI_J: ret = GS_KEY_J; break;
	case kVK_ANSI_Quote: ret = GS_KEY_QUOTE; break;
	case kVK_ANSI_K: ret = GS_KEY_K; break;
	case kVK_ANSI_Semicolon: ret = GS_KEY_SEMICOLON; break;
	case kVK_ANSI_Backslash: ret = GS_KEY_BACKSLASH; break;
	case kVK_ANSI_Comma: ret = GS_KEY_COMMA; break;
	case kVK_ANSI_Slash: ret = GS_KEY_SLASH; break;
	case kVK_ANSI_N: ret = GS_KEY_N; break;
	case kVK_ANSI_M: ret = GS_KEY_M; break;
	case kVK_ANSI_Period: ret = GS_KEY_PERIOD; break;
	case kVK_ANSI_Grave: ret = GS_KEY_GRAVE; break;
	case kVK_ANSI_KeypadDecimal: ret = GS_KEY_KEYPADDECIMAL; break;
	case kVK_ANSI_KeypadMultiply: ret = GS_KEY_KEYPADMULTIPLY; break;
	case kVK_ANSI_KeypadPlus: ret = GS_KEY_KEYPADPLUS; break;
	case kVK_ANSI_KeypadClear: ret = GS_KEY_KEYPADCLEAR; break;
	case kVK_ANSI_KeypadDivide: ret = GS_KEY_KEYPADDIVIDE; break;
	case kVK_ANSI_KeypadEnter: ret = GS_KEY_KEYPADENTER; break;
	case kVK_ANSI_KeypadMinus: ret = GS_KEY_KEYPADMINUS; break;
	case kVK_ANSI_KeypadEquals: ret = GS_KEY_KEYPADEQUALS; break;
	case kVK_ANSI_Keypad0: ret = GS_KEY_KEYPAD0; break;
	case kVK_ANSI_Keypad1: ret = GS_KEY_KEYPAD1; break;
	case kVK_ANSI_Keypad2: ret = GS_KEY_KEYPAD2; break;
	case kVK_ANSI_Keypad3: ret = GS_KEY_KEYPAD3; break;
	case kVK_ANSI_Keypad4: ret = GS_KEY_KEYPAD4; break;
	case kVK_ANSI_Keypad5: ret = GS_KEY_KEYPAD5; break;
	case kVK_ANSI_Keypad6: ret = GS_KEY_KEYPAD6; break;
	case kVK_ANSI_Keypad7: ret = GS_KEY_KEYPAD7; break;
	case kVK_ANSI_Keypad8: ret = GS_KEY_KEYPAD8; break;
	case kVK_ANSI_Keypad9: ret = GS_KEY_KEYPAD9; break;
	case kVK_Return: ret = GS_KEY_RETURN; break;
	case kVK_Tab: ret = GS_KEY_TAB; break;
	case kVK_Space: ret = GS_KEY_SPACE; break;
	case kVK_Delete: ret = GS_KEY_DELETE; break;
	case kVK_Escape: ret = GS_KEY_ESCAPE; break;
	case kVK_Command: ret = GS_KEY_COMMAND; break;
	case kVK_Shift: ret = GS_KEY_SHIFT; break;
	case kVK_CapsLock: ret = GS_KEY_CAPSLOCK; break;
	case kVK_Option: ret = GS_KEY_OPTION; break;
	case kVK_Control: ret = GS_KEY_CONTROL; break;
	case kVK_RightShift: ret = GS_KEY_RIGHTSHIFT; break;
	case kVK_RightOption: ret = GS_KEY_RIGHTOPTION; break;
	case kVK_RightControl: ret = GS_KEY_RIGHTCONTROL; break;
	case kVK_Function: ret = GS_KEY_FUNCTION; break;
	case kVK_F17: ret = GS_KEY_F17; break;
	case kVK_VolumeUp: ret = GS_KEY_VOLUMEUP; break;
	case kVK_VolumeDown: ret = GS_KEY_VOLUMEDOWN; break;
	case kVK_Mute: ret = GS_KEY_MUTE; break;
	case kVK_F18: ret = GS_KEY_F18; break;
	case kVK_F19: ret = GS_KEY_F19; break;
	case kVK_F20: ret = GS_KEY_F20; break;
	case kVK_F5: ret = GS_KEY_F5; break;
	case kVK_F6: ret = GS_KEY_F6; break;
	case kVK_F7: ret = GS_KEY_F7; break;
	case kVK_F3: ret = GS_KEY_F3; break;
	case kVK_F8: ret = GS_KEY_F8; break;
	case kVK_F9: ret = GS_KEY_F9; break;
	case kVK_F11: ret = GS_KEY_F11; break;
	case kVK_F13: ret = GS_KEY_F13; break;
	case kVK_F16: ret = GS_KEY_F16; break;
	case kVK_F14: ret = GS_KEY_F14; break;
	case kVK_F10: ret = GS_KEY_F10; break;
	case kVK_F12: ret = GS_KEY_F12; break;
	case kVK_F15: ret = GS_KEY_F15; break;
	case kVK_Help: ret = GS_KEY_HELP; break;
	case kVK_Home: ret = GS_KEY_HOME; break;
	case kVK_PageUp: ret = GS_KEY_PAGEUP; break;
	case kVK_ForwardDelete: ret = GS_KEY_FORWARDDELETE; break;
	case kVK_F4: ret = GS_KEY_F4; break;
	case kVK_End: ret = GS_KEY_END; break;
	case kVK_F2: ret = GS_KEY_F2; break;
	case kVK_PageDown: ret = GS_KEY_PAGEDOWN; break;
	case kVK_F1: ret = GS_KEY_F1; break;
	case kVK_LeftArrow: ret = GS_KEY_LEFTARROW; break;
	case kVK_RightArrow: ret = GS_KEY_RIGHTARROW; break;
	case kVK_DownArrow: ret = GS_KEY_DOWNARROW; break;
	case kVK_UpArrow: ret = GS_KEY_UPARROW; break;
	default: ret = GS_KEY_NONE;
	}
	return ret;
}

static NPError osevent(void * ve) {
	NPCocoaEvent * e = (NPCocoaEvent *)ve;
	debug("event");
	switch (e->type) {
	case NPCocoaEventDrawRect: 
		debug("NPCocoaEventDrawRect"); 
		break;
	case NPCocoaEventMouseDown: 
		debug("NPCocoaEventMouseDown"); 
		gsInject(GS_EVENT_DOWN, GS_KEY_MOUSELEFT 
		    + e->data.mouse.buttonNumber, 0);
		break;
	case NPCocoaEventMouseUp: 
		debug("NPCocoaEventMouseUp"); 
		gsInject(GS_EVENT_UP, GS_KEY_MOUSELEFT 
		    + e->data.mouse.buttonNumber, 0);
		break;
	case NPCocoaEventMouseMoved: 
		debug("NPCocoaEventMouseMoved"); 
		gsInject(GS_EVENT_MOTION, 
		    e->data.mouse.pluginX, e->data.mouse.pluginY);
		break;
	case NPCocoaEventMouseEntered: 
		debug("NPCocoaEventMouseEntered"); 
		break;
	case NPCocoaEventMouseExited: 
		debug("NPCocoaEventMouseExited"); 
		break;
	case NPCocoaEventMouseDragged: 
		debug("NPCocoaEventMouseDragged"); 
		break;
	case NPCocoaEventKeyDown: 
		debug("NPCocoaEventKeyDown"); 
		gsInject(GS_EVENT_DOWN, mapkeycode(e->data.key.keyCode), 0);
		break;
	case NPCocoaEventKeyUp: 
		debug("NPCocoaEventKeyUp"); 
		gsInject(GS_EVENT_UP, mapkeycode(e->data.key.keyCode), 0);
		break;
	case NPCocoaEventFlagsChanged: 
		debug("NPCocoaEventFlagsChanged"); 
		break;
	case NPCocoaEventFocusChanged: 
		debug("NPCocoaEventFocusChanged"); 
		break;
	case NPCocoaEventWindowFocusChanged: 
		debug("NPCocoaEventWindowFocusChanged"); 
		break;
	case NPCocoaEventScrollWheel: 
		debug("NPCocoaEventScrollWheel"); 
		break;
	case NPCocoaEventTextInput: 
		debug("NPCocoaEventTextInput"); 
		break;
	default: assert(0);
	}
	return NPERR_NO_ERROR;
}

static const char * osmodpath() {
        static Dl_info info;
        dladdr(NP_GetEntryPoints, &info);
        return info.dli_fname;
}

const char * gsResPath() {
        char buf[256];
        static char ret[256];
        snprintf(buf, sizeof(buf), "%s/../../Resources", osmodpath());
        return realpath(buf, ret);
}

static void * osresolve(const char * name) {
	return dlsym(RTLD_SELF, name);
}

static NPError osgetval(NPP i, NPPVariable var, void * v) {
	NPError ret = NPERR_NO_ERROR;
	debug("os getvalue"); 
	switch(var) {
	case NPNVpluginDrawingModel:
		debug("getval drawingmodel");
		*(int*)v = NPDrawingModelOpenGL;
		break;
	case NPPVpluginCoreAnimationLayer:
		s_l.backgroundColor = CGColorGetConstantColor(kCGColorBlack);
                s_l.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
		*(CALayer**)v = s_l;
		break;
	default: 
		debug("os getval default"); 
		ret = NPERR_GENERIC_ERROR; 
		break;
	}
	return ret;
} 

@implementation Layer
- (void)drawInCGLContext:(CGLContextObj)ct
	     pixelFormat:(CGLPixelFormatObj)pf
	    forLayerTime:(CFTimeInterval)lt
	     displayTime:(const CVTimeStamp *)dt
{
        gsInject(GS_EVENT_TICK, 0, 0);
	gsInject(GS_EVENT_DRAW, 0, 0);
	[super drawInCGLContext: ct pixelFormat: pf
                   forLayerTime: lt displayTime: dt];
}

@end

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
