CC = /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/gcc
COMMON = -arch i386 -m32 -isysroot /Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator3.2.sdk  -mmacosx-version-min=10.5 
CFLAGS= $(COMMON) -fobjc-abi-version=2 -fobjc-legacy-dispatch -Wall
CPPFLAGS = -D__IPHONE_OS_VERSION_MIN_REQUIRED=30200
LDFLAGS = $(COMMON) -framework OpenGLES -framework UIKit -framework QuartzCore -framework Foundation

install: test glestest
	cp test "$(HOME)/Library/Application Support/iPhone Simulator/4.0.2/Applications/awtest/awtest.app/awtest"
	cp glestest "$(HOME)/Library/Application Support/iPhone Simulator/4.0.2/Applications/gles/gles.app/gles"

test: test.o gs.o

glestest: glestest.o gs.o

gs.o: ios.m
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.a