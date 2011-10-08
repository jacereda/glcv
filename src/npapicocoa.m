#include "npapi.c"
#include <QuartzCore/QuartzCore.h>
#include <Carbon/Carbon.h> // For keycodes

@interface Layer : CAOpenGLLayer {
@public
        int initialized;
        int width;
        int height;
}

- (void)drawInCGLContext:(CGLContextObj)ct
	     pixelFormat:(CGLPixelFormatObj)pf
	    forLayerTime:(CFTimeInterval)lt
	     displayTime:(const CVTimeStamp *)dt;
- (BOOL)canDrawInCGLContext:(CGLContextObj)ct
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
}

static void osterm() {
	[s_l release];
        s_l = 0;
}

static void osglinit(NPWindow * w) {
        s_l.backgroundColor = CGColorGetConstantColor(kCGColorBlack);
        s_l.opaque = YES;
        s_l.asynchronous = YES;
        s_l->initialized = 0;
        s_l->width = w->width;
	s_l->height = w->height;
}

static unsigned mapkeycode(unsigned k) {
	unsigned ret;
	switch (k) {
	case kVK_ANSI_A: ret = GSK_A; break;
	case kVK_ANSI_S: ret = GSK_S; break;
	case kVK_ANSI_D: ret = GSK_D; break;
	case kVK_ANSI_F: ret = GSK_F; break;
	case kVK_ANSI_H: ret = GSK_H; break;
	case kVK_ANSI_G: ret = GSK_G; break;
	case kVK_ANSI_Z: ret = GSK_Z; break;
	case kVK_ANSI_X: ret = GSK_X; break;
	case kVK_ANSI_C: ret = GSK_C; break;
	case kVK_ANSI_V: ret = GSK_V; break;
	case kVK_ANSI_B: ret = GSK_B; break;
	case kVK_ANSI_Q: ret = GSK_Q; break;
	case kVK_ANSI_W: ret = GSK_W; break;
	case kVK_ANSI_E: ret = GSK_E; break;
	case kVK_ANSI_R: ret = GSK_R; break;
	case kVK_ANSI_Y: ret = GSK_Y; break;
	case kVK_ANSI_T: ret = GSK_T; break;
	case kVK_ANSI_1: ret = GSK_1; break;
	case kVK_ANSI_2: ret = GSK_2; break;
	case kVK_ANSI_3: ret = GSK_3; break;
	case kVK_ANSI_4: ret = GSK_4; break;
	case kVK_ANSI_6: ret = GSK_6; break;
	case kVK_ANSI_5: ret = GSK_5; break;
	case kVK_ANSI_Equal: ret = GSK_EQUAL; break;
	case kVK_ANSI_9: ret = GSK_9; break;
	case kVK_ANSI_7: ret = GSK_7; break;
	case kVK_ANSI_Minus: ret = GSK_MINUS; break;
	case kVK_ANSI_8: ret = GSK_8; break;
	case kVK_ANSI_0: ret = GSK_0; break;
	case kVK_ANSI_RightBracket: ret = GSK_RIGHTBRACKET; break;
	case kVK_ANSI_O: ret = GSK_O; break;
	case kVK_ANSI_U: ret = GSK_U; break;
	case kVK_ANSI_LeftBracket: ret = GSK_LEFTBRACKET; break;
	case kVK_ANSI_I: ret = GSK_I; break;
	case kVK_ANSI_P: ret = GSK_P; break;
	case kVK_ANSI_L: ret = GSK_L; break;
	case kVK_ANSI_J: ret = GSK_J; break;
	case kVK_ANSI_Quote: ret = GSK_QUOTE; break;
	case kVK_ANSI_K: ret = GSK_K; break;
	case kVK_ANSI_Semicolon: ret = GSK_SEMICOLON; break;
	case kVK_ANSI_Backslash: ret = GSK_BACKSLASH; break;
	case kVK_ANSI_Comma: ret = GSK_COMMA; break;
	case kVK_ANSI_Slash: ret = GSK_SLASH; break;
	case kVK_ANSI_N: ret = GSK_N; break;
	case kVK_ANSI_M: ret = GSK_M; break;
	case kVK_ANSI_Period: ret = GSK_PERIOD; break;
	case kVK_ANSI_Grave: ret = GSK_GRAVE; break;
	case kVK_ANSI_KeypadDecimal: ret = GSK_KEYPADDECIMAL; break;
	case kVK_ANSI_KeypadMultiply: ret = GSK_KEYPADMULTIPLY; break;
	case kVK_ANSI_KeypadPlus: ret = GSK_KEYPADPLUS; break;
	case kVK_ANSI_KeypadClear: ret = GSK_KEYPADCLEAR; break;
	case kVK_ANSI_KeypadDivide: ret = GSK_KEYPADDIVIDE; break;
	case kVK_ANSI_KeypadEnter: ret = GSK_KEYPADENTER; break;
	case kVK_ANSI_KeypadMinus: ret = GSK_KEYPADMINUS; break;
	case kVK_ANSI_KeypadEquals: ret = GSK_KEYPADEQUALS; break;
	case kVK_ANSI_Keypad0: ret = GSK_KEYPAD0; break;
	case kVK_ANSI_Keypad1: ret = GSK_KEYPAD1; break;
	case kVK_ANSI_Keypad2: ret = GSK_KEYPAD2; break;
	case kVK_ANSI_Keypad3: ret = GSK_KEYPAD3; break;
	case kVK_ANSI_Keypad4: ret = GSK_KEYPAD4; break;
	case kVK_ANSI_Keypad5: ret = GSK_KEYPAD5; break;
	case kVK_ANSI_Keypad6: ret = GSK_KEYPAD6; break;
	case kVK_ANSI_Keypad7: ret = GSK_KEYPAD7; break;
	case kVK_ANSI_Keypad8: ret = GSK_KEYPAD8; break;
	case kVK_ANSI_Keypad9: ret = GSK_KEYPAD9; break;
	case kVK_Return: ret = GSK_RETURN; break;
	case kVK_Tab: ret = GSK_TAB; break;
	case kVK_Space: ret = GSK_SPACE; break;
	case kVK_Delete: ret = GSK_DELETE; break;
	case kVK_Escape: ret = GSK_ESCAPE; break;
	case kVK_Command: ret = GSK_COMMAND; break;
	case kVK_Shift: ret = GSK_SHIFT; break;
	case kVK_CapsLock: ret = GSK_CAPSLOCK; break;
	case kVK_Option: ret = GSK_OPTION; break;
	case kVK_Control: ret = GSK_CONTROL; break;
	case kVK_RightShift: ret = GSK_RIGHTSHIFT; break;
	case kVK_RightOption: ret = GSK_RIGHTOPTION; break;
	case kVK_RightControl: ret = GSK_RIGHTCONTROL; break;
	case kVK_Function: ret = GSK_FUNCTION; break;
	case kVK_F17: ret = GSK_F17; break;
	case kVK_VolumeUp: ret = GSK_VOLUMEUP; break;
	case kVK_VolumeDown: ret = GSK_VOLUMEDOWN; break;
	case kVK_Mute: ret = GSK_MUTE; break;
	case kVK_F18: ret = GSK_F18; break;
	case kVK_F19: ret = GSK_F19; break;
	case kVK_F20: ret = GSK_F20; break;
	case kVK_F5: ret = GSK_F5; break;
	case kVK_F6: ret = GSK_F6; break;
	case kVK_F7: ret = GSK_F7; break;
	case kVK_F3: ret = GSK_F3; break;
	case kVK_F8: ret = GSK_F8; break;
	case kVK_F9: ret = GSK_F9; break;
	case kVK_F11: ret = GSK_F11; break;
	case kVK_F13: ret = GSK_F13; break;
	case kVK_F16: ret = GSK_F16; break;
	case kVK_F14: ret = GSK_F14; break;
	case kVK_F10: ret = GSK_F10; break;
	case kVK_F12: ret = GSK_F12; break;
	case kVK_F15: ret = GSK_F15; break;
	case kVK_Help: ret = GSK_HELP; break;
	case kVK_Home: ret = GSK_HOME; break;
	case kVK_PageUp: ret = GSK_PAGEUP; break;
	case kVK_ForwardDelete: ret = GSK_FORWARDDELETE; break;
	case kVK_F4: ret = GSK_F4; break;
	case kVK_End: ret = GSK_END; break;
	case kVK_F2: ret = GSK_F2; break;
	case kVK_PageDown: ret = GSK_PAGEDOWN; break;
	case kVK_F1: ret = GSK_F1; break;
	case kVK_LeftArrow: ret = GSK_LEFTARROW; break;
	case kVK_RightArrow: ret = GSK_RIGHTARROW; break;
	case kVK_DownArrow: ret = GSK_DOWNARROW; break;
	case kVK_UpArrow: ret = GSK_UPARROW; break;
	default: ret = GSK_NONE;
	}
	return ret;
}

static NPError osevent(void * ve) {
	NPCocoaEvent * e = (NPCocoaEvent *)ve;
	switch (e->type) {
	case NPCocoaEventDrawRect: 
		debug("NPCocoaEventDrawRect"); 
		break;
	case NPCocoaEventMouseDown: 
		debug("NPCocoaEventMouseDown"); 
		gsInject(GSC_DOWN, GSK_MOUSELEFT 
                         + e->data.mouse.buttonNumber, 0);
		break;
	case NPCocoaEventMouseUp: 
		debug("NPCocoaEventMouseUp"); 
		gsInject(GSC_UP, GSK_MOUSELEFT 
                         + e->data.mouse.buttonNumber, 0);
		break;
	case NPCocoaEventMouseMoved: 
		debug("NPCocoaEventMouseMoved"); 
		gsInject(GSC_MOTION, 
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
		gsInject(GSC_DOWN, mapkeycode(e->data.key.keyCode), 0);
		break;
	case NPCocoaEventKeyUp: 
		debug("NPCocoaEventKeyUp"); 
		gsInject(GSC_UP, mapkeycode(e->data.key.keyCode), 0);
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
	default: 
                debug("Unknown event");
                assert(0);
	}
	return NPERR_NO_ERROR;
}

static NPError osgetval(NPP i, NPPVariable var, void * v) {
	NPError ret = NPERR_NO_ERROR;
	debug("os getvalue %d", var); 
	switch(var) {
	case NPNVpluginDrawingModel:
		debug("getval drawingmodel");
		*(int*)v = NPDrawingModelOpenGL;
		break;
	case NPPVpluginCoreAnimationLayer:
		debug("getval calayer");
                s_l = [[Layer layer] retain];	
                s_l.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
		*(CALayer**)v = s_l;
                // I can't get Chrome to start displaying properly, 
                // this hack seems to work
                [s_l performSelector: @selector(setNeedsDisplay) 
                          withObject:nil 
                          afterDelay: 1];
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
        if (!initialized) {
                gsInject(GSC_RESIZE, width, height);
                gsInject(GSC_GLINIT, 0, 0);
                initialized = 1;
        }
        gsInject(GSC_UPDATE, 0, 0);
	[super drawInCGLContext: ct pixelFormat: pf
                   forLayerTime: lt displayTime: dt];
}

- (BOOL)canDrawInCGLContext:(CGLContextObj)ct
                pixelFormat:(CGLPixelFormatObj)pf
               forLayerTime:(CFTimeInterval)lt
                displayTime:(const CVTimeStamp *)dt
{
        return YES;
}

@end

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
