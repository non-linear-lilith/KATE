#!/bin/bash

# KATE Vulkan Project - Linux Build Script
# Generic Linux build generator for Ubuntu, Arch, Fedora, etc.

set -e  # Exit on any error

echo "========================================="
echo "KATE Vulkan Engine - Linux Build Script"
echo "========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
        VERSION=$VERSION_ID
    else
        DISTRO="unknown"
    fi
    print_status "Detected distribution: $DISTRO $VERSION"
}

# Install dependencies based on distribution
install_dependencies() {
    print_status "Installing dependencies..."
    
    case $DISTRO in
        ubuntu|debian)
            sudo apt update
            sudo apt install -y \
                build-essential \
                cmake \
                pkg-config \
                libglfw3-dev \
                libvulkan-dev \
                vulkan-tools \
                vulkan-validationlayers-dev \
                spirv-tools \
                libglm-dev
            ;;
        fedora)
            sudo dnf install -y \
                gcc-c++ \
                cmake \
                pkgconfig \
                glfw-devel \
                vulkan-devel \
                vulkan-tools \
                vulkan-validation-layers-devel \
                spirv-tools \
                glm-devel
            ;;
        arch|manjaro)
            sudo pacman -S --needed \
                base-devel \
                cmake \
                pkgconf \
                glfw \
                vulkan-devel \
                vulkan-tools \
                vulkan-validation-layers \
                spirv-tools \
                glm
            ;;
        opensuse|opensuse-leap|opensuse-tumbleweed)
            sudo zypper install -y \
                gcc-c++ \
                cmake \
                pkg-config \
                glfw3-devel \
                vulkan-devel \
                vulkan-tools \
                vulkan-validationlayers-devel \
                spirv-tools \
                glm-devel
            ;;
        *)
            print_warning "Unknown distribution. Please install the following packages manually:"
            echo "- build-essential/gcc-c++/base-devel"
            echo "- cmake"
            echo "- pkg-config/pkgconf"
            echo "- glfw3-dev/glfw-devel/glfw"
            echo "- vulkan-dev/vulkan-devel"
            echo "- vulkan-tools"
            echo "- vulkan-validation-layers"
            echo "- spirv-tools"
            echo "- glm-dev/glm-devel/glm"
            read -p "Press Enter to continue after installing dependencies..."
            ;;
    esac
}

# Check if Vulkan SDK is available
check_vulkan() {
    print_status "Checking Vulkan installation..."
    
    if command -v vulkaninfo &> /dev/null; then
        print_status "Vulkan tools found"
        vulkaninfo --summary
    else
        print_error "Vulkan tools not found. Please install vulkan-tools package."
        exit 1
    fi
    
    # Check for validation layers
    if [ -d "/usr/share/vulkan/explicit_layer.d" ] || [ -d "/usr/local/share/vulkan/explicit_layer.d" ]; then
        print_status "Vulkan validation layers found"
    else
        print_warning "Vulkan validation layers not found. Debugging may be limited."
    fi
}

# Create build directory
setup_build() {
    print_status "Setting up build directory..."
    
    if [ -d "build" ]; then
        print_warning "Build directory exists. Cleaning..."
        rm -rf build
    fi
    
    mkdir build
    cd build
}

# Generate makefiles and build
build_project() {
    print_status "Generating makefiles with CMake..."
    
    # Use system default or user-specified generator
    CMAKE_GENERATOR=${CMAKE_GENERATOR:-"Unix Makefiles"}
    CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-"Release"}
    
    cmake .. \
        -G "$CMAKE_GENERATOR" \
        -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    if [ $? -ne 0 ]; then
        print_error "CMake configuration failed!"
        exit 1
    fi
    
    print_status "Building project..."
    
    # Detect number of CPU cores for parallel build
    NPROC=$(nproc 2>/dev/null || echo 4)
    
    if [ "$CMAKE_GENERATOR" = "Ninja" ]; then
        ninja
    else
        make -j$NPROC
    fi
    
    if [ $? -ne 0 ]; then
        print_error "Build failed!"
        exit 1
    fi
}

# Compile shaders
compile_shaders() {
    print_status "Compiling shaders..."
    
    cd ..  # Go back to project root
    
    if [ -f "Linux_Shaders.sh" ]; then
        chmod +x Linux_Shaders.sh
        ./Linux_Shaders.sh
    else
        print_warning "Linux_Shaders.sh not found. Compiling shaders manually..."
        
        if command -v glslc &> /dev/null; then
            for shader in data/shaders/*.vert data/shaders/*.frag; do
                if [ -f "$shader" ]; then
                    print_status "Compiling $shader"
                    glslc "$shader" -o "${shader}.spv"
                fi
            done
        else
            print_error "glslc not found. Please install shaderc or vulkan-tools."
            exit 1
        fi
    fi
    
    cd build
}

# Main execution
main() {
    print_status "Starting KATE Vulkan Engine build process..."
    
    # Parse command line arguments
    SKIP_DEPS=false
    CLEAN_BUILD=false
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            --skip-deps)
                SKIP_DEPS=true
                shift
                ;;
            --clean)
                CLEAN_BUILD=true
                shift
                ;;
            --debug)
                CMAKE_BUILD_TYPE="Debug"
                shift
                ;;
            --ninja)
                CMAKE_GENERATOR="Ninja"
                shift
                ;;
            -h|--help)
                echo "Usage: $0 [OPTIONS]"
                echo "Options:"
                echo "  --skip-deps    Skip dependency installation"
                echo "  --clean        Clean build directory before building"
                echo "  --debug        Build in Debug mode"
                echo "  --ninja        Use Ninja generator instead of Make"
                echo "  -h, --help     Show this help message"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    detect_distro
    
    if [ "$SKIP_DEPS" = false ]; then
        install_dependencies
    else
        print_warning "Skipping dependency installation"
    fi
    
    check_vulkan
    setup_build
    build_project
    compile_shaders
    
    print_status "Build completed successfully!"
    print_status "Executable: $(pwd)/KATE"
    print_status "To run: cd build && ./KATE"
}

# Run main function
main "$@"