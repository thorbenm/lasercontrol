# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/programming/lasercontrol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/programming/lasercontrol

# Include any dependencies generated for this target.
include CMakeFiles/lasercontrol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lasercontrol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lasercontrol.dir/flags.make

CMakeFiles/lasercontrol.dir/main.cpp.o: CMakeFiles/lasercontrol.dir/flags.make
CMakeFiles/lasercontrol.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/programming/lasercontrol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lasercontrol.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lasercontrol.dir/main.cpp.o -c /home/pi/programming/lasercontrol/main.cpp

CMakeFiles/lasercontrol.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lasercontrol.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/programming/lasercontrol/main.cpp > CMakeFiles/lasercontrol.dir/main.cpp.i

CMakeFiles/lasercontrol.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lasercontrol.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/programming/lasercontrol/main.cpp -o CMakeFiles/lasercontrol.dir/main.cpp.s

CMakeFiles/lasercontrol.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/lasercontrol.dir/main.cpp.o.requires

CMakeFiles/lasercontrol.dir/main.cpp.o.provides: CMakeFiles/lasercontrol.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/lasercontrol.dir/build.make CMakeFiles/lasercontrol.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/lasercontrol.dir/main.cpp.o.provides

CMakeFiles/lasercontrol.dir/main.cpp.o.provides.build: CMakeFiles/lasercontrol.dir/main.cpp.o


CMakeFiles/lasercontrol.dir/spi.cpp.o: CMakeFiles/lasercontrol.dir/flags.make
CMakeFiles/lasercontrol.dir/spi.cpp.o: spi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/programming/lasercontrol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lasercontrol.dir/spi.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lasercontrol.dir/spi.cpp.o -c /home/pi/programming/lasercontrol/spi.cpp

CMakeFiles/lasercontrol.dir/spi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lasercontrol.dir/spi.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/programming/lasercontrol/spi.cpp > CMakeFiles/lasercontrol.dir/spi.cpp.i

CMakeFiles/lasercontrol.dir/spi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lasercontrol.dir/spi.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/programming/lasercontrol/spi.cpp -o CMakeFiles/lasercontrol.dir/spi.cpp.s

CMakeFiles/lasercontrol.dir/spi.cpp.o.requires:

.PHONY : CMakeFiles/lasercontrol.dir/spi.cpp.o.requires

CMakeFiles/lasercontrol.dir/spi.cpp.o.provides: CMakeFiles/lasercontrol.dir/spi.cpp.o.requires
	$(MAKE) -f CMakeFiles/lasercontrol.dir/build.make CMakeFiles/lasercontrol.dir/spi.cpp.o.provides.build
.PHONY : CMakeFiles/lasercontrol.dir/spi.cpp.o.provides

CMakeFiles/lasercontrol.dir/spi.cpp.o.provides.build: CMakeFiles/lasercontrol.dir/spi.cpp.o


# Object files for target lasercontrol
lasercontrol_OBJECTS = \
"CMakeFiles/lasercontrol.dir/main.cpp.o" \
"CMakeFiles/lasercontrol.dir/spi.cpp.o"

# External object files for target lasercontrol
lasercontrol_EXTERNAL_OBJECTS =

lasercontrol: CMakeFiles/lasercontrol.dir/main.cpp.o
lasercontrol: CMakeFiles/lasercontrol.dir/spi.cpp.o
lasercontrol: CMakeFiles/lasercontrol.dir/build.make
lasercontrol: CMakeFiles/lasercontrol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/programming/lasercontrol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lasercontrol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lasercontrol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lasercontrol.dir/build: lasercontrol

.PHONY : CMakeFiles/lasercontrol.dir/build

CMakeFiles/lasercontrol.dir/requires: CMakeFiles/lasercontrol.dir/main.cpp.o.requires
CMakeFiles/lasercontrol.dir/requires: CMakeFiles/lasercontrol.dir/spi.cpp.o.requires

.PHONY : CMakeFiles/lasercontrol.dir/requires

CMakeFiles/lasercontrol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lasercontrol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lasercontrol.dir/clean

CMakeFiles/lasercontrol.dir/depend:
	cd /home/pi/programming/lasercontrol && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/programming/lasercontrol /home/pi/programming/lasercontrol /home/pi/programming/lasercontrol /home/pi/programming/lasercontrol /home/pi/programming/lasercontrol/CMakeFiles/lasercontrol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lasercontrol.dir/depend

