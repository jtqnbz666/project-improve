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
CMAKE_SOURCE_DIR = /home/jt/my_im/server/src/test/test_pool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jt/my_im/server/src/test/test_pool/build

# Include any dependencies generated for this target.
include CMakeFiles/pool_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pool_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pool_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pool_test.dir/flags.make

CMakeFiles/pool_test.dir/CachePool.cpp.o: CMakeFiles/pool_test.dir/flags.make
CMakeFiles/pool_test.dir/CachePool.cpp.o: ../CachePool.cpp
CMakeFiles/pool_test.dir/CachePool.cpp.o: CMakeFiles/pool_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pool_test.dir/CachePool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pool_test.dir/CachePool.cpp.o -MF CMakeFiles/pool_test.dir/CachePool.cpp.o.d -o CMakeFiles/pool_test.dir/CachePool.cpp.o -c /home/jt/my_im/server/src/test/test_pool/CachePool.cpp

CMakeFiles/pool_test.dir/CachePool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pool_test.dir/CachePool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pool/CachePool.cpp > CMakeFiles/pool_test.dir/CachePool.cpp.i

CMakeFiles/pool_test.dir/CachePool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pool_test.dir/CachePool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pool/CachePool.cpp -o CMakeFiles/pool_test.dir/CachePool.cpp.s

CMakeFiles/pool_test.dir/DBPool.cpp.o: CMakeFiles/pool_test.dir/flags.make
CMakeFiles/pool_test.dir/DBPool.cpp.o: ../DBPool.cpp
CMakeFiles/pool_test.dir/DBPool.cpp.o: CMakeFiles/pool_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pool_test.dir/DBPool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pool_test.dir/DBPool.cpp.o -MF CMakeFiles/pool_test.dir/DBPool.cpp.o.d -o CMakeFiles/pool_test.dir/DBPool.cpp.o -c /home/jt/my_im/server/src/test/test_pool/DBPool.cpp

CMakeFiles/pool_test.dir/DBPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pool_test.dir/DBPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pool/DBPool.cpp > CMakeFiles/pool_test.dir/DBPool.cpp.i

CMakeFiles/pool_test.dir/DBPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pool_test.dir/DBPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pool/DBPool.cpp -o CMakeFiles/pool_test.dir/DBPool.cpp.s

CMakeFiles/pool_test.dir/test_dbpool.cpp.o: CMakeFiles/pool_test.dir/flags.make
CMakeFiles/pool_test.dir/test_dbpool.cpp.o: ../test_dbpool.cpp
CMakeFiles/pool_test.dir/test_dbpool.cpp.o: CMakeFiles/pool_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jt/my_im/server/src/test/test_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pool_test.dir/test_dbpool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pool_test.dir/test_dbpool.cpp.o -MF CMakeFiles/pool_test.dir/test_dbpool.cpp.o.d -o CMakeFiles/pool_test.dir/test_dbpool.cpp.o -c /home/jt/my_im/server/src/test/test_pool/test_dbpool.cpp

CMakeFiles/pool_test.dir/test_dbpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pool_test.dir/test_dbpool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jt/my_im/server/src/test/test_pool/test_dbpool.cpp > CMakeFiles/pool_test.dir/test_dbpool.cpp.i

CMakeFiles/pool_test.dir/test_dbpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pool_test.dir/test_dbpool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jt/my_im/server/src/test/test_pool/test_dbpool.cpp -o CMakeFiles/pool_test.dir/test_dbpool.cpp.s

# Object files for target pool_test
pool_test_OBJECTS = \
"CMakeFiles/pool_test.dir/CachePool.cpp.o" \
"CMakeFiles/pool_test.dir/DBPool.cpp.o" \
"CMakeFiles/pool_test.dir/test_dbpool.cpp.o"

# External object files for target pool_test
pool_test_EXTERNAL_OBJECTS =

pool_test: CMakeFiles/pool_test.dir/CachePool.cpp.o
pool_test: CMakeFiles/pool_test.dir/DBPool.cpp.o
pool_test: CMakeFiles/pool_test.dir/test_dbpool.cpp.o
pool_test: CMakeFiles/pool_test.dir/build.make
pool_test: CMakeFiles/pool_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jt/my_im/server/src/test/test_pool/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable pool_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pool_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pool_test.dir/build: pool_test
.PHONY : CMakeFiles/pool_test.dir/build

CMakeFiles/pool_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pool_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pool_test.dir/clean

CMakeFiles/pool_test.dir/depend:
	cd /home/jt/my_im/server/src/test/test_pool/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jt/my_im/server/src/test/test_pool /home/jt/my_im/server/src/test/test_pool /home/jt/my_im/server/src/test/test_pool/build /home/jt/my_im/server/src/test/test_pool/build /home/jt/my_im/server/src/test/test_pool/build/CMakeFiles/pool_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pool_test.dir/depend

