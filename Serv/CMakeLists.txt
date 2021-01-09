cmake_minimum_required(VERSION 3.16)
project(Serv)

set(CMAKE_CXX_STANDARD 98)
#link_directories(~/Denis/WebServ/Serv/libft/)

add_executable(Serv server_main.cpp HTTP.cpp CGI.cpp Server.cpp
        Parser/ServConf.cpp
        Parser/Location.cpp
        Parser/Config.cpp
        Parser/parser_utils.cpp
        Parser/parser_utils.hpp
        utils.cpp utils_for_file.cpp
        File.cpp utils.hpp Client.cpp Client.hpp)
#add_executable(Serv test_for_file.cpp HTTP.cpp Server.cpp File.cpp
#        Parser/ServConf.cpp
#        Parser/Location.cpp
#        Parser/Config.cpp Parser/parser_utils.cpp Parser/parser_utils.hpp utils_for_file.cpp)
#target_link_libraries(Serv ft)