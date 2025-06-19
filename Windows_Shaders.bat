@echo off

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
echo "Visual Studio 2022 Community Command Prompt initialized."
#cleaning the data folder in the bin directory
if exist bin\data (
    echo Cleaning up bin\data folder...
    rmdir /s /q bin\data
)
if not exist bin\data (
    echo No bin\data folder found, creating one...
    mkdir bin\data
    mkdir bin\data\shaders
)
#copying the models to the data folder
echo Copying models to bin\data folder...
xcopy /s /i /y data\models\* bin\data\models\


echo Compiling shaders...

%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.vert -o bin\data\shaders\simple_shader.vert.spv
%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.frag -o bin\data\shaders\simple_shader.frag.spv

%VULKAN_SDK%\Bin\glslc.exe data\shaders\point_light.vert -o bin\data\shaders\point_light.vert.spv
%VULKAN_SDK%\Bin\glslc.exe data\shaders\point_light.frag -o bin\data\shaders\point_light.frag.spv
echo Shaders compiled."v  