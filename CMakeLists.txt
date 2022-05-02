cmake_minimum_required(VERSION 3.21)
project(Vulkan)

set(CMAKE_CXX_STANDARD 23)

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)


file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/window.cpp
        src/window.h
        src/scene_handler.cpp
        src/scene_handler.h
        src/scene_data.cpp
        src/scene_data.h
        src/vk/gra_setup.cpp src/vk/gra_setup.h src/vk/gra_presentation.cpp src/vk/gra_presentation.h)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glm)
target_link_libraries(${PROJECT_NAME} glm::glm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/stb)

#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw-3.3.2.bin.WIN64/lib-mingw-w64/libglfw3dll.a)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw-3.3.2.bin.WIN64/lib-mingw-w64/libglfw3.a)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glfw/include)

target_link_libraries(${PROJECT_NAME}
        glfw)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Change this to false if you want to disable warnings_as_errors in developer mode
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)


# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
        "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
        )
