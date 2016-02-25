#include "cv.h"
#include "cvgl.h"
#include <assert.h>

static GLuint s_prg;
static GLint s_time;

static GLuint loadShader(GLenum type, const char * src) {
        GLuint sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, (void*)0);
        glCompileShader(sh);
        return sh;
}

static void glinit() {
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
        s_prg = prg;
        glBindAttribLocation(s_prg, 0, "pos");
        s_time = glGetUniformLocation(s_prg, "t");
}

static void draw(void) {
        GLfloat ver[] = {0.0f, 0.5f, 0.0f,
                         -0.5f, -0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f};
        static float t = 0;
        static int i = 0;
        glViewport(0, 0, cvWidth(), cvHeight());
        glClearColor((i++ & 0xff) / 255.0f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        t += 1/60.0;
        glUseProgram(s_prg);
        glUniform1f(s_time, t);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ver);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glFlush();
}

intptr_t event(const ev * e) {
        intptr_t ret = 1;
        switch (evType(e)) {
        case CVE_GLINIT: glinit(); break;
        case CVE_UPDATE: draw(); break;
        default: ret = 0; break;
        }
        return ret;
}

/* 
   Local variables: **
   c-file-style: "bsd" **
   c-basic-offset: 8 **
   indent-tabs-mode: nil **
   End: **
*/
