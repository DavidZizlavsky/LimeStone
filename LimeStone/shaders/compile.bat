@echo off
echo Starting shader compilation...
if "%VULKAN_SDK%"=="" (
    echo [ERROR] Environment variable VULKAN_SDK is not set
    pause
    exit /b 1
)
"%VULKAN_SDK%\Bin\glslc.exe" shader.vert -o vert.spv
"%VULKAN_SDK%\Bin\glslc.exe" shader.frag -o frag.spv
pause