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
CMAKE_SOURCE_DIR = /home/jt/my_im/server/src/http_msg_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jt/my_im/server/src/http_msg_server/build

# Include any dependencies generated for this target.
include CMakeFiles/http_msg_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/http_msg_server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/http_msg_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/http_msg_server.dir/flags.make

CMakeFiles/http_msg_server.dir/AttachData.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/AttachData.cpp.o: ../AttachData.cpp
CMakeFiles/http_msg_server.dir/AttachData.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/http_msg_server.dir/AttachData.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/AttachData.cpp.o -MF CMakeFiles/http_msg_server.dir/AttachData.cpp.o.d -o CMakeFiles/http_msg_server.dir/AttachData.cpp.o -c /home/jt/my_im/server/src/http_msg_server/AttachData.cpp

CMakeFiles/http_msg_server.dir/AttachData.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/AttachData.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/AttachData.cpp > CMakeFiles/http_msg_server.dir/AttachData.cpp.i

CMakeFiles/http_msg_server.dir/AttachData.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/AttachData.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/AttachData.cpp -o CMakeFiles/http_msg_server.dir/AttachData.cpp.s

CMakeFiles/http_msg_server.dir/DBServConn.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/DBServConn.cpp.o: ../DBServConn.cpp
CMakeFiles/http_msg_server.dir/DBServConn.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/http_msg_server.dir/DBServConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/DBServConn.cpp.o -MF CMakeFiles/http_msg_server.dir/DBServConn.cpp.o.d -o CMakeFiles/http_msg_server.dir/DBServConn.cpp.o -c /home/jt/my_im/server/src/http_msg_server/DBServConn.cpp

CMakeFiles/http_msg_server.dir/DBServConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/DBServConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/DBServConn.cpp > CMakeFiles/http_msg_server.dir/DBServConn.cpp.i

CMakeFiles/http_msg_server.dir/DBServConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/DBServConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/DBServConn.cpp -o CMakeFiles/http_msg_server.dir/DBServConn.cpp.s

CMakeFiles/http_msg_server.dir/HttpConn.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/HttpConn.cpp.o: ../HttpConn.cpp
CMakeFiles/http_msg_server.dir/HttpConn.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/http_msg_server.dir/HttpConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/HttpConn.cpp.o -MF CMakeFiles/http_msg_server.dir/HttpConn.cpp.o.d -o CMakeFiles/http_msg_server.dir/HttpConn.cpp.o -c /home/jt/my_im/server/src/http_msg_server/HttpConn.cpp

CMakeFiles/http_msg_server.dir/HttpConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/HttpConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/HttpConn.cpp > CMakeFiles/http_msg_server.dir/HttpConn.cpp.i

CMakeFiles/http_msg_server.dir/HttpConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/HttpConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/HttpConn.cpp -o CMakeFiles/http_msg_server.dir/HttpConn.cpp.s

CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o: ../HttpPdu.cpp
CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o -MF CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o.d -o CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o -c /home/jt/my_im/server/src/http_msg_server/HttpPdu.cpp

CMakeFiles/http_msg_server.dir/HttpPdu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/HttpPdu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/HttpPdu.cpp > CMakeFiles/http_msg_server.dir/HttpPdu.cpp.i

CMakeFiles/http_msg_server.dir/HttpPdu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/HttpPdu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/HttpPdu.cpp -o CMakeFiles/http_msg_server.dir/HttpPdu.cpp.s

CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o: ../HttpQuery.cpp
CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o -MF CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o.d -o CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o -c /home/jt/my_im/server/src/http_msg_server/HttpQuery.cpp

CMakeFiles/http_msg_server.dir/HttpQuery.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/HttpQuery.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/HttpQuery.cpp > CMakeFiles/http_msg_server.dir/HttpQuery.cpp.i

CMakeFiles/http_msg_server.dir/HttpQuery.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/HttpQuery.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/HttpQuery.cpp -o CMakeFiles/http_msg_server.dir/HttpQuery.cpp.s

CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o: ../RouteServConn.cpp
CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o -MF CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o.d -o CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o -c /home/jt/my_im/server/src/http_msg_server/RouteServConn.cpp

CMakeFiles/http_msg_server.dir/RouteServConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/RouteServConn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/RouteServConn.cpp > CMakeFiles/http_msg_server.dir/RouteServConn.cpp.i

CMakeFiles/http_msg_server.dir/RouteServConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/RouteServConn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/RouteServConn.cpp -o CMakeFiles/http_msg_server.dir/RouteServConn.cpp.s

CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o: CMakeFiles/http_msg_server.dir/flags.make
CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o: ../http_msg_server.cpp
CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o: CMakeFiles/http_msg_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o -MF CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o.d -o CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o -c /home/jt/my_im/server/src/http_msg_server/http_msg_server.cpp

CMakeFiles/http_msg_server.dir/http_msg_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http_msg_server.dir/http_msg_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/http_msg_server/http_msg_server.cpp > CMakeFiles/http_msg_server.dir/http_msg_server.cpp.i

CMakeFiles/http_msg_server.dir/http_msg_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http_msg_server.dir/http_msg_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/http_msg_server/http_msg_server.cpp -o CMakeFiles/http_msg_server.dir/http_msg_server.cpp.s

# Object files for target http_msg_server
http_msg_server_OBJECTS = \
"CMakeFiles/http_msg_server.dir/AttachData.cpp.o" \
"CMakeFiles/http_msg_server.dir/DBServConn.cpp.o" \
"CMakeFiles/http_msg_server.dir/HttpConn.cpp.o" \
"CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o" \
"CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o" \
"CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o" \
"CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o"

# External object files for target http_msg_server
http_msg_server_EXTERNAL_OBJECTS =

http_msg_server: CMakeFiles/http_msg_server.dir/AttachData.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/DBServConn.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/HttpConn.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/HttpPdu.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/HttpQuery.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/RouteServConn.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/http_msg_server.cpp.o
http_msg_server: CMakeFiles/http_msg_server.dir/build.make
http_msg_server: CMakeFiles/http_msg_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jt/my_im/server/src/http_msg_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable http_msg_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http_msg_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/http_msg_server.dir/build: http_msg_server
.PHONY : CMakeFiles/http_msg_server.dir/build

CMakeFiles/http_msg_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/http_msg_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/http_msg_server.dir/clean

CMakeFiles/http_msg_server.dir/depend:
	cd /home/jt/my_im/server/src/http_msg_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jt/my_im/server/src/http_msg_server /home/jt/my_im/server/src/http_msg_server /home/jt/my_im/server/src/http_msg_server/build /home/jt/my_im/server/src/http_msg_server/build /home/jt/my_im/server/src/http_msg_server/build/CMakeFiles/http_msg_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/http_msg_server.dir/depend

