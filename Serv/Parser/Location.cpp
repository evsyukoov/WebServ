//
// Created by Денис on 19/12/2020.
//

#include "Location.hpp"


Location::Location(const std::string &rawLocation) {
    this->raw_location = rawLocation;
}

int Location::parseRaw()
{
    size_t pos;
    pos = raw_location.find('{');
    std::string rawLoc = raw_location.substr(0, pos);
    std::string innerLocation = raw_location.substr(pos + 1);
    if (parseLocation(rawLoc) == -1)
        return (0);

    std::list<std::string>  raw_directives;

    while ((pos = innerLocation.find(';')) != std::string::npos)
    {

    }
    return (1);
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


