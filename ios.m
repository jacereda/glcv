/*
  Copyright (c) 2008-2010, Jorge Acereda Maciá
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:
        
  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
        
  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. Neither the name of the author nor the names of its contributors
  may be used to endorse or promote products derived from this
  software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "gs.c"
#include "gsgl.h"
#include <OpenGLES/ES2/glext.h>
#include <assert.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface GLView : UIView {}
@end

@interface TextField : UITextField {}
@end

@interface Delegate : NSObject <UIApplicationDelegate, UITextFieldDelegate> {
        UIWindow * win;
        GLView * view; 
        EAGLContext * ctx;
	UITextField * tf;
@public
        CADisplayLink * dpylink;
}

- (void) update;
@end

static Delegate * g_delegate = 0;

Delegate * getDelegate() {
	assert(g_delegate);
	return g_delegate;
}

static void fakekeyuni(unsigned key, unsigned unicode) {
	got(GS_EVENT_DOWN, key, 0);
	got(GS_EVENT_UNICODE, unicode, 0);
	got(GS_EVENT_UP, key, 0);
}

static unsigned mapkey(unsigned k) {
	unsigned ret;
	switch (k) {
	case 'A': ret = GS_KEY_A; break;
	case 'B': ret = GS_KEY_B; break;
	case 'C': ret = GS_KEY_C; break;
	case 'D': ret = GS_KEY_D; break;
	case 'E': ret = GS_KEY_E; break;
	case 'F': ret = GS_KEY_F; break;
	case 'G': ret = GS_KEY_G; break;
	case 'H': ret = GS_KEY_H; break;
	case 'I': ret = GS_KEY_I; break;
	case 'J': ret = GS_KEY_J; break;
	case 'K': ret = GS_KEY_K; break;
	case 'L': ret = GS_KEY_L; break;
	case 'M': ret = GS_KEY_M; break;
	case 'N': ret = GS_KEY_N; break;
	case 'O': ret = GS_KEY_O; break;
	case 'P': ret = GS_KEY_P; break;
	case 'Q': ret = GS_KEY_Q; break;
	case 'R': ret = GS_KEY_R; break;
	case 'S': ret = GS_KEY_S; break;
	case 'T': ret = GS_KEY_T; break;
	case 'U': ret = GS_KEY_U; break;
	case 'V': ret = GS_KEY_V; break;
	case 'W': ret = GS_KEY_W; break;
	case 'X': ret = GS_KEY_X; break;
	case 'Y': ret = GS_KEY_Y; break;
	case 'Z': ret = GS_KEY_Z; break;
	case '0': ret = GS_KEY_0; break;
	case '1': ret = GS_KEY_1; break;
	case '2': ret = GS_KEY_2; break;
	case '3': ret = GS_KEY_3; break;
	case '4': ret = GS_KEY_4; break;
	case '5': ret = GS_KEY_5; break;
	case '6': ret = GS_KEY_6; break;
	case '7': ret = GS_KEY_7; break;
	case '8': ret = GS_KEY_8; break;
	case '9': ret = GS_KEY_9; break;
	case '=': ret = GS_KEY_EQUAL; break;
	case '\b': ret = GS_KEY_DELETE; break;
	case '\n': ret = GS_KEY_RETURN; break;
	default: ret = GS_KEY_NONE;
	}
	return ret;
}

static void fakekey(unsigned unicode) {
	int upper = isupper(unicode);
	int luni = toupper(unicode);
	if (upper)
		got(GS_EVENT_DOWN, GS_KEY_SHIFT, 0);
	fakekeyuni(mapkey(luni), unicode);
	if (upper)
		got(GS_EVENT_UP, GS_KEY_SHIFT, 0);
}

@implementation GLView
+ (Class) layerClass {
        return [CAEAGLLayer class];
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch locationInView: self];
        got(GS_EVENT_MOTION, loc.x, loc.y);
        got(GS_EVENT_DOWN, GS_KEY_MOUSELEFT, 0);
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch locationInView: self];
        got(GS_EVENT_MOTION, loc.x, loc.y);
        got(GS_EVENT_UP, GS_KEY_MOUSELEFT, 0);
}


- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch previousLocationInView: self];
        got(GS_EVENT_MOTION, loc.x, loc.y);
}

@end

@implementation Delegate

- (void) applicationWillTerminate: (UIApplication*) a {
	got(GS_EVENT_CLOSE, 0, 0);
	[self update];
}

- (BOOL) TextField: (UITextField *)tf 
shouldChangeCharactersInRange: (NSRange)range 
 replacementString: (NSString *)s {
        int sl = [s length];
        int i;
	if (!sl) 
		fakekey('\b');
        for (i = 0; i < sl; i++)
		fakekey([s characterAtIndex: i]);
	return NO;
}

- (BOOL)TextFieldShouldReturn: (UITextField*)tf {
	fakekey('\n');
	return NO;
}

- (void)showKeyboard {
	[tf becomeFirstResponder];
	[tf setHidden: NO];
}

- (void)hideKeyboard {
	[tf setHidden: YES];
	[tf resignFirstResponder];	
}

- (void)update {
	tick();
	draw();
	[ctx presentRenderbuffer: GL_RENDERBUFFER];
}

-(void) initContext {
	GLuint cb, fb;
        ctx = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES2];
	[EAGLContext setCurrentContext: ctx];
	glGenFramebuffers(1, &fb);
	glGenRenderbuffers(1, &cb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glBindRenderbuffer(GL_RENDERBUFFER , cb);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
				  GL_COLOR_ATTACHMENT0, 
				  GL_RENDERBUFFER, 
				  cb);
	[ctx renderbufferStorage: GL_RENDERBUFFER 
		    fromDrawable: (CAEAGLLayer *) [view layer]];
}

- (void) initWindow {
        CGRect r = [[UIScreen mainScreen] bounds];
        win = [[UIWindow alloc] initWithFrame: r];
        view = [[GLView alloc] initWithFrame: r];
	tf = [[UITextField alloc] initWithFrame: CGRectMake(0,0,0,0)];
	[tf setDelegate: self];
	[tf setAutocapitalizationType: UITextAutocapitalizationTypeNone];
	[tf setAutocorrectionType: UITextAutocorrectionTypeNo];
	[tf setEnablesReturnKeyAutomatically: NO];
	[tf setText: @" "];
	[tf setHidden: YES];
        [win makeKeyAndVisible];
        [win addSubview: view];
	[win addSubview: tf];

	[self initContext];
	got(GS_EVENT_RESIZE, r.size.width, r.size.height);
}

- (void) applicationDidFinishLaunching: (UIApplication*) application 
{
	g_delegate = self;
        dpylink = [CADisplayLink displayLinkWithTarget: self
                                 selector: @selector(update)];
        [dpylink setFrameInterval: 1];
        [dpylink addToRunLoop: [NSRunLoop currentRunLoop] 
                 forMode: NSDefaultRunLoopMode];
	[self initWindow];
}

- (void) dealloc
{
        [dpylink release];
        [win release];
        [view release];
        [ctx release];
        [super dealloc];
}

@end

int gsShowKeyboard() {
	[getDelegate() showKeyboard];
	return 1;
}

int gsHideKeyboard() {
	[getDelegate() hideKeyboard];
	return 1;
}

int main(int argc, char ** argv) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int ret = 42;
	if (init(argc, argv)) {
		UIApplicationMain(argc, argv, nil, @"Delegate");
		ret = term();
	}
	[pool release];
        return ret;
}

