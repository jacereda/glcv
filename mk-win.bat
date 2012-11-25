set MK=gcc $* -I src src/win.c 
set LIBS=-lgdi32 -lopengl32
mkdir b
%MK% test/simple.c -o b/simple.exe %LIBS
%MK% test/test.c -o b/test.exe %LIBS
