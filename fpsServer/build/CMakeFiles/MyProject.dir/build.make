# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/jt/project-improve/fpsServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jt/project-improve/fpsServer/build

# Include any dependencies generated for this target.
include CMakeFiles/MyProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyProject.dir/flags.make

CMakeFiles/MyProject.dir/server.cpp.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/server.cpp.o: ../server.cpp
CMakeFiles/MyProject.dir/server.cpp.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/project-improve/fpsServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyProject.dir/server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyProject.dir/server.cpp.o -MF CMakeFiles/MyProject.dir/server.cpp.o.d -o CMakeFiles/MyProject.dir/server.cpp.o -c /home/jt/project-improve/fpsServer/server.cpp

CMakeFiles/MyProject.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyProject.dir/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/project-improve/fpsServer/server.cpp > CMakeFiles/MyProject.dir/server.cpp.i

CMakeFiles/MyProject.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyProject.dir/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/project-improve/fpsServer/server.cpp -o CMakeFiles/MyProject.dir/server.cpp.s

CMakeFiles/MyProject.dir/MsgConn.cpp.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/MsgConn.cpp.o: ../MsgConn.cpp
CMakeFiles/MyProject.dir/MsgConn.cpp.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/project-improve/fpsServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyProject.dir/MsgConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyProject.dir/MsgConn.cpp.o -MF CMakeFiles/MyProject.dir/MsgConn.cpp.o.d -o CMakeFiles/MyProject.dir/MsgConn.cpp.o -c /home/jt/project-improve/fpsServer/MsgConn.cpp

CMakeFiles/MyProject.dir/MsgConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyProject.dir/MsgConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/project-improve/fpsServer/MsgConn.cpp > CMakeFiles/MyProject.dir/MsgConn.cpp.i

CMakeFiles/MyProject.dir/MsgConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyProject.dir/MsgConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/project-improve/fpsServer/MsgConn.cpp -o CMakeFiles/MyProject.dir/MsgConn.cpp.s

CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o: ../api/MoveHandle.cpp
CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/project-improve/fpsServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o -MF CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o.d -o CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o -c /home/jt/project-improve/fpsServer/api/MoveHandle.cpp

CMakeFiles/MyProject.dir/api/MoveHandle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyProject.dir/api/MoveHandle.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/project-improve/fpsServer/api/MoveHandle.cpp > CMakeFiles/MyProject.dir/api/MoveHandle.cpp.i

CMakeFiles/MyProject.dir/api/MoveHandle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyProject.dir/api/MoveHandle.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/project-improve/fpsServer/api/MoveHandle.cpp -o CMakeFiles/MyProject.dir/api/MoveHandle.cpp.s

CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o: CMakeFiles/MyProject.dir/flags.make
CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o: ../api/RouterRegister.cpp
CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o: CMakeFiles/MyProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/project-improve/fpsServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o -MF CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o.d -o CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o -c /home/jt/project-improve/fpsServer/api/RouterRegister.cpp

CMakeFiles/MyProject.dir/api/RouterRegister.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyProject.dir/api/RouterRegister.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/project-improve/fpsServer/api/RouterRegister.cpp > CMakeFiles/MyProject.dir/api/RouterRegister.cpp.i

CMakeFiles/MyProject.dir/api/RouterRegister.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyProject.dir/api/RouterRegister.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/project-improve/fpsServer/api/RouterRegister.cpp -o CMakeFiles/MyProject.dir/api/RouterRegister.cpp.s

# Object files for target MyProject
MyProject_OBJECTS = \
"CMakeFiles/MyProject.dir/server.cpp.o" \
"CMakeFiles/MyProject.dir/MsgConn.cpp.o" \
"CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o" \
"CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o"

# External object files for target MyProject
MyProject_EXTERNAL_OBJECTS =

MyProject: CMakeFiles/MyProject.dir/server.cpp.o
MyProject: CMakeFiles/MyProject.dir/MsgConn.cpp.o
MyProject: CMakeFiles/MyProject.dir/api/MoveHandle.cpp.o
MyProject: CMakeFiles/MyProject.dir/api/RouterRegister.cpp.o
MyProject: CMakeFiles/MyProject.dir/build.make
MyProject: base/libbase.a
MyProject: CMakeFiles/MyProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jt/project-improve/fpsServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable MyProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyProject.dir/build: MyProject
.PHONY : CMakeFiles/MyProject.dir/build

CMakeFiles/MyProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyProject.dir/clean

CMakeFiles/MyProject.dir/depend:
	cd /home/jt/project-improve/fpsServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jt/project-improve/fpsServer /home/jt/project-improve/fpsServer /home/jt/project-improve/fpsServer/build /home/jt/project-improve/fpsServer/build /home/jt/project-improve/fpsServer/build/CMakeFiles/MyProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyProject.dir/depend

