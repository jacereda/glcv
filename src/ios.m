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
#include "gs.h"
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

static Delegate * s_delegate = 0;

Delegate * getDelegate() {
	assert(s_delegate);
	return s_delegate;
}

static void fakekeyuni(unsigned key, unsigned unicode) {
	gsInject(GSC_DOWN, key, 0);
	gsInject(GSC_UNICODE, unicode, 0);
	gsInject(GSC_UP, key, 0);
}

static unsigned mapkey(unsigned k) {
	unsigned ret;
	switch (k) {
	case 'A': ret = GSK_A; break;
	case 'B': ret = GSK_B; break;
	case 'C': ret = GSK_C; break;
	case 'D': ret = GSK_D; break;
	case 'E': ret = GSK_E; break;
	case 'F': ret = GSK_F; break;
	case 'G': ret = GSK_G; break;
	case 'H': ret = GSK_H; break;
	case 'I': ret = GSK_I; break;
	case 'J': ret = GSK_J; break;
	case 'K': ret = GSK_K; break;
	case 'L': ret = GSK_L; break;
	case 'M': ret = GSK_M; break;
	case 'N': ret = GSK_N; break;
	case 'O': ret = GSK_O; break;
	case 'P': ret = GSK_P; break;
	case 'Q': ret = GSK_Q; break;
	case 'R': ret = GSK_R; break;
	case 'S': ret = GSK_S; break;
	case 'T': ret = GSK_T; break;
	case 'U': ret = GSK_U; break;
	case 'V': ret = GSK_V; break;
	case 'W': ret = GSK_W; break;
	case 'X': ret = GSK_X; break;
	case 'Y': ret = GSK_Y; break;
	case 'Z': ret = GSK_Z; break;
	case '0': ret = GSK_0; break;
	case '1': ret = GSK_1; break;
	case '2': ret = GSK_2; break;
	case '3': ret = GSK_3; break;
	case '4': ret = GSK_4; break;
	case '5': ret = GSK_5; break;
	case '6': ret = GSK_6; break;
	case '7': ret = GSK_7; break;
	case '8': ret = GSK_8; break;
	case '9': ret = GSK_9; break;
	case '=': ret = GSK_EQUAL; break;
	case '\b': ret = GSK_DELETE; break;
	case '\n': ret = GSK_RETURN; break;
	default: ret = GSK_NONE;
	}
	return ret;
}

static void fakekey(unsigned unicode) {
	int upper = isupper(unicode);
	int luni = toupper(unicode);
	if (upper)
		gsInject(GSC_DOWN, GSK_SHIFT, 0);
	fakekeyuni(mapkey(luni), unicode);
	if (upper)
		gsInject(GSC_UP, GSK_SHIFT, 0);
}

@implementation GLView
+ (Class) layerClass {
        return [CAEAGLLayer class];
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch locationInView: self];
        gsInject(GSC_MOTION, loc.x, loc.y);
        gsInject(GSC_DOWN, GSK_MOUSELEFT, 0);
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch locationInView: self];
        gsInject(GSC_MOTION, loc.x, loc.y);
        gsInject(GSC_UP, GSK_MOUSELEFT, 0);
}


- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) e
{
        UITouch* touch = [touches anyObject];
        CGPoint loc = [touch previousLocationInView: self];
        gsInject(GSC_MOTION, loc.x, loc.y);
}

@end

@implementation Delegate

- (void) applicationWillTerminate: (UIApplication*) a {
	gsInject(GSC_CLOSE, 0, 0);
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
        gsInject(GSC_UPDATE, 0, 0);
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
	gsInject(GSC_RESIZE, r.size.width, r.size.height);
	gsInject(GSC_GLINIT, 0, 0);
}

- (void) applicationDidFinishLaunching: (UIApplication*) application 
{
	s_delegate = self;
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
	if (gsInject(GSC_INIT, argc, (intptr_t)argv)) {
		UIApplicationMain(argc, argv, nil, @"Delegate");
		ret = gsInject(GSC_TERM, 0, 0);
	}
	[pool release];
        return ret;
}

void gsReportV(const char *fmt, va_list ap) {
	FILE *out = fopen("/tmp/gs.log", "a");
	if(out) {
		vfprintf(out, fmt, ap);
		fputs("\n", out);
		fclose(out);
	}
}
