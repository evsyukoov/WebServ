# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ccarl/Desktop/WebServ/Serv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Serv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Serv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Serv.dir/flags.make

CMakeFiles/Serv.dir/server_main.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/server_main.cpp.o: ../server_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Serv.dir/server_main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/server_main.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/server_main.cpp

CMakeFiles/Serv.dir/server_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/server_main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/server_main.cpp > CMakeFiles/Serv.dir/server_main.cpp.i

CMakeFiles/Serv.dir/server_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/server_main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/server_main.cpp -o CMakeFiles/Serv.dir/server_main.cpp.s

CMakeFiles/Serv.dir/HTTP.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/HTTP.cpp.o: ../HTTP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Serv.dir/HTTP.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/HTTP.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/HTTP.cpp

CMakeFiles/Serv.dir/HTTP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/HTTP.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/HTTP.cpp > CMakeFiles/Serv.dir/HTTP.cpp.i

CMakeFiles/Serv.dir/HTTP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/HTTP.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/HTTP.cpp -o CMakeFiles/Serv.dir/HTTP.cpp.s

CMakeFiles/Serv.dir/CGI.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/CGI.cpp.o: ../CGI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Serv.dir/CGI.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/CGI.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/CGI.cpp

CMakeFiles/Serv.dir/CGI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/CGI.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/CGI.cpp > CMakeFiles/Serv.dir/CGI.cpp.i

CMakeFiles/Serv.dir/CGI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/CGI.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/CGI.cpp -o CMakeFiles/Serv.dir/CGI.cpp.s

CMakeFiles/Serv.dir/Server.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Serv.dir/Server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Server.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/Server.cpp

CMakeFiles/Serv.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/Server.cpp > CMakeFiles/Serv.dir/Server.cpp.i

CMakeFiles/Serv.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/Server.cpp -o CMakeFiles/Serv.dir/Server.cpp.s

CMakeFiles/Serv.dir/Parser/ServConf.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/ServConf.cpp.o: ../Parser/ServConf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Serv.dir/Parser/ServConf.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/ServConf.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/Parser/ServConf.cpp

CMakeFiles/Serv.dir/Parser/ServConf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/ServConf.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/Parser/ServConf.cpp > CMakeFiles/Serv.dir/Parser/ServConf.cpp.i

CMakeFiles/Serv.dir/Parser/ServConf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/ServConf.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/Parser/ServConf.cpp -o CMakeFiles/Serv.dir/Parser/ServConf.cpp.s

CMakeFiles/Serv.dir/Parser/Location.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/Location.cpp.o: ../Parser/Location.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Serv.dir/Parser/Location.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/Location.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/Parser/Location.cpp

CMakeFiles/Serv.dir/Parser/Location.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/Location.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/Parser/Location.cpp > CMakeFiles/Serv.dir/Parser/Location.cpp.i

CMakeFiles/Serv.dir/Parser/Location.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/Location.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/Parser/Location.cpp -o CMakeFiles/Serv.dir/Parser/Location.cpp.s

CMakeFiles/Serv.dir/Parser/Config.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/Config.cpp.o: ../Parser/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Serv.dir/Parser/Config.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/Config.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/Parser/Config.cpp

CMakeFiles/Serv.dir/Parser/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/Config.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/Parser/Config.cpp > CMakeFiles/Serv.dir/Parser/Config.cpp.i

CMakeFiles/Serv.dir/Parser/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/Config.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/Parser/Config.cpp -o CMakeFiles/Serv.dir/Parser/Config.cpp.s

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o: ../Parser/parser_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o -c /Users/ccarl/Desktop/WebServ/Serv/Parser/parser_utils.cpp

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ccarl/Desktop/WebServ/Serv/Parser/parser_utils.cpp > CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ccarl/Desktop/WebServ/Serv/Parser/parser_utils.cpp -o CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s

# Object files for target Serv
Serv_OBJECTS = \
"CMakeFiles/Serv.dir/server_main.cpp.o" \
"CMakeFiles/Serv.dir/HTTP.cpp.o" \
"CMakeFiles/Serv.dir/CGI.cpp.o" \
"CMakeFiles/Serv.dir/Server.cpp.o" \
"CMakeFiles/Serv.dir/Parser/ServConf.cpp.o" \
"CMakeFiles/Serv.dir/Parser/Location.cpp.o" \
"CMakeFiles/Serv.dir/Parser/Config.cpp.o" \
"CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o"

# External object files for target Serv
Serv_EXTERNAL_OBJECTS =

Serv: CMakeFiles/Serv.dir/server_main.cpp.o
Serv: CMakeFiles/Serv.dir/HTTP.cpp.o
Serv: CMakeFiles/Serv.dir/CGI.cpp.o
Serv: CMakeFiles/Serv.dir/Server.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/ServConf.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/Location.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/Config.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o
Serv: CMakeFiles/Serv.dir/build.make
Serv: CMakeFiles/Serv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable Serv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Serv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Serv.dir/build: Serv

.PHONY : CMakeFiles/Serv.dir/build

CMakeFiles/Serv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Serv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Serv.dir/clean

CMakeFiles/Serv.dir/depend:
	cd /Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ccarl/Desktop/WebServ/Serv /Users/ccarl/Desktop/WebServ/Serv /Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug /Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug /Users/ccarl/Desktop/WebServ/Serv/cmake-build-debug/CMakeFiles/Serv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Serv.dir/depend

