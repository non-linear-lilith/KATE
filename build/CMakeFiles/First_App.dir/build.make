# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build

# Include any dependencies generated for this target.
include CMakeFiles/First_App.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/First_App.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/First_App.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/First_App.dir/flags.make

CMakeFiles/First_App.dir/src/first_app.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/first_app.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/first_app.cpp
CMakeFiles/First_App.dir/src/first_app.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/First_App.dir/src/first_app.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/first_app.cpp.o -MF CMakeFiles/First_App.dir/src/first_app.cpp.o.d -o CMakeFiles/First_App.dir/src/first_app.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/first_app.cpp

CMakeFiles/First_App.dir/src/first_app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/first_app.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/first_app.cpp > CMakeFiles/First_App.dir/src/first_app.cpp.i

CMakeFiles/First_App.dir/src/first_app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/first_app.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/first_app.cpp -o CMakeFiles/First_App.dir/src/first_app.cpp.s

CMakeFiles/First_App.dir/src/kate_device.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/kate_device.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_device.cpp
CMakeFiles/First_App.dir/src/kate_device.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/First_App.dir/src/kate_device.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/kate_device.cpp.o -MF CMakeFiles/First_App.dir/src/kate_device.cpp.o.d -o CMakeFiles/First_App.dir/src/kate_device.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_device.cpp

CMakeFiles/First_App.dir/src/kate_device.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/kate_device.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_device.cpp > CMakeFiles/First_App.dir/src/kate_device.cpp.i

CMakeFiles/First_App.dir/src/kate_device.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/kate_device.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_device.cpp -o CMakeFiles/First_App.dir/src/kate_device.cpp.s

CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_pipeline.cpp
CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o -MF CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o.d -o CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_pipeline.cpp

CMakeFiles/First_App.dir/src/kate_pipeline.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/kate_pipeline.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_pipeline.cpp > CMakeFiles/First_App.dir/src/kate_pipeline.cpp.i

CMakeFiles/First_App.dir/src/kate_pipeline.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/kate_pipeline.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_pipeline.cpp -o CMakeFiles/First_App.dir/src/kate_pipeline.cpp.s

CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_swapp_chain.cpp
CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o -MF CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o.d -o CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_swapp_chain.cpp

CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_swapp_chain.cpp > CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.i

CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_swapp_chain.cpp -o CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.s

CMakeFiles/First_App.dir/src/kate_window.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/kate_window.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_window.cpp
CMakeFiles/First_App.dir/src/kate_window.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/First_App.dir/src/kate_window.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/kate_window.cpp.o -MF CMakeFiles/First_App.dir/src/kate_window.cpp.o.d -o CMakeFiles/First_App.dir/src/kate_window.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_window.cpp

CMakeFiles/First_App.dir/src/kate_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/kate_window.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_window.cpp > CMakeFiles/First_App.dir/src/kate_window.cpp.i

CMakeFiles/First_App.dir/src/kate_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/kate_window.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/kate_window.cpp -o CMakeFiles/First_App.dir/src/kate_window.cpp.s

CMakeFiles/First_App.dir/src/main.cpp.o: CMakeFiles/First_App.dir/flags.make
CMakeFiles/First_App.dir/src/main.cpp.o: /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/main.cpp
CMakeFiles/First_App.dir/src/main.cpp.o: CMakeFiles/First_App.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/First_App.dir/src/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/First_App.dir/src/main.cpp.o -MF CMakeFiles/First_App.dir/src/main.cpp.o.d -o CMakeFiles/First_App.dir/src/main.cpp.o -c /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/main.cpp

CMakeFiles/First_App.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/First_App.dir/src/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/main.cpp > CMakeFiles/First_App.dir/src/main.cpp.i

CMakeFiles/First_App.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/First_App.dir/src/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/src/main.cpp -o CMakeFiles/First_App.dir/src/main.cpp.s

# Object files for target First_App
First_App_OBJECTS = \
"CMakeFiles/First_App.dir/src/first_app.cpp.o" \
"CMakeFiles/First_App.dir/src/kate_device.cpp.o" \
"CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o" \
"CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o" \
"CMakeFiles/First_App.dir/src/kate_window.cpp.o" \
"CMakeFiles/First_App.dir/src/main.cpp.o"

# External object files for target First_App
First_App_EXTERNAL_OBJECTS =

First_App: CMakeFiles/First_App.dir/src/first_app.cpp.o
First_App: CMakeFiles/First_App.dir/src/kate_device.cpp.o
First_App: CMakeFiles/First_App.dir/src/kate_pipeline.cpp.o
First_App: CMakeFiles/First_App.dir/src/kate_swapp_chain.cpp.o
First_App: CMakeFiles/First_App.dir/src/kate_window.cpp.o
First_App: CMakeFiles/First_App.dir/src/main.cpp.o
First_App: CMakeFiles/First_App.dir/build.make
First_App: /usr/lib/libglfw.so.3.3
First_App: /lib/libvulkan.so
First_App: CMakeFiles/First_App.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable First_App"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/First_App.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/First_App.dir/build: First_App
.PHONY : CMakeFiles/First_App.dir/build

CMakeFiles/First_App.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/First_App.dir/cmake_clean.cmake
.PHONY : CMakeFiles/First_App.dir/clean

CMakeFiles/First_App.dir/depend:
	cd /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build /home/kathy/Documents/Vulkan_Proyects/VISUAL_NOVEL_ENGINE/build/CMakeFiles/First_App.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/First_App.dir/depend

