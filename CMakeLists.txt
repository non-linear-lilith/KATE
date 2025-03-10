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
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE INPUT "src/input/*.cpp")
file(GLOB_RECURSE INCLUDES "include/*.h" "include/*.hpp")


#GLFW
set(GLFW_LIB "libs/lib-vc2022")
set(GLFW_INCLUDE_DIR "external/GLFW")
#VULKAN
set(VULKAN_LIB_DIR "${VULKAN_SDK_PATH}/Lib")
set(VULKAN_INCLUDE_DIR "${VULKAN_SDK_PATH}/Include")
#glm
set(GLM_INCLUDE_DIR "external/glm")
#freetype
set(FREETYPE_INCLUDE_DIR "external/freetype")
message(STATUS "FREETYPE_INCLUDE_DIR: ${FREETYPE_INCLUDE_DIR}")
#tinyobjloader
set(TINYOBJ_INCLUDE_DIR "external/tinyobjloader")
message(STATUS "TINYOBJ_INCLUDE_DIR: ${TINYOBJ_INCLUDE_DIR}")

set(PROJECT_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIR} 
    ${VULKAN_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
    ${TINYOBJ_INCLUDE_DIR}
)

set(PROJECT_LIB_DIRS 
    ${GLFW_LIB}
    ${VULKAN_LIB_DIR}
)

add_library(kate_lib SHARED ${SOURCE_FILES} ${INPUT})
target_link_directories(kate_lib PUBLIC ${PROJECT_INCLUDE_DIRS})

include_directories(${PROJECT_INCLUDE_DIRS})

link_directories(${PROJECT_LIB_DIRS})

#Build shared library

add_executable(${NAME} ${SOURCE_FILES} ${INPUT})

target_link_libraries(${NAME} kate_lib ${GLFW_LIB}/glfw3.dll ${VULKAN_LIB_DIR}/vulkan-1.lib kate_lib)


target_include_directories(${NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_include_directories(kate_lib PUBLIC ${PROJECT_INCLUDE_DIRS})
