#include "gs.c"
#define evType evTypeC // conflicts
#include <AppKit/NSScreen.h>
#include <AppKit/NSWindow.h>
//#include <AppKit/NSCursor.h>
#include <AppKit/NSEvent.h>
#include <AppKit/NSOpenGL.h>
#include <AppKit/NSTextView.h>
#undef evType

#include <Carbon/Carbon.h> // For keycodes

static int g_done = 0;

#define dbg gsReport
#define err gsReport

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case GSE_QUIT: g_done = 1; break;
        default: ret = 0;
        }
        return ret;
}

static gskey mapkeycode(unsigned k) {
        gskey ret;
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

@interface Window : NSWindow {
}
@end 

@interface View : NSTextView<NSWindowDelegate> {
        unsigned _prevflags;
}
- (NSPoint)toAbs: (NSPoint)p;
@end

@implementation View

- (void) dealloc {
        [super dealloc];
}

- (NSPoint)toAbs: (NSPoint)p {
        return [[self window] convertBaseToScreen: p];
}

- (void)setConstrainedFrameSize:(NSSize)desiredSize{}


- (BOOL) acceptsFirstResponder {
        return YES;
}

- (void) windowDidResize:(NSNotification *)n {
        NSRect fr = [self frame];
        NSSize sz = fr.size;
        gsInject(GSE_RESIZE, sz.width, sz.height);
        gsInject(GSE_GLINIT, 0, 0);
}

- (BOOL) windowShouldClose: (id)s {
        gsInject(GSE_CLOSE, 0, 0);
        return NO;
}

- (void) handleKeyEvent: (NSEvent *)ev mode: (int) mode {
        gsInject(mode, mapkeycode([ev keyCode]), 0);
}

- (void) keyUp: (NSEvent *)ev {
        [self handleKeyEvent: ev mode: GSE_UP];
}

- (void) keyDown: (NSEvent *)ev {
        [self handleKeyEvent: ev mode: GSE_DOWN];
        [self interpretKeyEvents: [NSArray arrayWithObject: ev]];
}

- (void)deleteBackward:(id)sender {
        gsInject(GSE_UNICODE, GSK_DELETE, 0);
}

- (void)insertText:(id)s {
        int sl = [s length];
        int i;
        for (i = 0; i < sl; i++)
                gsInject(GSE_UNICODE, [s characterAtIndex: i], 0);
}

- (unsigned) keyFor: (unsigned)mask {
        unsigned ret = 0;
        switch (mask) {
        case NSShiftKeyMask: ret = GSK_SHIFT; break;
        case NSControlKeyMask: ret = GSK_CONTROL; break;
        case NSAlternateKeyMask: ret = GSK_OPTION; break;
        case NSCommandKeyMask: ret = GSK_COMMAND; break;
        default: assert(0);
        }
        return ret;
}

- (void) handleMod: (unsigned)mask 
             flags: (unsigned)flags {
        unsigned delta = flags ^ _prevflags;
        unsigned pressed = delta & flags;
        unsigned released = delta & ~flags;
        if (mask & pressed)
                gsInject(GSE_DOWN, [self keyFor: mask], 0);
        if (mask & released)
                gsInject(GSE_UP, [self keyFor: mask], 0);
}

- (void) flagsChanged: (NSEvent *)ev {
        unsigned flags = [ev modifierFlags];
        [self handleMod: NSShiftKeyMask flags: flags];
        [self handleMod: NSControlKeyMask flags: flags];
        [self handleMod: NSAlternateKeyMask flags: flags];
        [self handleMod: NSCommandKeyMask flags: flags];
        _prevflags = flags;
        [super flagsChanged: ev];
}

- (void) handleMouseEvent: (NSEvent *)ev mode: (int)mode{
        gsInject(mode, [ev buttonNumber] + GSK_MOUSELEFT, 0);
}

- (void) mouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_DOWN];
}

- (void) mouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_UP];
}

- (void) rightMouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_DOWN];
}

- (void) rightMouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_UP];
}

- (void) otherMouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_DOWN];
}

- (void) otherMouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: GSE_UP];
}

- (void)scrollWheel:(NSEvent *)ev {
        unsigned k = [ev deltaY] > 0? 
                GSK_MOUSEWHEELUP : GSK_MOUSEWHEELDOWN;
        gsInject(GSE_DOWN, k, 0);
        gsInject(GSE_UP, k, 0);
}

- (void) handleMotion: (NSEvent *)ev {
        NSPoint l = [ev locationInWindow];
        gsInject(GSE_MOTION, l.x, [self frame].size.height - l.y);
}

- (void) mouseMoved: (NSEvent *)ev {
        [self handleMotion: ev];
}

- (BOOL) isOpaque {
        return YES;
}

@end

@implementation Window

- (void) dealloc {
        [super dealloc];
}

- (BOOL) canBecomeKeyWindow
{
        return YES;
}

- (BOOL)isReleasedWhenClosed
{
        return NO;
}
@end

int gsrun(int argc, char ** argv) {
        NSAutoreleasePool * arp = [[NSAutoreleasePool alloc] init];
        ProcessSerialNumber psn = { 0, kCurrentProcess };
        Window * win;
        NSArray* scrs;
        NSScreen * scr;
        NSRect frm;
        View * view;
        int style = gsInject(GSQ_BORDERS, 0, 0)? 
                0 
                | NSTitledWindowMask
                | NSClosableWindowMask
                | NSMiniaturizableWindowMask 
                | NSResizableWindowMask
                : NSBorderlessWindowMask;
        GLint param = 1;
        NSOpenGLContext *ctx = 0;
        CGDirectDisplayID dpy = kCGDirectMainDisplay;
        NSOpenGLPixelFormatAttribute attr[] = {
                NSOpenGLPFAFullScreen,
                NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(dpy),
                NSOpenGLPFAColorSize, 24,
                NSOpenGLPFADepthSize, 32,
                NSOpenGLPFAAlphaSize, 8,
                NSOpenGLPFADoubleBuffer,
                NSOpenGLPFAAccelerated,
                NSOpenGLPFANoRecovery,
                0, 0, 0, 0, 0, 0, 0, 0,
        };
        NSOpenGLPixelFormat * fmt;
        NSRect rect;
        NSApplication * app;
        TransformProcessType(&psn, kProcessTransformToForegroundApplication);
        SetFrontProcess(&psn);
        app = [NSApplication sharedApplication];
        scrs = [NSScreen screens];
        scr = [scrs objectAtIndex: 0];
        rect.origin.x = gsInject(GSQ_XPOS, 0, 0);
        rect.origin.y = gsInject(GSQ_YPOS, 0, 0);
        rect.size.width = gsInject(GSQ_WIDTH, 0, 0);
        rect.size.height = gsInject(GSQ_HEIGHT, 0, 0);
        rect.origin.y = [scr frame].size.height - 1 - 
                rect.origin.y - rect.size.height;
        if (rect.size.width == -1)
                rect = [scr frame];
        win = [[Window alloc] initWithContentRect: rect
                                        styleMask: style
                                          backing: NSBackingStoreBuffered
                                            defer:NO];
        [win setLevel: NSPopUpMenuWindowLevel];
        frm = [Window contentRectForFrameRect: [win frame] styleMask: style];
        view = [[View alloc] initWithFrame: frm];
        [win makeFirstResponder: view];
        [win setDelegate: view];
        [win setContentView: view];
        [win setReleasedWhenClosed: NO];
        [win makeKeyAndOrderFront: view];

        fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: attr];
        ctx = [[NSOpenGLContext alloc] 
                              initWithFormat:fmt  
                                shareContext: 0];
        [fmt release];
        [ctx setView: view];
        [ctx setValues: &param forParameter: NSOpenGLCPSwapInterval];
        gsInject(GSE_INIT, argc, (intptr_t)argv);
        [ctx makeCurrentContext];
//      [NSCursor hide];
//              [app updateWindows];
        [app activateIgnoringOtherApps: YES];
        [app setWindowsNeedUpdate: YES];
        [app finishLaunching];

        [arp drain];
        do {
                arp = [[NSAutoreleasePool alloc] init];
                NSEvent * ev = [app nextEventMatchingMask: NSAnyEventMask
                                                untilDate: [NSDate distantPast]
                                                   inMode: NSDefaultRunLoopMode
                                                  dequeue: YES];
                [app sendEvent: ev];
                gsInject(GSE_UPDATE, 0, 0);
                [arp drain];
                [ctx flushBuffer];
        } while (!g_done);
        arp = [[NSAutoreleasePool alloc] init];
        [ctx clearDrawable];
        [ctx release];
        [win makeKeyAndOrderFront: nil];
        [win makeFirstResponder: nil];
        [win setDelegate: nil];
        [win setContentView: nil];
        [win close];
        [win release];
        [view release];
        [arp drain];
        return gsInject(GSE_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
