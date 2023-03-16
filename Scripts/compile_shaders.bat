@echo off

for %%f in ("%~dp0shaders\*.vert") do (
    C:\VulkanSDK\1.3.236.0\Bin\glslc.exe "%%f" -o "%%~dpnf.spv"
)

for %%f in ("%~dp0shaders\*.frag") do (
    C:\VulkanSDK\1.3.236.0\Bin\glslc.exe "%%f" -o "%%~dpnf.spv"
)

pause
