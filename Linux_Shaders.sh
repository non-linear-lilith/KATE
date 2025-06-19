#!/bin/bash

# KATE Vulkan Project - Linux Shader Compilation Script
# Compiles GLSL shaders to SPIR-V bytecode

set -e  # Exit on any error

echo "========================================="
echo "KATE Vulkan Engine - Shader Compilation"
echo "========================================="

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if glslc is available
check_glslc() {
    if ! command -v glslc &> /dev/null; then
        print_error "glslc compiler not found!"
        print_error "Please install shaderc or vulkan-tools package:"
        print_error "  Ubuntu/Debian: sudo apt install shaderc"
        print_error "  Fedora: sudo dnf install shaderc"
        print_error "  Arch: sudo pacman -S shaderc"
        exit 1
    fi
    
    print_status "Found glslc: $(which glslc)"
}

# Create shader directory if it doesn't exist
setup_shader_dir() {
    if [ ! -d "data/shaders" ]; then
        print_warning "Shader directory not found, creating data/shaders/"
        mkdir -p data/shaders
    fi
}

# Compile a single shader
compile_shader() {
    local shader_file="$1"
    local output_file="${shader_file}.spv"
    
    print_status "Compiling: $shader_file -> $output_file"
    
    # Compile with optimization and debug info
    glslc "$shader_file" -o "$output_file" -O
    
    if [ $? -eq 0 ]; then
        print_status "✓ Compiled successfully: $output_file"
        
        # Show file size
        if command -v stat &> /dev/null; then
            local size=$(stat -f%z "$output_file" 2>/dev/null || stat -c%s "$output_file" 2>/dev/null || echo "unknown")
            print_status "  Size: $size bytes"
        fi
    else
        print_error "✗ Failed to compile: $shader_file"
        exit 1
    fi
}

# Find and compile all shaders
compile_all_shaders() {
    print_status "Searching for shader files..."
    
    local shader_count=0
    local compiled_count=0
    
    # Find all vertex and fragment shaders
    for shader_file in data/shaders/*.vert data/shaders/*.frag data/shaders/*.comp data/shaders/*.geom data/shaders/*.tesc data/shaders/*.tese; do
        if [ -f "$shader_file" ]; then
            shader_count=$((shader_count + 1))
            
            # Check if recompilation is needed
            local output_file="${shader_file}.spv"
            local should_compile=true
            
            if [ -f "$output_file" ]; then
                # Check if source is newer than compiled version
                if [ "$shader_file" -ot "$output_file" ]; then
                    should_compile=false
                    print_status "⚡ Skipping (up to date): $shader_file"
                fi
            fi
            
            if [ "$should_compile" = true ]; then
                compile_shader "$shader_file"
                compiled_count=$((compiled_count + 1))
            fi
        fi
    done
    
    if [ $shader_count -eq 0 ]; then
        print_warning "No shader files found in data/shaders/"
        print_warning "Expected file extensions: .vert, .frag, .comp, .geom, .tesc, .tese"
    else
        print_status "Found $shader_count shader files"
        print_status "Compiled $compiled_count shaders"
    fi
}

# Clean compiled shaders
clean_shaders() {
    print_status "Cleaning compiled shaders..."
    
    local cleaned_count=0
    for spv_file in data/shaders/*.spv; do
        if [ -f "$spv_file" ]; then
            rm "$spv_file"
            cleaned_count=$((cleaned_count + 1))
            print_status "Removed: $spv_file"
        fi
    done
    
    if [ $cleaned_count -eq 0 ]; then
        print_status "No compiled shaders to clean"
    else
        print_status "Cleaned $cleaned_count compiled shaders"
    fi
}

# Show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -c, --clean    Clean all compiled shaders (.spv files)"
    echo "  -f, --force    Force recompilation of all shaders"
    echo "  -h, --help     Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0              # Compile shaders (incremental)"
    echo "  $0 --clean     # Clean compiled shaders"
    echo "  $0 --force     # Force recompile all shaders"
}

# Main execution
main() {
    local force_compile=false
    local clean_only=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -c|--clean)
                clean_only=true
                shift
                ;;
            -f|--force)
                force_compile=true
                shift
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                show_help
                exit 1
                ;;
        esac
    done
    
    setup_shader_dir
    
    if [ "$clean_only" = true ]; then
        clean_shaders
        exit 0
    fi
    
    check_glslc
    
    if [ "$force_compile" = true ]; then
        print_status "Force recompilation enabled"
        # Remove all .spv files to force recompilation
        for spv_file in data/shaders/*.spv; do
            if [ -f "$spv_file" ]; then
                rm "$spv_file"
            fi
        done
    fi
    
    compile_all_shaders
    
    print_status "Shader compilation completed!"
}

# Run main function
main "$@"