mkdir build
gcc -c socket_win.c -lws2_32 -o build/libsocket.o
cd build
ar rcs libsocket.a libsocket.o
del libsocket.o
cd ../