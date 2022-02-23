@ECHO off

ECHO Trying to Compile with mingw gcc
ECHO -----------------
pushd gcc_bin64
gcc -O3 -ffast-math ../src/*.c -o test64.exe -I ../third_party/SDL/x86_64-w64-mingw32/include -I ../include -L ../third_party/SDL/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2
popd
ECHO -----------------
ECHO END