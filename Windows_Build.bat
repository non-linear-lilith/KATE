@echo off
setlocal EnableDelayedExpansion

REM Header
echo.
echo ========================================
echo          KATE Vulkan Build Script      
echo ========================================
echo.

REM Initialize Visual Studio environment
echo [1/4] Initializing Visual Studio environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Failed to initialize Visual Studio environment
    echo Please make sure Visual Studio 2022 Community is installed
    pause
    exit /b 1
)
echo [OK] Visual Studio 2022 Community environment initialized
echo.

REM Compile shaders
echo [2/4] Compiling shaders...
echo   ^> Compiling simple_shader.vert
%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.vert -o data\shaders\simple_shader.vert.spv
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile simple_shader.vert
    pause
    exit /b 1
)

echo   ^> Compiling simple_shader.frag
%VULKAN_SDK%\Bin\glslc.exe data\shaders\simple_shader.frag -o data\shaders\simple_shader.frag.spv
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile simple_shader.frag
    pause
    exit /b 1
)

echo   ^> Compiling point_light.vert
%VULKAN_SDK%\Bin\glslc.exe data\shaders\point_light.vert -o data\shaders\point_light.vert.spv
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile point_light.vert
    pause
    exit /b 1
)

echo   ^> Compiling point_light.frag
%VULKAN_SDK%\Bin\glslc.exe data\shaders\point_light.frag -o data\shaders\point_light.frag.spv
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile point_light.frag
    pause
    exit /b 1
)

echo [OK] All shaders compiled successfully
echo.

REM Setup build directory
echo [3/4] Setting up build environment...
if not exist build (
    echo   ^> Creating build directory
    mkdir build
    echo [OK] Build directory created
) else (
    echo [OK] Build directory already exists
)

pushd build

REM CMake configuration
echo   ^> Running CMake configuration
cmake -S ../ -B .
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    popd
    pause
    exit /b 1
)

echo   ^> Building with CMake
cmake --build .
if %errorlevel% neq 0 (
    echo [ERROR] CMake build failed
    popd
    pause
    exit /b 1
)

echo [OK] CMake build completed successfully
popd

REM Create bin directory
if not exist bin (
    mkdir bin
)

echo.

REM Build main executable
echo [4/4] Building main executable...

SET includes=/I"src" /I"%VULKAN_SDK%\Include" /I"external" /I"include" /I"external\GLFW\include/" /I"external\tinyobjloader" /I"external\glm" /I"external\ImGui" /I"external\glfw\include"
SET links=/link /LIBPATH:"%VULKAN_SDK%\Lib" vulkan-1.lib /LIBPATH:"libs\GLFW" glfw3.lib user32.lib gdi32.lib shell32.lib opengl32.lib
SET defines=/D DEBUG

echo   ^> Compiling and linking KATE_TESTING.exe
cl /Od /MD /Zi /EHcs %includes% %defines% /Fo"build\\" /Fd"build\KATE.pdb" /Fe"bin\KATE_TESTING.exe" "src\*.cpp" "src\input\keyboard_input.cpp" "src\systems\*.cpp" "external\ImGui\*.cpp" "external\ImGui\backends\imgui_impl_glfw.cpp" "external\ImGui\backends\imgui_impl_vulkan.cpp" %links% >nul 2>&1

if %errorlevel% neq 0 (
    echo [ERROR] Main executable build failed
    echo Check the error messages above for details
    pause
    exit /b 1
)

echo [OK] Main executable built successfully
echo.

REM Success message
echo ========================================
echo          BUILD COMPLETED SUCCESS       
echo ========================================
echo.
echo Executable: bin\KATE_TESTING.exe
echo Debug info: build\KATE.pdb
echo.
echo Build completed at %DATE% %TIME%

