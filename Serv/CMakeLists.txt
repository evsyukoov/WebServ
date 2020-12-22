cmake_minimum_required(VERSION 3.16)
project(Serv)

set(CMAKE_CXX_STANDARD 98)
#link_directories(~/Denis/WebServ/Serv/libft/)

add_executable(Serv server_main.cpp HttpRequest.cpp HttpResponse.cpp CGI.cpp Server.cpp
        Parser/ServConf.cpp
        Parser/Location.cpp
        Parser/Config.cpp Parser/parser_utils.cpp Parser/parser_utils.hpp)

#target_link_libraries(Serv ft)