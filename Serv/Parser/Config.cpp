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
	if (in.is_open())
	{
		while (std::getline(in, line)) {
			//убираем пустые строчки, пробелы в начале и конце, коменты
			if (!line.empty()) {
				trim(line);
				if (line[0] != '#')
					raw_conf.push_back(line);
			}
		}
	}
	else
		return (error("Error opening file!\n"));
	in.close();
	return (1);
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
			std::string::iterator tmp = ite1--;
			s.erase(++ite1);
			it = tmp;
		}
		else
			break ;
	}
}

int Config::check_errors()
{

}

int Config::checkBracketsDirectives() {
	for(std::list<std::string>::iterator it = raw_conf.begin(); it != raw_conf.end(); it++)
	{

	}
}

//int Config::checkBracketsOpen() {
//	int balance = 0;
//	for(std::list<std::string>::iterator it = raw_conf.begin(); it != raw_conf.end(); it++)
//	{
//		std::string curr = (*it);
//		if (curr.find('{'))
//		{
//			if (curr.size() == 1)
//				balance++;
//		}
//		else if (curr.find('}'))
//		{
//			if (curr.size() == 1)
//				balance--;
//		}
//	}
//}
