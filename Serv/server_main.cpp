//
// Created by Денис on 13/12/2020.
//
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#include <zconf.h>
#include <unistd.h>
#include <dirent.h>
//#include <list>
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

//--->     ./program  /path/CGI_scrypt  /path/config.conf
//                             или
//-->      ./program  /path/interpretator  /path/CGI_scrypt    /path/config.conf

int     parseCommandLineArgs(int argc, char **argv, input &in)
{
    if (argc < 3)
        return (error("Bad number of arguments"));
    std::string conf = std::string(argv[argc - 1]);
    if (!(conf.substr(conf.size() - 5) == ".conf"))
        return (error("Bad extension of config file"));
    if (argc == 3)
    {
        in.scrypt = std::string(argv[1]);
        in.conf = std::string(argv[2]);
    }
    if (argc == 4)
    {
        in.interptretator = std::string(argv[1]);
        in.scrypt = std::string(argv[2]);
        in.conf = std::string(argv[3]);
    }
    return (1);
}

int main(int argc, char **argv, char **env)
{
    struct input in;
    if (parseCommandLineArgs(argc, argv, in) == -1)
        return (0);
    Config conf(in.conf);
    if (conf.readConf() == -1)
        return (0);
    std::list<ServConf> lst = conf.getConfig();
    std::list<ServConf>::iterator it = lst.begin();
    //char buff[60] = "./CGI_Scrypts/cgi_tester";
    Server server = Server(in, conf);
    server.run();
    return (1);
}


