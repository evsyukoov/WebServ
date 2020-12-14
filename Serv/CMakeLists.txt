cmake_minimum_required(VERSION 3.16)
project(Serv)

set(CMAKE_CXX_STANDARD 98)
#link_directories(~/Denis/WebServ/Serv/libft/)

add_executable(Serv Net.cpp server_main.cpp lib.cpp HttpRequest.cpp HttpResponse.cpp)

#target_link_libraries(Serv ft)