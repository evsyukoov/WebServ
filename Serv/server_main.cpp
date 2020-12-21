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
#include "Parser/Config.hpp"

void    printCongig(const Config &config)
{
    std::list<ServConf> servConf = config.getConfig();
    std::list<ServConf>::iterator it = servConf.begin();
    std::list<ServConf>::iterator ite = servConf.end();
    int i = 0;
    while (it != ite)
    {
        i++;
        std::cout << GREEN << "Server number "  << i << RESET << std::endl;
        std::cout << "   Server name: " << it->getServerName() << std::endl;
        std::cout << "   Port: " << it->getPort() << std::endl;
        std::map<int, std::string> error_pages = it->getErrorPages();
        for(std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
            std::cout << RED << "   error page N " << it->first << " path " << it->second << std::endl;
        std::cout << RESET;
        std::vector<Location> locations = it->getLocations();
        for (int i = 0; i < locations.size(); i++)
        {
            std::cout << BLUE << "   Location N" << i + 1 << " path: " << locations[i].getLocation() << RESET << std::endl;
            std::cout << "      index: " << locations[i].getIndex() << std::endl;
            std::cout << "      root: " << locations[i].getRoot() << std::endl;
            std::cout << "      cgi extension: " << locations[i].getCgiExtension() << std::endl;
            std::cout << "      cgi scrypt: " << locations[i].getCgiScrypt() << std::endl;
            std::cout << "      max body: " << locations[i].getMaxBody() << std::endl;
            std::cout << "      autoindex: " << locations[i].isAutoindex() << std::endl;
            std::cout << "      upload: " << locations[i].isEnableUpload() << std::endl;
            if (!locations[i].getUploadPath().empty())
                std::cout << "      upload path: " << locations[i].isEnableUpload() << std::endl;
            std::cout << "      Allowed methods: ";
            std::vector<std::string> methods = locations[i].getMethods();
            for (int i = 0; i < methods.size(); i++)
            {
                std::cout << methods[i] << " ";
            }
            std::cout << std::endl;
        }
        it++;
    }
}

int main(int argc, char **argv)
{
    //для запуска закоментить конфиг
	Config conf("Config.txt");
	conf.readConf();
    printCongig(conf);
    // это все из конфига будет браться, надо отдельный класс ServConf
//    char buff1[16] = "127.0.0.1:8000";
//    char buff2[16] = "127.0.0.1:1234";
//    char buff3[16] = "127.0.0.1:3456";
//    Net net1(buff1);
//    Net net2(buff2);
//    Net net3(buff3);
//    std::list<Net> listeners;
//    listeners.push_back(net1);
//    listeners.push_back(net2);
//    listeners.push_back(net3);
//    Server server(listeners);
//    server.run();
}


