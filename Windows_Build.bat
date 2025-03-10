@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo "Visual Studio 2022 Community Command Prompt initialized."

echo "Compiling shaders..."
%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.vert -o data\shaders\simple_shader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.frag -o data\shaders\simple_shader.frag.spv
echo "Shaders compiled."
echo .
echo .
echo "Building, creating build folder if none exist..."
if not exist build (
    echo "No build folder found, creating one..."
    mkdir build
)
echo "Build folder found, continuing at build folder..."
cd build

echo Starting Cmake build:
echo .
echo .
echo "cmake -S ../ -B ."
cmake -S ../ -B . 
echo .
echo .

echo "cmake --build ."
cmake --build . 
echo . 
echo .
echo Cmake build done.

cd ..
if not exist bin (
    mkdir bin
)

SET includes=/I"src" /I"%VULKAN_SDK%\Include" /I"external" /I"include" /I"external\GLFW" /I"external\tinyobjloader" /I"external\glm" /I"external\freetype"
SET links=/link /LIBPATH:"%VULKAN_SDK%\Lib" vulkan-1.lib /LIBPATH:"libs\lib-vc2022" glfw3.lib user32.lib gdi32.lib shell32.lib 
SET defines=/D DEBUG

echo "Building main..."
cl /Od /MD /Zi /EHcs %includes% %defines% /Fe"bin\KATE_TESTING.exe" "src\*.cpp" "src\input\keyboard_input.cpp" %links% 
    
echo "Main built."

echo "Building dll..."
cl /LD /Od /MD /Zi /EHcs %includes% %defines% /Fe"bin\kate_lib.dll" "src\*.cpp" "src\input\keyboard_input.cpp" %links% 
echo "Dll built."


del *.obj
del *.pdb

cd bin
echo "Cleaning up..."
del *.obj
del *.ilk
del *.pdb
echo "Build done."
