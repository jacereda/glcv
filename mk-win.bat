mk() {
     gcc $* -I src src/win.c -lgdi32 -lopengl32
}
install -d b
mk test/simple.c -o b/simple.exe
mk test/test.c -o b/test.exe

