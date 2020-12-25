

#ifndef SERV_FILE_HPP
#define SERV_FILE_HPP

#include "HTTP.hpp"

class File
{
private:
	std::string 		file_name;
	std::vector<int>	content_types;
	std::vector<int> 	content_language;
	long 				content_length;

	long contentLength(std::map<std::string, std::string> &reqMap);

	void contentLanguage(std::map<std::string, std::string> &reqMap);

	void contentType(std::map<std::string, std::string> &reqMap);


public:
	File(std::map<std::string, std::string>&);

};


#endif //SERV_FILE_HPP
