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
CMAKE_SOURCE_DIR = /home/jt/my_im/server/src/test/test_pdu/2-im_new

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jt/my_im/server/src/test/test_pdu/2-im_new/build

# Include any dependencies generated for this target.
include CMakeFiles/server_im.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server_im.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server_im.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_im.dir/flags.make

CMakeFiles/server_im.dir/msg.pb.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/msg.pb.cc.o: ../msg.pb.cc
CMakeFiles/server_im.dir/msg.pb.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server_im.dir/msg.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/msg.pb.cc.o -MF CMakeFiles/server_im.dir/msg.pb.cc.o.d -o CMakeFiles/server_im.dir/msg.pb.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/msg.pb.cc

CMakeFiles/server_im.dir/msg.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/msg.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/msg.pb.cc > CMakeFiles/server_im.dir/msg.pb.cc.i

CMakeFiles/server_im.dir/msg.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/msg.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/msg.pb.cc -o CMakeFiles/server_im.dir/msg.pb.cc.s

CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o: ../IM.BaseDefine.pb.cc
CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o -MF CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o.d -o CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.BaseDefine.pb.cc

CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.BaseDefine.pb.cc > CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.i

CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.BaseDefine.pb.cc -o CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.s

CMakeFiles/server_im.dir/IM.Login.pb.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/IM.Login.pb.cc.o: ../IM.Login.pb.cc
CMakeFiles/server_im.dir/IM.Login.pb.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/server_im.dir/IM.Login.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/IM.Login.pb.cc.o -MF CMakeFiles/server_im.dir/IM.Login.pb.cc.o.d -o CMakeFiles/server_im.dir/IM.Login.pb.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Login.pb.cc

CMakeFiles/server_im.dir/IM.Login.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/IM.Login.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Login.pb.cc > CMakeFiles/server_im.dir/IM.Login.pb.cc.i

CMakeFiles/server_im.dir/IM.Login.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/IM.Login.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Login.pb.cc -o CMakeFiles/server_im.dir/IM.Login.pb.cc.s

CMakeFiles/server_im.dir/IM.Message.pb.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/IM.Message.pb.cc.o: ../IM.Message.pb.cc
CMakeFiles/server_im.dir/IM.Message.pb.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/server_im.dir/IM.Message.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/IM.Message.pb.cc.o -MF CMakeFiles/server_im.dir/IM.Message.pb.cc.o.d -o CMakeFiles/server_im.dir/IM.Message.pb.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Message.pb.cc

CMakeFiles/server_im.dir/IM.Message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/IM.Message.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Message.pb.cc > CMakeFiles/server_im.dir/IM.Message.pb.cc.i

CMakeFiles/server_im.dir/IM.Message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/IM.Message.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/IM.Message.pb.cc -o CMakeFiles/server_im.dir/IM.Message.pb.cc.s

CMakeFiles/server_im.dir/UtilPdu.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/UtilPdu.cc.o: ../UtilPdu.cc
CMakeFiles/server_im.dir/UtilPdu.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/server_im.dir/UtilPdu.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/UtilPdu.cc.o -MF CMakeFiles/server_im.dir/UtilPdu.cc.o.d -o CMakeFiles/server_im.dir/UtilPdu.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/UtilPdu.cc

CMakeFiles/server_im.dir/UtilPdu.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/UtilPdu.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/UtilPdu.cc > CMakeFiles/server_im.dir/UtilPdu.cc.i

CMakeFiles/server_im.dir/UtilPdu.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/UtilPdu.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/UtilPdu.cc -o CMakeFiles/server_im.dir/UtilPdu.cc.s

CMakeFiles/server_im.dir/ImPduBase.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/ImPduBase.cc.o: ../ImPduBase.cc
CMakeFiles/server_im.dir/ImPduBase.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/server_im.dir/ImPduBase.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/ImPduBase.cc.o -MF CMakeFiles/server_im.dir/ImPduBase.cc.o.d -o CMakeFiles/server_im.dir/ImPduBase.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/ImPduBase.cc

CMakeFiles/server_im.dir/ImPduBase.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/ImPduBase.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/ImPduBase.cc > CMakeFiles/server_im.dir/ImPduBase.cc.i

CMakeFiles/server_im.dir/ImPduBase.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/ImPduBase.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/ImPduBase.cc -o CMakeFiles/server_im.dir/ImPduBase.cc.s

CMakeFiles/server_im.dir/util.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/util.cc.o: ../util.cc
CMakeFiles/server_im.dir/util.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/server_im.dir/util.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/util.cc.o -MF CMakeFiles/server_im.dir/util.cc.o.d -o CMakeFiles/server_im.dir/util.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/util.cc

CMakeFiles/server_im.dir/util.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/util.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/util.cc > CMakeFiles/server_im.dir/util.cc.i

CMakeFiles/server_im.dir/util.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/util.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/util.cc -o CMakeFiles/server_im.dir/util.cc.s

CMakeFiles/server_im.dir/Lock.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/Lock.cc.o: ../Lock.cc
CMakeFiles/server_im.dir/Lock.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/server_im.dir/Lock.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/Lock.cc.o -MF CMakeFiles/server_im.dir/Lock.cc.o.d -o CMakeFiles/server_im.dir/Lock.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/Lock.cc

CMakeFiles/server_im.dir/Lock.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/Lock.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/Lock.cc > CMakeFiles/server_im.dir/Lock.cc.i

CMakeFiles/server_im.dir/Lock.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/Lock.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/Lock.cc -o CMakeFiles/server_im.dir/Lock.cc.s

CMakeFiles/server_im.dir/server.cc.o: CMakeFiles/server_im.dir/flags.make
CMakeFiles/server_im.dir/server.cc.o: ../server.cc
CMakeFiles/server_im.dir/server.cc.o: CMakeFiles/server_im.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/server_im.dir/server.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server_im.dir/server.cc.o -MF CMakeFiles/server_im.dir/server.cc.o.d -o CMakeFiles/server_im.dir/server.cc.o -c /home/jt/my_im/server/src/test/test_pdu/2-im_new/server.cc

CMakeFiles/server_im.dir/server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_im.dir/server.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pdu/2-im_new/server.cc > CMakeFiles/server_im.dir/server.cc.i

CMakeFiles/server_im.dir/server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_im.dir/server.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pdu/2-im_new/server.cc -o CMakeFiles/server_im.dir/server.cc.s

# Object files for target server_im
server_im_OBJECTS = \
"CMakeFiles/server_im.dir/msg.pb.cc.o" \
"CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o" \
"CMakeFiles/server_im.dir/IM.Login.pb.cc.o" \
"CMakeFiles/server_im.dir/IM.Message.pb.cc.o" \
"CMakeFiles/server_im.dir/UtilPdu.cc.o" \
"CMakeFiles/server_im.dir/ImPduBase.cc.o" \
"CMakeFiles/server_im.dir/util.cc.o" \
"CMakeFiles/server_im.dir/Lock.cc.o" \
"CMakeFiles/server_im.dir/server.cc.o"

# External object files for target server_im
server_im_EXTERNAL_OBJECTS =

server_im: CMakeFiles/server_im.dir/msg.pb.cc.o
server_im: CMakeFiles/server_im.dir/IM.BaseDefine.pb.cc.o
server_im: CMakeFiles/server_im.dir/IM.Login.pb.cc.o
server_im: CMakeFiles/server_im.dir/IM.Message.pb.cc.o
server_im: CMakeFiles/server_im.dir/UtilPdu.cc.o
server_im: CMakeFiles/server_im.dir/ImPduBase.cc.o
server_im: CMakeFiles/server_im.dir/util.cc.o
server_im: CMakeFiles/server_im.dir/Lock.cc.o
server_im: CMakeFiles/server_im.dir/server.cc.o
server_im: CMakeFiles/server_im.dir/build.make
server_im: CMakeFiles/server_im.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable server_im"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_im.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_im.dir/build: server_im
.PHONY : CMakeFiles/server_im.dir/build

CMakeFiles/server_im.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_im.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_im.dir/clean

CMakeFiles/server_im.dir/depend:
	cd /home/jt/my_im/server/src/test/test_pdu/2-im_new/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jt/my_im/server/src/test/test_pdu/2-im_new /home/jt/my_im/server/src/test/test_pdu/2-im_new /home/jt/my_im/server/src/test/test_pdu/2-im_new/build /home/jt/my_im/server/src/test/test_pdu/2-im_new/build /home/jt/my_im/server/src/test/test_pdu/2-im_new/build/CMakeFiles/server_im.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_im.dir/depend

