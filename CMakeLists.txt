cmake_minimum_required(VERSION 3.21)
project(Vulkan)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

#set(VULKAN_SDK "C:/VulkanSDK/1.3.250.1")
#set(Vulkan_INCLUDE_DIR "${VULKAN_SDK}/Include")
#set(Vulkan_LIBRARY "${VULKAN_SDK}/Lib")

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME}
        src/models/Building.cpp
        src/models/Building.h
        src/path_finding.cpp
        src/path_finding.h
        src/test/test_selection.cpp
        src/test/test_selection.h
        src/shaders/Shaders.cpp
        src/shaders/Shaders.h
        src/models/Component.cpp
        src/models/Component.h
        src/models/consts/ShaderName.cpp
        src/file_watcher.cpp
        src/file_watcher.h
        src/main.cpp
        src/window.cpp
        src/window.h
        src/scene/scene_handler.cpp
        src/scene/scene_handler.h
        src/vk/gra_setup.cpp
        src/vk/gra_setup.h
        src/vk/shading/gra_depth.cpp
        src/vk/shading/gra_depth.h
        src/vk/presentation/gra_surface.cpp
        src/vk/presentation/gra_surface.h
        src/vk/setup/gra_logical_device.cpp
        src/vk/setup/gra_physical_device.cpp
        src/vk/setup/gra_queue_families.cpp
        src/vk/setup/gra_instance.cpp
        src/vk/setup/gra_debug.cpp

        src/vk/setup/gra_queue_families.cpp.h
        src/vk/setup/gra_physical_device.h
        src/vk/setup/gra_logical_device.h
        src/vk/setup/gra_instance.h
        src/vk/setup/gra_debug.h
        src/vk/presentation/gra_swap_chain.cpp
        src/vk/presentation/gra_swap_chain.h
        src/vk/presentation/gra_image_views.cpp
        src/vk/presentation/gra_image_views.h
        src/vk/pipeline/gra_pipeline.cpp src/vk/pipeline/gra_pipeline.h src/vk/pipeline/gra_shader.cpp src/vk/pipeline/gra_shader.h src/vk/pipeline/gra_render_passes.cpp src/vk/pipeline/gra_render_passes.h src/file_util.h src/vk/drawing/gra_framebuffers.cpp src/vk/drawing/gra_framebuffers.h src/vk/drawing/gra_command_buffers.cpp src/vk/drawing/gra_command_buffers.h src/vk/drawing/gra_drawing.cpp src/vk/drawing/gra_drawing.h src/vk/shading/gra_vertex.cpp src/vk/shading/gra_vertex.h src/vk/shading/gra_fragment.cpp src/vk/shading/gra_fragment.h src/vk/shading/gra_memory_utils.cpp src/vk/shading/gra_memory_utils.h src/camera.cpp src/camera.h src/vk/shading/gra_uniform.cpp src/vk/shading/gra_uniform.h src/vk/shading/gra_texture.cpp src/vk/shading/gra_texture.h
        src/rendering/Model.cpp
        src/rendering/Model.h
        src/rendering/Mesh2D.cpp src/rendering/Mesh2D.h
        src/rendering/Vertex.cpp src/rendering/Vertex.h
        src/math/Vec3.cpp src/math/Vec3.h
        src/math/Vec2.cpp src/math/Vec2.h
        src/rendering/Entity.cpp src/rendering/Entity.h
        src/models/Map.cpp src/models/Map.h
        src/models/Villager.cpp src/models/Villager.h
        src/network/RemoteLan.cpp src/network/RemoteLan.h
        src/game/vills/Maleness.cpp
        src/game/vills/Maleness.h
        src/game/vills/Femaleness.cpp
        src/game/vills/Femaleness.h
        src/game/vills/Vill.cpp
        src/game/vills/Vill.h
        src/timer_util.cpp
        src/timer_util.h
        src/game/vills/Bodyparts.cpp src/game/vills/Bodyparts.h
        src/file_util.cpp
        src/models/SelectionBoxModel.cpp
        src/models/SelectionBoxModel.h
        src/math/math_stuff.h
        src/scene/scene_data.h
        src/scene/scene_data.cpp
        src/math/math_stuff.cpp)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Libraries/socket.io-client-cpp)
target_link_libraries(${PROJECT_NAME} sioclient)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw/include)

target_link_libraries(${PROJECT_NAME}
        glfw)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
        )

# shader compile
# add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND cmd //C ${CMAKE_CURRENT_LIST_DIR}/res/models/compile.bat)
# add_custom_target(compile_shaders
#         COMMAND cmd /c ${CMAKE_CURRENT_LIST_DIR}/res/models/compile.bat
# )
# add_dependencies(${PROJECT_NAME} compile_shaders)

# execute_process(
#   COMMAND cmd /C ${CMAKE_CURRENT_LIST_DIR}/res/models/compile.bat
# )


# Copy resource folder to binary
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_dependencies(${PROJECT_NAME} copy_assets)


if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RMDEV=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC RMDEV=0)
endif()
