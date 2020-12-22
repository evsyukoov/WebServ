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
	if (checkBracketsDirectives() == -1)
	    return (-1);
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
	    if (!servConf.parseRaw())
	        return (-1);
	    config.push_back(servConf);
	}
    return (1);
}

Config::Config() {}

Config::~Config() {

}

Config::Config(const Config &other)
{
	this->operator=(other);
}

Config &Config::operator=(const Config &other){
	this->path_to_conf = other.path_to_conf;
	this->config = other.config;
	return (*this);

}
