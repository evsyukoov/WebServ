//
// Created by Денис on 19/12/2020.
//

#include "Location.hpp"
#include "../Request_Processing/CGI.hpp"


Location::Location(const std::string &rawLocation) {
    this->raw_location = rawLocation;
    autoindex = false;
    max_body = -1;
}

int Location::parseRaw()
{
    size_t pos;
    pos = raw_location.find('{');
    std::string rawLoc = raw_location.substr(0, pos);
    std::string innerLocation = raw_location.substr(pos + 1);
    if (parseLocation(rawLoc) == -1)
        return (0);

    while ((pos = innerLocation.find(';')) != std::string::npos)
    {
        std::string directive = innerLocation.substr(0, pos);
        directive = directive.substr(skipSpaces(directive));
        if (isErrorDelimetr(directive) == -1)
            return (-1);
        else
        {
            raw_directives.push_back(directive);
            innerLocation = innerLocation.substr(pos + 1);
        }
    }
    if (!analizeMethods())
        return (0);
    return (1);
}

int     Location::isRightDirective(const std::string &direct, const std::string &method)
{
    int i = 0;
    while (direct[i] == method[i])
        i++;
    if ((!method[i] && direct[i] != ' ') || (method[i]))
        return (0);
    return (i);
}

int     Location::isAllowedMethod(std::string &method)
{
	size_t i = method.size() - 1;
	while (method[i] == ' ' && method[i])
		i--;
	method = method.substr(0, ++i);
    if (method == "GET" || method == "POST" || method == "PUT" || method == "DELETE" || method == "HEAD")
        return (1);
    return (0);
}

int     Location::parseMethods(std::string &directive, int ind)
{
    directive = directive.substr(ind);
    int i = skipSpaces(directive);
    std::string _methods = directive.substr(i);
    size_t pos;
    while ((pos = _methods.find(',')) != std::string::npos)
    {
    	std::string method = _methods.substr(0, pos);
        if (!isAllowedMethod(method))
            return (error("Not allowed method in location block"));;
        methods.push_back(method);
        _methods = _methods.substr(pos + 1);
        i = skipSpaces(_methods);
        _methods = _methods.substr(i);
    }
	if (!isAllowedMethod(_methods))
		return (error("Not allowed method in location block"));;
	methods.push_back(_methods);
    return (1);
}

int Location::analizeDirective(std::list<std::string> &directive)
{
    std::list<std::string>::iterator it = directive.begin();
    if (directive.size() != 2)
        return (error("Error in location block"));
    else
    {
        if (*it == "root")
            root = *(++it);
        else if (*it == "cgi")
            cgi_extension = *(++it);
        else if (*it == "index")
            index = *(++it);
        else if (*it == "autoindex")
        {
            if (*(++it) == "on")
                autoindex = true;
            else if (*it == "off")
                autoindex = false;
            else
                return (error("Error in autoindex description"));
        }
        else if (*it == "max_body")
        {
            if (isDigit(*(++it)))
                max_body = std::atoi((*it).c_str());
            else
                return (error("Error in max_body description"));
        }
        else if (*it == "scrypt") {
            if (!ifFileExists(*(++it)))
                return error("No such scrypt on server");
            else
                cgi_scrypt = *it;
        }
        else if (*it == "cgi_index")
            cgi_index = *(++it);
        else
            return (error("Unknown directive in location block!"));
    }
    return (1);
}

int     Location::ifFileExists(const std::string &fileName)
{
    int fd;
    if ((fd = open(fileName.c_str(), O_RDONLY)) < 0)
        return (0);
    close(fd);
    return (1);
}

int     Location::analizeMethods()
{
    for(std::list<std::string>::iterator it = raw_directives.begin(); it != raw_directives.end(); it++)
    {
        int i;
        if ((i = isRightDirective(*it, "limit_except")) != 0) {
            if (parseMethods(*it, i) == -1)
                return (0);
        }
        else
        {
            std::list<std::string> directive = split(*it);
            if (analizeDirective(directive) == -1)
                return (0);
        }
    }
    return (1);
}

int     Location::parse()
{

    for(std::list<std::string>::iterator it = raw_directives.begin(); it != raw_directives.end(); it++)
    {
        std::list<std::string> directives = split(*it);
    }
    return (0);
}

int Location::parseLocation(const std::string &loc) {
    std::list<std::string> loc_list = split(loc);
    if (loc_list.size() != 2)
        return (error("Location error"));
    std::list<std::string>::iterator it = loc_list.begin();
        if (*it == "location")
        {
            if ((*(++it))[0] != '/' || isUnexpectedSymbol(*it, ';'))
                return error("Unexpected symbol in location directive");
            else {
                location = *it;
            }
        }
        else
            return (error("Unknown directive in block name"));
    return (1);
}

const std::string &Location::getLocation() const {
    return location;
}

const std::vector<std::string> &Location::getMethods() const {
    return methods;
}

int Location::getMaxBody() const {
    return max_body;
}

const std::string &Location::getRoot() const {
    return root;
}

const std::string &Location::getIndex() const {
    return index;
}

const std::string &Location::getCgiExtension() const {
    return cgi_extension;
}

const std::string &Location::getCgiScrypt() const {
    return cgi_scrypt;
}

bool Location::isAutoindex() const {
    return autoindex;
}


Location::Location() {}

Location::~Location() {

}

Location &Location::operator=(const Location &other) {
	location = other.location;
	methods = other.methods;
	max_body = other.max_body;
	root = other.root;
	index = other.index;
	cgi_extension = other.cgi_extension;
	interpretator = other.interpretator;
	cgi_scrypt = other.cgi_scrypt;
	autoindex = other.autoindex;
	cgi_index = other.cgi_index;
	return (*this);
}

Location::Location(const Location &other) {
	this->operator=(other);
}

const std::string &Location::getInterpretator() const {
    return interpretator;
}

const std::string &Location::getCgiIndex() const {
    return cgi_index;
}

