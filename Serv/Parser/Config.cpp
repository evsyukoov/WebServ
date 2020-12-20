//
// Created by Денис on 19/12/2020.
//


#include "Config.hpp"

const std::list<ServConf> &Config::getConfig() const {
	return config;
}

Config::Config(const std::string &pathToConf) {
	this->path_to_conf = pathToConf;
}

int Config::readConf()
{
	std::ifstream in(path_to_conf);
	std::string line;
	std::string server;
	int i = 0;

	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			if (!line.empty()) {
                trim(line);
                //каждая директива сервер в отдельной строчке
                if (line[0] != '#') {
                    if (startWith(line, "server"))
                        i++;
                    if (i == 2) {
                        raw_conf.push_back(server);
                        server = "";
                        i = 1;
                    }
                    server += line;
                }
            }
		}
		raw_conf.push_back(server);
	}
	else
		return (error("Error opening file!\n"));

	in.close();
	checkBracketsDirectives();
	return (1);
}

int     Config::startWith(const std::string &str, const std::string &in)
{
    int i = 0;
    if (in == str)
        return (1);
    while (str[i] == in[i])
        i++;
    if (!in[i] && (str[i] == ' ' || str[i] == '{')){
        return (1);
    }
    return (0);
}

int Config::error(std::string msg) {
	std::cout << "Config error: " << msg << std::endl;
	return (-1);
}

const std::list<std::string> &Config::getRawConf() const {
	return raw_conf;
}


void Config::trim(std::string &s) {
	std::string::iterator it = s.begin();
	std::string::iterator ite = s.end();
	while (it != ite)
	{
		if (std::isspace(*it))
			it = s.erase(it);
		else
			break ;
	}
	std::string::iterator it1 = s.begin();
	std::string::iterator ite1 = --s.end();
	while (ite1 != it1)
	{
		if (std::isspace(*ite1))
		{
			ite1 = s.erase(ite1);
			ite1--;
		}
		else
			break ;
	}
}



int Config::checkBracketsDirectives() {
	for(std::list<std::string>::iterator it = raw_conf.begin(); it != raw_conf.end(); it++)
	{
	    int bracketsBalance = 0;
	    size_t posOpenBracket = (*it).find('{');
	    if (posOpenBracket == std::string::npos)
            return (error("Brackets error"));
	    bracketsBalance++;
	    std::string serv_directive = (*it).substr(0, posOpenBracket);
	    trim(serv_directive);
	    if (serv_directive != "server"){
            return error("Unknown server directive");
        }
	    //проверим баланс открывающих и закрывающих скобок
	    else
        {
            for (int i = posOpenBracket + 1; i < (*it). size(); i++)
            {
                if ((*it)[i] == '{')
                    bracketsBalance++;
                if ((*it)[i] == '}')
                    bracketsBalance--;
                if (bracketsBalance < 0)
                    return (error("Brackets error"));
            }
            if (bracketsBalance != 0)
                return error("Brackets error");

        }
	    //все хорошо - дальнейшая обработка в классе ServConf
	    ServConf servConf((*it), posOpenBracket);
	    servConf.parseRaw();
	    std::cout << "server name " << servConf.getServerName() << std::endl;
	    std::cout << "port " << servConf.getPort() << std::endl;
        std::map<int, std::string> errors = servConf.getErrorPages();
        std::cout << "Error pages: " << std::endl;
	    for (std::map<int, std::string>::iterator i = errors.begin(); i != errors.end(); i++)
        {
	        std::cout << i->first << ": " << i->second << std::endl;
        }
        std::cout << "Locations: " << std::endl;
	    std::vector<Location> locations = servConf.getLocations();
	    for (int i = 0;i < locations.size(); i++)
        {
	        std::cout << "loc  " << i << " " << locations[i].getLocation() << std::endl;
        }

	}
    return (1);
}
