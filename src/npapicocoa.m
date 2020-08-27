#include "npapi.c"
#include <assert.h>
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
        browser->setvalue(i, NPPVpluginDrawingModel, 
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
        case kVK_ANSI_A: ret = CVK_A; break;
        case kVK_ANSI_S: ret = CVK_S; break;
        case kVK_ANSI_D: ret = CVK_D; break;
        case kVK_ANSI_F: ret = CVK_F; break;
        case kVK_ANSI_H: ret = CVK_H; break;
        case kVK_ANSI_G: ret = CVK_G; break;
        case kVK_ANSI_Z: ret = CVK_Z; break;
        case kVK_ANSI_X: ret = CVK_X; break;
        case kVK_ANSI_C: ret = CVK_C; break;
        case kVK_ANSI_V: ret = CVK_V; break;
        case kVK_ANSI_B: ret = CVK_B; break;
        case kVK_ANSI_Q: ret = CVK_Q; break;
        case kVK_ANSI_W: ret = CVK_W; break;
        case kVK_ANSI_E: ret = CVK_E; break;
        case kVK_ANSI_R: ret = CVK_R; break;
        case kVK_ANSI_Y: ret = CVK_Y; break;
        case kVK_ANSI_T: ret = CVK_T; break;
        case kVK_ANSI_1: ret = CVK_1; break;
        case kVK_ANSI_2: ret = CVK_2; break;
        case kVK_ANSI_3: ret = CVK_3; break;
        case kVK_ANSI_4: ret = CVK_4; break;
        case kVK_ANSI_6: ret = CVK_6; break;
        case kVK_ANSI_5: ret = CVK_5; break;
        case kVK_ANSI_Equal: ret = CVK_EQUAL; break;
        case kVK_ANSI_9: ret = CVK_9; break;
        case kVK_ANSI_7: ret = CVK_7; break;
        case kVK_ANSI_Minus: ret = CVK_MINUS; break;
        case kVK_ANSI_8: ret = CVK_8; break;
        case kVK_ANSI_0: ret = CVK_0; break;
        case kVK_ANSI_RightBracket: ret = CVK_RIGHTBRACKET; break;
        case kVK_ANSI_O: ret = CVK_O; break;
        case kVK_ANSI_U: ret = CVK_U; break;
        case kVK_ANSI_LeftBracket: ret = CVK_LEFTBRACKET; break;
        case kVK_ANSI_I: ret = CVK_I; break;
        case kVK_ANSI_P: ret = CVK_P; break;
        case kVK_ANSI_L: ret = CVK_L; break;
        case kVK_ANSI_J: ret = CVK_J; break;
        case kVK_ANSI_Quote: ret = CVK_QUOTE; break;
        case kVK_ANSI_K: ret = CVK_K; break;
        case kVK_ANSI_Semicolon: ret = CVK_SEMICOLON; break;
        case kVK_ANSI_Backslash: ret = CVK_BACKSLASH; break;
        case kVK_ANSI_Comma: ret = CVK_COMMA; break;
        case kVK_ANSI_Slash: ret = CVK_SLASH; break;
        case kVK_ANSI_N: ret = CVK_N; break;
        case kVK_ANSI_M: ret = CVK_M; break;
        case kVK_ANSI_Period: ret = CVK_PERIOD; break;
        case kVK_ANSI_Grave: ret = CVK_GRAVE; break;
        case kVK_ANSI_KeypadDecimal: ret = CVK_KEYPADDECIMAL; break;
        case kVK_ANSI_KeypadMultiply: ret = CVK_KEYPADMULTIPLY; break;
        case kVK_ANSI_KeypadPlus: ret = CVK_KEYPADPLUS; break;
        case kVK_ANSI_KeypadClear: ret = CVK_KEYPADCLEAR; break;
        case kVK_ANSI_KeypadDivide: ret = CVK_KEYPADDIVIDE; break;
        case kVK_ANSI_KeypadEnter: ret = CVK_KEYPADENTER; break;
        case kVK_ANSI_KeypadMinus: ret = CVK_KEYPADMINUS; break;
        case kVK_ANSI_KeypadEquals: ret = CVK_KEYPADEQUALS; break;
        case kVK_ANSI_Keypad0: ret = CVK_KEYPAD0; break;
        case kVK_ANSI_Keypad1: ret = CVK_KEYPAD1; break;
        case kVK_ANSI_Keypad2: ret = CVK_KEYPAD2; break;
        case kVK_ANSI_Keypad3: ret = CVK_KEYPAD3; break;
        case kVK_ANSI_Keypad4: ret = CVK_KEYPAD4; break;
        case kVK_ANSI_Keypad5: ret = CVK_KEYPAD5; break;
        case kVK_ANSI_Keypad6: ret = CVK_KEYPAD6; break;
        case kVK_ANSI_Keypad7: ret = CVK_KEYPAD7; break;
        case kVK_ANSI_Keypad8: ret = CVK_KEYPAD8; break;
        case kVK_ANSI_Keypad9: ret = CVK_KEYPAD9; break;
        case kVK_Return: ret = CVK_RETURN; break;
        case kVK_Tab: ret = CVK_TAB; break;
        case kVK_Space: ret = CVK_SPACE; break;
        case kVK_Delete: ret = CVK_DELETE; break;
        case kVK_Escape: ret = CVK_ESCAPE; break;
        case kVK_Command: ret = CVK_COMMAND; break;
        case kVK_Shift: ret = CVK_SHIFT; break;
        case kVK_CapsLock: ret = CVK_CAPSLOCK; break;
        case kVK_Option: ret = CVK_OPTION; break;
        case kVK_Control: ret = CVK_CONTROL; break;
        case kVK_RightShift: ret = CVK_RIGHTSHIFT; break;
        case kVK_RightOption: ret = CVK_RIGHTOPTION; break;
        case kVK_RightControl: ret = CVK_RIGHTCONTROL; break;
        case kVK_Function: ret = CVK_FUNCTION; break;
        case kVK_F17: ret = CVK_F17; break;
        case kVK_VolumeUp: ret = CVK_VOLUMEUP; break;
        case kVK_VolumeDown: ret = CVK_VOLUMEDOWN; break;
        case kVK_Mute: ret = CVK_MUTE; break;
        case kVK_F18: ret = CVK_F18; break;
        case kVK_F19: ret = CVK_F19; break;
        case kVK_F20: ret = CVK_F20; break;
        case kVK_F5: ret = CVK_F5; break;
        case kVK_F6: ret = CVK_F6; break;
        case kVK_F7: ret = CVK_F7; break;
        case kVK_F3: ret = CVK_F3; break;
        case kVK_F8: ret = CVK_F8; break;
        case kVK_F9: ret = CVK_F9; break;
        case kVK_F11: ret = CVK_F11; break;
        case kVK_F13: ret = CVK_F13; break;
        case kVK_F16: ret = CVK_F16; break;
        case kVK_F14: ret = CVK_F14; break;
        case kVK_F10: ret = CVK_F10; break;
        case kVK_F12: ret = CVK_F12; break;
        case kVK_F15: ret = CVK_F15; break;
        case kVK_Help: ret = CVK_HELP; break;
        case kVK_Home: ret = CVK_HOME; break;
        case kVK_PageUp: ret = CVK_PAGEUP; break;
        case kVK_ForwardDelete: ret = CVK_FORWARDDELETE; break;
        case kVK_F4: ret = CVK_F4; break;
        case kVK_End: ret = CVK_END; break;
        case kVK_F2: ret = CVK_F2; break;
        case kVK_PageDown: ret = CVK_PAGEDOWN; break;
        case kVK_F1: ret = CVK_F1; break;
        case kVK_LeftArrow: ret = CVK_LEFTARROW; break;
        case kVK_RightArrow: ret = CVK_RIGHTARROW; break;
        case kVK_DownArrow: ret = CVK_DOWNARROW; break;
        case kVK_UpArrow: ret = CVK_UPARROW; break;
        default: ret = CVK_NONE;
        }
        return ret;
}

static void textinput(NPNSString * nt) {
        NSString * t = (NSString *)nt;
        int tl = [t length];
        int i;
        for (i = 0; i < tl; i++)
                cvInject(CVE_UNICODE, [t characterAtIndex: i], 0);
}

static NPError osevent(void * ve) {
        NPCocoaEvent * e = (NPCocoaEvent *)ve;
        switch (e->type) {
        case NPCocoaEventDrawRect: 
                debug("NPCocoaEventDrawRect"); 
                break;
        case NPCocoaEventMouseDown: 
                debug("NPCocoaEventMouseDown"); 
                cvInject(CVE_DOWN, CVK_MOUSELEFT 
                         + e->data.mouse.buttonNumber, 0);
                break;
        case NPCocoaEventMouseUp: 
                debug("NPCocoaEventMouseUp"); 
                cvInject(CVE_UP, CVK_MOUSELEFT 
                         + e->data.mouse.buttonNumber, 0);
                break;
        case NPCocoaEventMouseMoved: 
                debug("NPCocoaEventMouseMoved"); 
                cvInject(CVE_MOTION, 
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
                cvInject(CVE_DOWN, mapkeycode(e->data.key.keyCode), 0);
                textinput(e->data.key.characters);
                break;
        case NPCocoaEventKeyUp: 
                debug("NPCocoaEventKeyUp"); 
                cvInject(CVE_UP, mapkeycode(e->data.key.keyCode), 0);
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
                textinput(e->data.text.text);
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
        case NPPVpluginDrawingModel:
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
                cvInject(CVE_RESIZE, width, height);
                cvInject(CVE_GLINIT, 0, 0);
                initialized = 1;
        }
        cvInject(CVE_UPDATE, 0, 0);
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
