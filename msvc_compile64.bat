@echo off
pushd msvc_bin64
cl  /O2 /arch:AVX /Fe:test64.exe /nologo /TC ../src/*.c -I ../third_party/SDL/x86_64-VC/include  /I ../include /link /SUBSYSTEM:CONSOLE /LIBPATH:../third_party/SDL/x86_64-VC/lib/x64 SDL2main.lib SDL2.lib
popd