
#ifndef GIT_SERV_HTTP_HPP
#define GIT_SERV_HTTP_HPP
#define YELLOW  "\033[1;33m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define AC_LANG "Accept-Language"
#define AC_CHARSET "Accept-Charset"
#define ALLOW "Allow"

#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>
#include "Parser/Config.hpp"
#include <unistd.h>
#include <limits>
#include <sys/socket.h>
#include "File.hpp"
#include "utils.hpp"
#include "CGI.hpp"

class HTTP
{
private:

	ServConf		servConf;
	std::string 	buff_req;
	std::map<std::string, std::string>	reqMap;  //map запроса включает в себя все заголовки
	std::list<Location>::const_iterator it;
	std::string 	result;
	int				client_fd;
	struct input    in;
	std::vector<File> files;
	std::map<std::string, std::string> respMap; //map заголовков ответа

	//сформированная страничка со списком директорий для автоиндекса
	std::string listing;

	void 	get();

	void 	post();

	void 	put();

	bool checkForAllowedMethod();

	bool postPutvalidation(std::string &put_post_root, File &file);

	bool locationMatch(const std::string& location);

	std::string pathFormerer();

	int checkDirectory(const std::string& root);

	std::string rootSwitcher(const std::string& root, const std::string& serv_index, const std::string& loc_index);

	int sendReq(std::string header, std::string request);

	void readFile(int file_size, int fd);

	std::list<Location>::const_iterator getMatchingLocation();

	long contentLength();

	bool validateExtencion(std::string &root);

	int 	initMap();

	bool validateMethod();

	bool validateProtocol();

	void 	locationToRootReplcaer(std::string& root_with_slash);

	void	fill_cgi(t_cgi *cgi, File &file, std::string &root);

	std::string postRoot();

	bool validateRequestLine();

	bool parceRequestLine(size_t &second_pos, size_t &rev_pos);

	bool validateHeaderMap();

	bool postGet();

	int		initListingHTML(const std::string &path);

	bool	putInPriorMap(std::map<std::string, float>& prior_map, std::string lang);

	bool	priorityValidation(std::string prior);

	bool	accepts(std::map<std::string, float>& prior_map, std::string base);

	void	printVec(std::vector<std::string> vector);

	std::vector<std::string>	passMap(std::map<std::string, float> accept);

	std::string					searchForMatchingLanguage(std::map<std::string, float> accepts, std::string& path);

	bool	compareContentLanguage(std::vector<File>::iterator matching_file, std::string language);

	void	rewriteFileToVector(File &file);

	bool	checkMatchingLanguage(std::string matching_language);

	std::string		responceMapToString();

	std::string		makeAllow(std::string exept);

public:

	HTTP(int client, char *buf, const ServConf&);

	HTTP(); // дефолтный конструктор, не инициализирует ничего

	void setFields(int client, char *buf, const ServConf &serv, struct input&); // функция инициализации полей для дальнейшей обработки

	std::string &getResponce();

	const std::map<std::string, std::string> &getRequestMap() const; // для отладки

	void 	printMap();  //для отладки

	void 	manager();

};


#endif //GIT_SERV_HTTP_HPP
