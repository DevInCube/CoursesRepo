mkdir build
gcc -c cJSON.c -o build/libcjson.o
cd build
ar rcs libcjson.a libcjson.o
rm libcjson.o
cd ../
