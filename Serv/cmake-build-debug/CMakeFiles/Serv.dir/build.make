# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/denis/Desktop/WebServerConf/Serv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Serv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Serv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Serv.dir/flags.make

CMakeFiles/Serv.dir/server_main.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/server_main.cpp.o: ../server_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Serv.dir/server_main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/server_main.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/server_main.cpp

CMakeFiles/Serv.dir/server_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/server_main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/server_main.cpp > CMakeFiles/Serv.dir/server_main.cpp.i

CMakeFiles/Serv.dir/server_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/server_main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/server_main.cpp -o CMakeFiles/Serv.dir/server_main.cpp.s

CMakeFiles/Serv.dir/HttpRequest.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/HttpRequest.cpp.o: ../HttpRequest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Serv.dir/HttpRequest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/HttpRequest.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/HttpRequest.cpp

CMakeFiles/Serv.dir/HttpRequest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/HttpRequest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/HttpRequest.cpp > CMakeFiles/Serv.dir/HttpRequest.cpp.i

CMakeFiles/Serv.dir/HttpRequest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/HttpRequest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/HttpRequest.cpp -o CMakeFiles/Serv.dir/HttpRequest.cpp.s

CMakeFiles/Serv.dir/HttpResponse.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/HttpResponse.cpp.o: ../HttpResponse.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Serv.dir/HttpResponse.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/HttpResponse.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/HttpResponse.cpp

CMakeFiles/Serv.dir/HttpResponse.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/HttpResponse.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/HttpResponse.cpp > CMakeFiles/Serv.dir/HttpResponse.cpp.i

CMakeFiles/Serv.dir/HttpResponse.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/HttpResponse.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/HttpResponse.cpp -o CMakeFiles/Serv.dir/HttpResponse.cpp.s

CMakeFiles/Serv.dir/CGI.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/CGI.cpp.o: ../CGI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Serv.dir/CGI.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/CGI.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/CGI.cpp

CMakeFiles/Serv.dir/CGI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/CGI.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/CGI.cpp > CMakeFiles/Serv.dir/CGI.cpp.i

CMakeFiles/Serv.dir/CGI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/CGI.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/CGI.cpp -o CMakeFiles/Serv.dir/CGI.cpp.s

CMakeFiles/Serv.dir/Server.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Server.cpp.o: ../Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Serv.dir/Server.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Server.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/Server.cpp

CMakeFiles/Serv.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Server.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/Server.cpp > CMakeFiles/Serv.dir/Server.cpp.i

CMakeFiles/Serv.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Server.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/Server.cpp -o CMakeFiles/Serv.dir/Server.cpp.s

CMakeFiles/Serv.dir/Parser/ServConf.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/ServConf.cpp.o: ../Parser/ServConf.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Serv.dir/Parser/ServConf.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/ServConf.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/Parser/ServConf.cpp

CMakeFiles/Serv.dir/Parser/ServConf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/ServConf.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/Parser/ServConf.cpp > CMakeFiles/Serv.dir/Parser/ServConf.cpp.i

CMakeFiles/Serv.dir/Parser/ServConf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/ServConf.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/Parser/ServConf.cpp -o CMakeFiles/Serv.dir/Parser/ServConf.cpp.s

CMakeFiles/Serv.dir/Parser/Location.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/Location.cpp.o: ../Parser/Location.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Serv.dir/Parser/Location.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/Location.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/Parser/Location.cpp

CMakeFiles/Serv.dir/Parser/Location.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/Location.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/Parser/Location.cpp > CMakeFiles/Serv.dir/Parser/Location.cpp.i

CMakeFiles/Serv.dir/Parser/Location.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/Location.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/Parser/Location.cpp -o CMakeFiles/Serv.dir/Parser/Location.cpp.s

CMakeFiles/Serv.dir/Parser/Config.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/Config.cpp.o: ../Parser/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Serv.dir/Parser/Config.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/Config.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/Parser/Config.cpp

CMakeFiles/Serv.dir/Parser/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/Config.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/Parser/Config.cpp > CMakeFiles/Serv.dir/Parser/Config.cpp.i

CMakeFiles/Serv.dir/Parser/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/Config.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/Parser/Config.cpp -o CMakeFiles/Serv.dir/Parser/Config.cpp.s

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o: CMakeFiles/Serv.dir/flags.make
CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o: ../Parser/parser_utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o -c /Users/denis/Desktop/WebServerConf/Serv/Parser/parser_utils.cpp

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/denis/Desktop/WebServerConf/Serv/Parser/parser_utils.cpp > CMakeFiles/Serv.dir/Parser/parser_utils.cpp.i

CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/denis/Desktop/WebServerConf/Serv/Parser/parser_utils.cpp -o CMakeFiles/Serv.dir/Parser/parser_utils.cpp.s

# Object files for target Serv
Serv_OBJECTS = \
"CMakeFiles/Serv.dir/server_main.cpp.o" \
"CMakeFiles/Serv.dir/HttpRequest.cpp.o" \
"CMakeFiles/Serv.dir/HttpResponse.cpp.o" \
"CMakeFiles/Serv.dir/CGI.cpp.o" \
"CMakeFiles/Serv.dir/Server.cpp.o" \
"CMakeFiles/Serv.dir/Parser/ServConf.cpp.o" \
"CMakeFiles/Serv.dir/Parser/Location.cpp.o" \
"CMakeFiles/Serv.dir/Parser/Config.cpp.o" \
"CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o"

# External object files for target Serv
Serv_EXTERNAL_OBJECTS =

Serv: CMakeFiles/Serv.dir/server_main.cpp.o
Serv: CMakeFiles/Serv.dir/HttpRequest.cpp.o
Serv: CMakeFiles/Serv.dir/HttpResponse.cpp.o
Serv: CMakeFiles/Serv.dir/CGI.cpp.o
Serv: CMakeFiles/Serv.dir/Server.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/ServConf.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/Location.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/Config.cpp.o
Serv: CMakeFiles/Serv.dir/Parser/parser_utils.cpp.o
Serv: CMakeFiles/Serv.dir/build.make
Serv: CMakeFiles/Serv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable Serv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Serv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Serv.dir/build: Serv

.PHONY : CMakeFiles/Serv.dir/build

CMakeFiles/Serv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Serv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Serv.dir/clean

CMakeFiles/Serv.dir/depend:
	cd /Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/denis/Desktop/WebServerConf/Serv /Users/denis/Desktop/WebServerConf/Serv /Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug /Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug /Users/denis/Desktop/WebServerConf/Serv/cmake-build-debug/CMakeFiles/Serv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Serv.dir/depend

