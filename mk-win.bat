install -d b
gcc -o b/test -I src src/win.c src/gs.c test/test.c -lgdi32 -lopengl32
