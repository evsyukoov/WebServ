//
// Created by Денис on 13/12/2020.
//
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#include <zconf.h>
#include <unistd.h>
#include "CGI.hpp"
#include "Server.hpp"
# ifdef BONUS
#  include "Worker.hpp"
# endif

void    printConfig(const Config &config)
{
    std::list<ServConf> servConf = config.getConfig();
    std::list<ServConf>::iterator it = servConf.begin();
    std::list<ServConf>::iterator ite = servConf.end();
    size_t i = 0;
    while (it != ite)
    {
        i++;
        std::cout << GREEN << "Server number "  << i << RESET << std::endl;
        std::cout << "   Server name: " << it->getServerName() << std::endl;
        std::cout << "   Port: " << it->getPort() << std::endl;
		std::cout << "   Root: " << it->getRoot() << std::endl;
		std::cout << "   Index: " << it->getIndex() << std::endl;
        std::map<int, std::string> error_pages = it->getErrorPages();
        for(std::map<int, std::string>::iterator iter = error_pages.begin(); iter != error_pages.end(); iter++)
            std::cout << RED << "   error page N " << iter->first << " path " << iter->second << std::endl;
        std::cout << RESET;
        std::list<Location> locations = it->getLocations();
        i = 0;
        for (std::list<Location>::iterator iter = locations.begin(); iter != locations.end(); iter++)
        {
            std::cout << BLUE << "   Location N" << i + 1 << " path: " << (*iter).getLocation() << RESET << std::endl;
            std::cout << "      index: " << (*iter).getIndex() << std::endl;
            std::cout << "      root: " << (*iter).getRoot() << std::endl;
            std::cout << "      cgi extension: " << (*iter).getCgiExtension() << std::endl;
            std::cout << "      cgi scrypt: " << (*iter).getCgiScrypt() << std::endl;
            std::cout << "      cgi index:  " << (*iter).getCgiIndex() << std::endl;
            std::cout << "      max body: " << (*iter).getMaxBody() << std::endl;
            std::cout << "      autoindex: " << (*iter).isAutoindex() << std::endl;
            std::cout << "      Allowed methods: ";
            std::vector<std::string> methods = (*iter).getMethods();
            for (i = 0; i < methods.size(); i++)
            {
                std::cout << methods[i] << " ";
            }
            std::cout << std::endl;
            i++;
        }
        it++;
    }
}

//--->     ./program  /path/config.conf

int     parseCommandLineArgs(int argc, char **argv, input &in)
{
    if (argc != 2)
        return (error("Bad numbers of arguments, first arg - config"));
    std::string conf_name = std::string(argv[1]);
    if (conf_name.substr(conf_name.size() - 5) != ".conf")
        return (error("Bad extension of config file. Expected .conf"));
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0)
        return (error("No such config file"));
    close(fd);
    in.conf = conf_name;
    return (1);
}

int main(int argc, char **argv)
{
    try {
    	signal(SIGPIPE, SIG_IGN);

        struct input in;
        if (parseCommandLineArgs(argc, argv, in) == -1)
            return (0);
        Config conf(in.conf);

        if (conf.readConf() == -1)
            return (0);
        Server server = Server(in, conf);
        server.run();
    }
    catch (std::exception &obj)
    {
        std::cerr<< obj.what() << std::endl;
    }
    return (1);
}


