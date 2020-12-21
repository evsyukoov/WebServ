//
// Created by Денис on 19/12/2020.
//

#include <sstream>
#include "ServConf.hpp"

ServConf::ServConf(const std::string &rawServconf, int pos) : raw_servconf(rawServconf), pos(pos) {
    port = -1;
}

int     ServConf::isLocationBlock(const std::string &directive)
{
    size_t index = directive.find(' ');
    if (index == std::string::npos)
        return (error("Error location directive"));
    std::string s = directive.substr(0, index);
    if (s == "location")
        return (1);
    return (0);
}

int    ServConf::parseRaw()
{
    std::string servconf = raw_servconf.substr(pos + 1);
    size_t pos_delim;
    while ((pos_delim = servconf.find(';')) != std::string::npos)
    {
        std::string directive = servconf.substr(0, pos_delim);
        if (isErrorDelimetr(directive) == -1)
            return (0);
        else if (isLocationBlock(directive))
        {
            int close_bracket_index = servconf.find('}');
            std::string location_block = servconf.substr(0, close_bracket_index);
            servconf = servconf.substr(close_bracket_index + 1);
            Location location(location_block);
            if (!location.parseRaw())
                return (0);
            locations.push_back(location);
        }
        else if (isLocationBlock(directive) == -1)
            return (0);
        else
        {
            raw_directives.push_back(directive);
            servconf = servconf.substr(pos_delim + 1);
        }
    }
    if (parse())
        return (1);
    return (0);
}

int    ServConf::analizeDirective(std::list<std::string> line)
{
    std::list<std::string>::iterator str = line.begin();
    if (*str == "server_name" || *str == "listen")
    {
        if (line.size() != 2)
            return (error("Bad directive in Server Block"));
        else
        {
            if (*str == "server_name" && !server_name.empty()) {
                std::cout << *str << std::endl;
                return (error("Dublicate directive in Server Block"));
            }
            if (*str == "server_name" && server_name.empty())
            {
                if (*(++str) == "localhost")
                    server_name = "127.0.0.1";
                else
                    server_name = *str;
            }
            if (*str == "listen" && port != -1)
                return (error("Dublicate directive in Server Block"));
            if (*str == "listen" && isDigit(*(++str)) && port == -1)
                port = std::atoi((*(str)).c_str());
        }
    }
    else if (*str == "default_error_page")
    {
        if (line.size() != 3)
            return (error("Bad directive in Server Block(error pages)"));
        else
        {
            if (isDigit(*(++str))) {
                int error = std::atoi((*(str)).c_str());
                error_pages[error] = *(++str);
            }
            else
                return (error("Bad number of error page"));
        }
    }
    return (1);
}

int    ServConf::parse()
{
    for(std::list<std::string>::iterator it = raw_directives.begin(); it != raw_directives.end(); it++)
    {
        std::list<std::string> splitted = split(*it);
        if (analizeDirective(splitted) == -1)
            return (-1);
    }
    return (1);
}

const std::string &ServConf::getServerName() const {
    return server_name;
}

int ServConf::getPort() const {
    return port;
}

const std::map<int, std::string> &ServConf::getErrorPages() const {
    return error_pages;
}

const std::vector<Location> &ServConf::getLocations() const {
    return locations;
}

