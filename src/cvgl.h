#if defined _WIN32
#include <windows.h>
#include <GL/gl.h>
#elif defined __ANDROID__
#include <GLES2/gl2.h>
#elif defined __IPHONE_OS_VERSION_MIN_REQUIRED
#include <OpenGLES/ES2/gl.h>
#elif defined __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
