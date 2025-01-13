@echo OFF

SET includes=-Ilib -Ilib/raylib/include -Ilib/OpenAL/include
SET libpaths=-Llib/OpenAL -Llib/raylib 
SET libs=-lmsvcrt -lOpenGL32 -lGdi32 -lWinMM -lkernel32 -lshell32 -lUser32 -lOpenAL32 -lraylib

clang++ %includes% %libpaths% %libs% src/main.cpp -o main.exe