set MK=clang -O2 -Wall -I src -D_CRT_SECURE_NO_WARNINGS src/win.c
set LIBS=-lgdi32 -lopengl32 -luser32 -ldwmapi
mkdir b
%MK% test/simple.c -o b/simple.exe %LIBS%
%MK% test/test.c -o b/test.exe %LIBS%
