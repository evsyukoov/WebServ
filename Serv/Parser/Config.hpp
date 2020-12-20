//
// Created by Денис on 19/12/2020.
//

#ifndef SERV_CONFIG_HPP
#define SERV_CONFIG_HPP
#include "ServConf.hpp"
#include <iostream>
#include <fstream>

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

	int 	checkBracketsOpen();

    void    modifyConfig();

    int     startWith(const std::string &str, const std::string &in);



public:
	const std::list<ServConf> &getConfig() const;

	const std::list<std::string> &getRawConf() const;

	Config(const std::string &pathToConf);

	int		 readConf();

	int 	 check_errors();


};


#endif //SERV_CONFIG_HPP
