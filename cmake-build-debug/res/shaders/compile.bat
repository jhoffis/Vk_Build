for /r %%i in (*.frag) do "%VULKAN_SDK%/Bin/glslc" %%i -o %%~ni_frag.spv
for /r %%i in (*.vert) do "%VULKAN_SDK%/Bin/glslc" %%i -o %%~ni_vert.spv
pause