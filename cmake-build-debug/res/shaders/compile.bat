for /r %%i in (*.frag, *.vert) do C:/VulkanSDK/1.2.141.2/Bin/glslc %%i -o %%~ni.spv
pause