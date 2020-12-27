

#ifndef SERV_FILE_HPP
#define SERV_FILE_HPP

#include "HTTP.hpp"
#include "utils_for_file.hpp"
#define LANG "Content-Language"
#define LENGTH "Content-Length"
#define TYPE "Content-Type"
#define ENCODE "Content-Encoding"

class File
{
private:
	std::string 				file_name;
	std::string 				content_type;
	std::string 				charset;
	std::vector<std::string> 	content_language;
	std::vector<std::string>	content_encoding;
	size_t 						content_length;

	long contentLength(std::map<std::string, std::string> &reqMap);

	void contentWithComma(std::map<std::string, std::string> &reqMap, std::string base);

	void contentType(std::map<std::string, std::string> &reqMap);

	bool typeValidity(std::vector<std::string> &charset_vector, std::vector<std::string> &type_vector);

public:
	File(std::map<std::string, std::string>&);

	const long &getContentLength();

	const std::vector<std::string>& getContentLanguage();

	const std::vector<std::string>& getContentEncoding();

	const std::string &getCharset();

	const std::string &getContentType();

	std::string getMime();

};


#endif //SERV_FILE_HPP
