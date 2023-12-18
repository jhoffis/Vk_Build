for /r %%i in (*.frag) do "%VULKAN_SDK%/Bin/glslc" "%%i" -o "res/shaders/compiled/%%~ni_frag.spv"
for /r %%i in (*.vert) do "%VULKAN_SDK%/Bin/glslc" "%%i" -o "res/shaders/compiled/%%~ni_vert.spv"