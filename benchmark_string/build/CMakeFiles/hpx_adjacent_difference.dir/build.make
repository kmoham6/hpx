# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/apps/cmake/3.26.3/bin/cmake

# The command to remove a file.
RM = /opt/apps/cmake/3.26.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /work/karame.mp/hpx_march/benchmark_time/benchmark_string

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /work/karame.mp/hpx_march/benchmark_time/benchmark_string/build

# Include any dependencies generated for this target.
include CMakeFiles/hpx_adjacent_difference.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hpx_adjacent_difference.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hpx_adjacent_difference.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hpx_adjacent_difference.dir/flags.make

CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o: CMakeFiles/hpx_adjacent_difference.dir/flags.make
CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o: /work/karame.mp/hpx_march/benchmark_time/benchmark_string/adjacent_difference.cpp
CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o: CMakeFiles/hpx_adjacent_difference.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/work/karame.mp/hpx_march/benchmark_time/benchmark_string/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o"
	/opt/apps/gcc/13.1.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o -MF CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o.d -o CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o -c /work/karame.mp/hpx_march/benchmark_time/benchmark_string/adjacent_difference.cpp

CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.i"
	/opt/apps/gcc/13.1.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /work/karame.mp/hpx_march/benchmark_time/benchmark_string/adjacent_difference.cpp > CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.i

CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.s"
	/opt/apps/gcc/13.1.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /work/karame.mp/hpx_march/benchmark_time/benchmark_string/adjacent_difference.cpp -o CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.s

# Object files for target hpx_adjacent_difference
hpx_adjacent_difference_OBJECTS = \
"CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o"

# External object files for target hpx_adjacent_difference
hpx_adjacent_difference_EXTERNAL_OBJECTS =

hpx_adjacent_difference: CMakeFiles/hpx_adjacent_difference.dir/adjacent_difference.cpp.o
hpx_adjacent_difference: CMakeFiles/hpx_adjacent_difference.dir/build.make
hpx_adjacent_difference: /work/karame.mp/hpx_march/hpx/build_release/lib/libhpx_wrap.a
hpx_adjacent_difference: /work/karame.mp/hpx_march/hpx/build_release/lib/libhpx_init.a
hpx_adjacent_difference: /work/karame.mp/hpx_march/hpx/build_release/lib/libhpx.so.1.9.0
hpx_adjacent_difference: /work/karame.mp/hpx_march/hpx/build_release/lib/libhpx_core.so
hpx_adjacent_difference: /usr/lib64/libtcmalloc_minimal.so
hpx_adjacent_difference: /opt/apps/hwloc/2.9.1/lib/libhwloc.so
hpx_adjacent_difference: CMakeFiles/hpx_adjacent_difference.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/work/karame.mp/hpx_march/benchmark_time/benchmark_string/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hpx_adjacent_difference"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hpx_adjacent_difference.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hpx_adjacent_difference.dir/build: hpx_adjacent_difference
.PHONY : CMakeFiles/hpx_adjacent_difference.dir/build

CMakeFiles/hpx_adjacent_difference.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hpx_adjacent_difference.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hpx_adjacent_difference.dir/clean

CMakeFiles/hpx_adjacent_difference.dir/depend:
	cd /work/karame.mp/hpx_march/benchmark_time/benchmark_string/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /work/karame.mp/hpx_march/benchmark_time/benchmark_string /work/karame.mp/hpx_march/benchmark_time/benchmark_string /work/karame.mp/hpx_march/benchmark_time/benchmark_string/build /work/karame.mp/hpx_march/benchmark_time/benchmark_string/build /work/karame.mp/hpx_march/benchmark_time/benchmark_string/build/CMakeFiles/hpx_adjacent_difference.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hpx_adjacent_difference.dir/depend

