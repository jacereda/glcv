#include "cv.c"
#define evType evTypeC // conflicts
#include <AppKit/NSScreen.h>
#include <AppKit/NSWindow.h>
#include <AppKit/NSCursor.h>
#include <AppKit/NSEvent.h>
#include <AppKit/NSOpenGL.h>
#include <AppKit/NSTextView.h>
#undef evType

#include <Carbon/Carbon.h> // For keycodes

static int g_done = 0;

#define dbg cvReport
#define err cvReport

intptr_t osEvent(ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case CVE_QUIT: g_done = 1; break;
        case CVE_SHOWCURSOR: [NSCursor unhide]; break;
        case CVE_HIDECURSOR: [NSCursor hide]; break;
        default: ret = 0;
        }
        return ret;
}

static cvkey mapkeycode(unsigned k) {
        cvkey ret;
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
        [[NSOpenGLContext currentContext] update];

        cvInject(CVE_RESIZE, sz.width, sz.height);
}

- (BOOL) windowShouldClose: (id)s {
        cvInject(CVE_CLOSE, 0, 0);
        return NO;
}

- (void) handleKeyEvent: (NSEvent *)ev mode: (int) mode {
        cvInject(mode, mapkeycode([ev keyCode]), 0);
}

- (void) keyUp: (NSEvent *)ev {
        [self handleKeyEvent: ev mode: CVE_UP];
}

- (void) keyDown: (NSEvent *)ev {
        [self handleKeyEvent: ev mode: CVE_DOWN];
        [self interpretKeyEvents: [NSArray arrayWithObject: ev]];
}

- (void)deleteBackward:(id)sender {
        cvInject(CVE_UNICODE, CVK_DELETE, 0);
}

- (void)insertText:(id)s {
        int sl = [s length];
        int i;
        for (i = 0; i < sl; i++)
                cvInject(CVE_UNICODE, [s characterAtIndex: i], 0);
}

- (unsigned) keyFor: (unsigned)mask {
        unsigned ret = 0;
        switch (mask) {
        case NSShiftKeyMask: ret = CVK_SHIFT; break;
        case NSControlKeyMask: ret = CVK_CONTROL; break;
        case NSAlternateKeyMask: ret = CVK_OPTION; break;
        case NSCommandKeyMask: ret = CVK_COMMAND; break;
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
                cvInject(CVE_DOWN, [self keyFor: mask], 0);
        if (mask & released)
                cvInject(CVE_UP, [self keyFor: mask], 0);
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
        cvInject(mode, [ev buttonNumber] + CVK_MOUSELEFT, 0);
}

- (void) mouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_DOWN];
}

- (void) mouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_UP];
}

- (void) rightMouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_DOWN];
}

- (void) rightMouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_UP];
}

- (void) otherMouseDown: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_DOWN];
}

- (void) otherMouseUp: (NSEvent*)ev {
        [self handleMouseEvent: ev mode: CVE_UP];
}

- (void)scrollWheel:(NSEvent *)ev {
        unsigned k = [ev deltaY] > 0? 
                CVK_MOUSEWHEELUP : CVK_MOUSEWHEELDOWN;
        cvInject(CVE_DOWN, k, 0);
        cvInject(CVE_UP, k, 0);
}

- (void) handleMotion: (NSEvent *)ev {
        NSPoint l = [ev locationInWindow];
        cvInject(CVE_MOTION, l.x, [self frame].size.height - l.y);
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

int cvrun(int argc, char ** argv) {
        NSAutoreleasePool * arp = [[NSAutoreleasePool alloc] init];
        ProcessSerialNumber psn = { 0, kCurrentProcess };
        Window * win;
        NSArray* scrs;
        NSScreen * scr;
        NSRect frm;
        View * view;
        int style = cvInject(CVQ_BORDERS, 0, 0)? 
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
        [app activateIgnoringOtherApps: YES];
        [app finishLaunching];

        scrs = [NSScreen screens];
        scr = [scrs objectAtIndex: 0];
        cvInject(CVE_INIT, argc, (intptr_t)argv);
        rect.origin.x = cvInject(CVQ_XPOS, 0, 0);
        rect.origin.y = cvInject(CVQ_YPOS, 0, 0);
        rect.size.width = cvInject(CVQ_WIDTH, 0, 0);
        rect.size.height = cvInject(CVQ_HEIGHT, 0, 0);
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

        fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: attr];
        ctx = [[NSOpenGLContext alloc] 
                              initWithFormat:fmt  
                                shareContext: 0];
        [fmt release];
        [ctx setView: view];
        [ctx setValues: &param forParameter: NSOpenGLCPSwapInterval];
        [ctx makeCurrentContext];
        cvInject(CVE_GLINIT, 0, 0);
        [win makeKeyAndOrderFront: view];
        cvInject(CVE_RESIZE, rect.size.width, rect.size.height);

        [arp drain];
        do {
                arp = [[NSAutoreleasePool alloc] init];
                NSEvent * ev = [app nextEventMatchingMask: NSAnyEventMask
                                                untilDate: [NSDate distantPast]
                                                   inMode: NSDefaultRunLoopMode
                                                  dequeue: YES];
                [app sendEvent: ev];
                cvInject(CVE_UPDATE, 0, 0);
                [arp drain];
                [ctx flushBuffer];
        } while (!g_done);
        arp = [[NSAutoreleasePool alloc] init];
        cvInject(CVE_GLTERM, 0, 0);
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
        return cvInject(CVE_TERM, 0, 0);
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
