//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_CONFIG_HPP
#define SERV_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <list>

#include "ServConf.hpp"

//весь конфиг
class Config {
	std::string 			path_to_conf;
	//на выходе
	std::list<ServConf> 	config;

	//то что считаем
	std::list<std::string>	raw_conf;

	int 	error(std::string msg);

	void 	trim(std::string &s);

	int 	checkBracketsDirectives();

	int     startWith(const std::string &str, const std::string &in);

	int 	checkSeverNames();


public:
	const std::list<ServConf> &getConfig() const;

	Config(const std::string &pathToConf);

	int		 readConf();

	//Coplien's

	Config();

	Config(const Config &config);

	Config		&operator=(const Config &config);

	virtual ~Config();


};


#endif //SERV_CONFIG_HPP
