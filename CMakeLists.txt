cmake_minimum_required(VERSION 3.25.1)
if(WIN32)
  include(${CMAKE_SOURCE_DIR}/envWindows.cmake)
  message(STATUS "WINDOWS ENVIRONMENT")

elseif(UNIX)
  include(${CMAKE_SOURCE_DIR}/envLinux.cmake)
  message(STATUS "LINUX ENVIRONMENT")
endif()

set(NAME KATE)

project(${NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INPUT "src/input/*.cpp")

file(GLOB_RECURSE INCLUDES "include/*.h" "include/.hpp")

#GLFW
set(GLFW_LIB "${CMAKE_SOURCE_DIR}/libs/glfw")
set(GLFW_INCLUDE_DIR "external/GLFW/include")
#VULKAN
set(VULKAN_LIB_DIR "${VULKAN_SDK_PATH}/Lib")
set(VULKAN_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
#glm
set(GLM_INCLUDE_DIR "external/glm")
#ImGui
set(IMGUI_INCLUDE_DIR "external/ImGui")
message(STATUS "IMGUI_INCLUDE_DIR: ${IMGUI_INCLUDE_DIR}")

set(IMGUI_SOURCES
    external/ImGui/imgui.cpp
    external/ImGui/imgui_draw.cpp
    external/ImGui/imgui_tables.cpp
    external/ImGui/imgui_widgets.cpp
    external/ImGui/backends/imgui_impl_glfw.cpp
    external/ImGui/backends/imgui_impl_vulkan.cpp
)

#tinyobjloader
set(TINYOBJ_INCLUDE_DIR "external/tinyobjloader")
message(STATUS "TINYOBJ_INCLUDE_DIR: ${TINYOBJ_INCLUDE_DIR}")

set(PROJECT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/input
    ${GLFW_INCLUDE_DIR} 
    ${VULKAN_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${TINYOBJ_INCLUDE_DIR}
)


set(PROJECT_LIB_DIRS 
    ${GLFW_LIB}
    ${VULKAN_LIB_DIR}
)

add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES 
IMPORTED_LOCATION ${GLFW_LIB}/glfw3.lib
IMPORTED_IMPLIB ${GLFW_LIB}/glfw3dll.lib
)

# Create ImGui as a separate library
add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC 
    ${IMGUI_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}  # Add GLFW include directory
    ${VULKAN_INCLUDE_DIR}  # Add Vulkan include directory
)

target_link_directories(imgui PUBLIC ${PROJECT_INCLUDE_DIRS} )

target_link_libraries(imgui PUBLIC glfw)


link_directories(${PROJECT_LIB_DIRS})

add_executable(${NAME} ${SOURCES} ${INPUT} )
target_include_directories(${NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})



target_link_libraries(${NAME} glfw "${VULKAN_LIB_DIR}/vulkan-1.lib" imgui)

target_compile_features(${NAME} PUBLIC cxx_std_17)


set_property(TARGET ${NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")


