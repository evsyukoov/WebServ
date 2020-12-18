//
// Created by Денис on 13/12/2020.
//
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#include <zconf.h>
#include <list>
#include "CGI.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
    // это все из конфига будет браться, надо отдельный класс Config
    char buff1[16] = "127.0.0.1:8000";
   char buff2[16] = "127.0.0.1:1234";
    char buff3[16] = "127.0.0.1:3456";
    Net net1(buff1);
    Net net2(buff2);
    Net net3(buff3);
    std::list<Net> listeners;
    listeners.push_back(net1);
    listeners.push_back(net2);
    listeners.push_back(net3);
    Server server(listeners);
    server.run();
}


