#include "gs.h"
#include "gsgl.h"
#include <assert.h>

static int s_running = 1;
static GLuint g_prg;
static GLint g_time;

static GLuint loadShader(GLenum type, const char * src) {
	GLuint sh = glCreateShader(type);
	glShaderSource(sh, 1, &src, (void*)0);
	glCompileShader(sh);
	return sh;
}

int init(int argc, char ** argv) {
	return 1;
	}

void glinit() {
	const char vss[] =
		"attribute vec4 pos;   \n"
		"uniform float t;            \n"
		"void main()                 \n"
		"{                           \n"
		"   gl_Position = pos; \n"
		"   gl_Position.y += sin(t) / 2.0; \n"
		"}                           \n";
	const char fss[] =
		"void main()                                \n"
		"{                                          \n"
		" gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);  \n"
		"}                                          \n";
	GLuint vs = loadShader(GL_VERTEX_SHADER, vss);
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, fss);
	GLuint prg = glCreateProgram();
	glAttachShader(prg, vs);
	glAttachShader(prg, fs);
	glLinkProgram(prg);
	g_prg = prg;
	glBindAttribLocation(g_prg, 0, "pos");
	g_time = glGetUniformLocation(g_prg, "t");
}

int term() {
	return 1;
}

int tick() {
	return s_running;
}

void draw(void) {
	GLfloat ver[] = {0.0f, 0.5f, 0.0f,
			 -0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f};
	static float t = 0;
        static int i = 0;
	glViewport(0, 0, gsWidth(), gsHeight());
        glClearColor((i++ & 0xff) / 255.0f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	t += 1/60.0;
//	glClearColor(0.7, 0.7, 0.7, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(g_prg);
	glUniform1f(g_time, t);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ver);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
}

void event(const ev * e) {
	switch (evType(e)) {
	case GS_EVENT_CLOSE:
		s_running = 0;
		break;
	default: break;
	}
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   End: **
*/
