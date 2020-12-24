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
		std::cout << "   Root: " << it->getRoot() << std::endl;
		std::cout << "   Index: " << it->getIndex() << std::endl;
        std::map<int, std::string> error_pages = it->getErrorPages();
        for(std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
            std::cout << RED << "   error page N " << it->first << " path " << it->second << std::endl;
        std::cout << RESET;
        std::list<Location> locations = it->getLocations();
        int i = 0;
        for (std::list<Location>::iterator it = locations.begin(); it != locations.end(); it++)
        {
            std::cout << BLUE << "   Location N" << i + 1 << " path: " << (*it).getLocation() << RESET << std::endl;
            std::cout << "      index: " << (*it).getIndex() << std::endl;
            std::cout << "      root: " << (*it).getRoot() << std::endl;
            std::cout << "      cgi extension: " << (*it).getCgiExtension() << std::endl;
            std::cout << "      cgi scrypt: " << (*it).getCgiScrypt() << std::endl;
            std::cout << "      max body: " << (*it).getMaxBody() << std::endl;
            std::cout << "      autoindex: " << (*it).isAutoindex() << std::endl;
            std::cout << "      upload: " << (*it).isEnableUpload() << std::endl;
            if (!(*it).getUploadPath().empty())
                std::cout << "      upload path: " << (*it).isEnableUpload() << std::endl;
            std::cout << "      Allowed methods: ";
            std::vector<std::string> methods = (*it).getMethods();
            for (int i = 0; i < methods.size(); i++)
            {
                std::cout << methods[i] << " ";
            }
            std::cout << std::endl;
            i++;
        }
        it++;
    }
}

int main(int argc, char **argv)
{
    //для запуска закоментить конфиг
	Config conf("Config.txt");
	if (conf.readConf() == -1)
	    return (0);
    Server server = Server(conf);
    server.run();
    return (1);
}


