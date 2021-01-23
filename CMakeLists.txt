cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)


add_executable(
        webserv
        main.cpp
        Request_Processing/HTTP.cpp
        Request_Processing/CGI.cpp
        Server/Server.cpp
        Parser/ServConf.cpp
        Parser/Location.cpp
        Parser/Config.cpp
        Parser/parser_utils.cpp
        Parser/parser_utils.hpp
        Utils/utils.cpp Utils/utils_for_file.cpp
        Request-Response/Response.cpp
        Request_Processing/File.cpp
        Utils/utils.hpp
        Request-Response/Client.cpp
        AServer.hpp
)

include_directories(Parser Server Utils Request-Response Request_Processing .)
set(CMAKE_CXX_FLAGS "-std=C++98 -Wall -Wextra -Werror")