@echo off

echo Compiling shaders ...

slangc src\shaders\vert.slang -target spirv -profile spirv_1_5 -entry main -stage vertex -o src\spvs\vert.spv
slangc src\shaders\frag.slang -target spirv -profile spirv_1_5 -entry main -stage fragment -o src\spvs\frag.spv
slangc src\shaders\comp.slang -target spirv -profile spirv_1_5 -entry main -stage compute -o src\spvs\comp.spv

echo Configuring CMake...
cmake -S . -B build ^
    -G "MinGW Makefiles" ^
    -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe ^
    -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe ^
    -DCMAKE_BUILD_TYPE=Release ^
    -Dglfw3_DIR=C:/glfw-mingw/lib/cmake/glfw3

echo Building...
cmake --build build

echo Running...
build\vk_app.exe
