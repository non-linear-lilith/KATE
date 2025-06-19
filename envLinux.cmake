# Linux environment configuration for KATE Vulkan Project

# Find Vulkan SDK path
find_package(Vulkan REQUIRED)
if(Vulkan_FOUND)
    set(VULKAN_SDK_PATH ${Vulkan_INCLUDE_DIRS}/..)
    message(STATUS "Vulkan SDK found at: ${VULKAN_SDK_PATH}")
else()
    # Fallback paths for common Linux installations
    set(VULKAN_SDK_PATHS
        "/usr"
        "/usr/local"
        "$ENV{HOME}/VulkanSDK"
        "$ENV{VULKAN_SDK}"
    )
    
    foreach(PATH ${VULKAN_SDK_PATHS})
        if(EXISTS "${PATH}/include/vulkan/vulkan.h")
            set(VULKAN_SDK_PATH ${PATH})
            message(STATUS "Vulkan SDK found at: ${VULKAN_SDK_PATH}")
            break()
        endif()
    endforeach()
    
    if(NOT VULKAN_SDK_PATH)
        message(FATAL_ERROR "Vulkan SDK not found. Please install vulkan-sdk or set VULKAN_SDK environment variable")
    endif()
endif()

# Set library paths for Linux
set(GLFW_LIB_NAME "glfw")
set(VULKAN_LIB_NAME "vulkan")

# Additional Linux-specific compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")