@echo OFF

SET defines=-D_CRT_SECURE_NO_WARNINGS
SET includes=-Ilib -Ilib/raylib/include -Ilib/OpenAL/include
SET libpaths=-Llib/OpenAL -Llib/raylib
SET libs=-lmsvcrt -lOpenGL32 -lGdi32 -lWinMM -lkernel32 -lshell32 -lUser32 -lComdlg32 -lOpenAL32 -lraylib

echo Building Windows Layer
clang++ %defines% %includes% src/win_layer.cpp -c -o win_layer.obj -g
echo DONE
echo(

echo Building Project
clang++ %defines% %includes% %libpaths% %libs% win_layer.obj src/main.cpp -o main.exe  -g
echo DONE
echo(