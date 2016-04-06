mkdir build
gcc main.c file_win.c  ^
	-I ../libsocket ^
	-L ../libsocket/build/ -lsocket -lws2_32 ^
	-o build/http_server.exe