
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
#define DATE "Date"
#define SERVER "Server"
#define LAST_MOD "Last-Modified"
#define TRANSFER "Transfer-Encoding"
#define LOCATION "Location"
#define CONT_LOC "Content-Location"
#define HOST "Host"
#define BYTES_TO_WRITE 10000

#include <sys/stat.h>
#include <fcntl.h>
#include "Parser/Config.hpp"
#include <unistd.h>
#include <limits>
#include <sys/socket.h>
#include "File.hpp"
#include "Response.hpp"
#include "utils.hpp"
#include "CGI.hpp"
#include <sys/time.h>
#include <ServConf.hpp>

class HTTP
{

private:

	ServConf		servConf;
	std::string 	buff_req;
	std::map<std::string, std::string>	reqMap;  //map запроса включает в себя все заголовки
	std::list<Location>::const_iterator it; // нахождение нужного location
	std::string 	result;
	int				client_fd;
	struct input    in;
	std::vector<File> files; // вектор отслеживаемых файлов
	std::map<std::string, std::string> respMap; //map заголовков ответа

    std::map<int, std::string> errors; //карта ошибок
	//сформированная страничка со списком директорий для автоиндекса
	std::string listing; // автоиндекс
	Response    *to_send;

	int 	get();

	void 	post(bool post_get_flag = false);

	void 	put();

	bool	checkForAllowedMethod();

	bool	postGetValidation(std::string &root);

	bool	postPutvalidation(std::string &put_post_root, File &file, bool post_flag);

	bool	validateHost();

	bool	postRootConfig(std::string &post_root);

	bool	locationMatch(const std::string& location);

	bool	createNewRepresent(std::string &post_root, File &file);

	std::string pathFormerer();

	//int checkDirectory(const std::string& root);

	std::string rootSwitcher(const std::string& root, const std::string& serv_index, const std::string& loc_index);

	int sendReq(std::string const &header, std::string request);

	void readFile(struct stat &st, int fd, std::string &path);

	std::list<Location>::const_iterator getMatchingLocation();

//	long contentLength();

	bool validateExtencion(std::string &root);

	int 	initMap();

	int		validateMethod();

	bool	validateProtocol();

	void 	locationToRootReplcaer(std::string& root_with_slash);

	void	cgiFiller(File &file, std::string &location);

	std::string postRoot();

	static bool validateRequestLine(std::map<std::string, std::string> &map);

	static bool parceRequestLine(std::map<std::string, std::string> &map, std::string request);

	static bool validateHeaderMap(std::map<std::string, std::string> &map);

	bool postGet();


	void    initErrorMap();

	int		initListingHTML(std::string &path);

	static bool	doubleHostLength(bool &host, bool &name, std::string &header);

	bool	putInPriorMap(std::map<std::string, float>& prior_map, std::string const &lang);

	bool	priorityValidation(std::string prior);

	bool	accepts(std::map<std::string, float>& prior_map, const std::string &base);

	std::vector<std::string>	passMap(std::map<std::string, float> accept);

	std::string searchForMatchingAccept(std::map<std::string, float> &accepts, std::string const &path,
                bool (*func)(std::vector<File>::iterator, std::string const &), std::string const &base);

	static bool	compareContentLanguage(std::vector<File>::iterator matching_file, std::string const &language);

	void	rewriteFileToVector(File &file);

	bool	checkMatchingAccept(std::string const &matching, std::string const &base);

	static bool compareCharset(std::vector<File>::iterator matching_file, std::string const &charset);

	std::string		responceMapToString(int statuscode);

	std::string		makeAllow(std::string const &except = "");

	std::string getMatchingAccept(std::map<std::string, float> accepts, bool (*func)(std::vector<File>::iterator, const std::string &), std::vector<File>::iterator iter);

	static std::string removeAllUnnecessarySlash(std::string path);

	bool		findMethod(std::string const &find);

	void		formContentTypeLength(const std::string &path, ssize_t file_size);

	void		formTime(long long time_sec, std::string const &base);

	void		formRespHeaderOK(std::string &path, struct stat st);

	void		timer();

	std::string	errorPageResponece(int error_num);

	bool		validateTransferEncoding();

	bool 		tryAutoindex(std::string &path);

	void		former(std::string &root);

	static int			x_write(int fd, std::string const &buf, size_t len);
	int			        x_write(std::map<std::string, std::string> respMap);

	bool				putManager(std::string &put_root, File &file, std::string const &uri, int put_flag = 0, const std::string &responce = std::string(""));

	void				hardcodeMap(std::map<std::string, std::string> responseMap);

	static std::map<std::string, std::string>	clear(std::map<std::string, std::string> &map);


public:

	HTTP(int client, char *buf, const ServConf&);

	HTTP(); // дефолтный конструктор, не инициализирует ничего

	void setFields(int client, char const *buf, const ServConf &serv, struct input&); // функция инициализации полей для дальнейшей обработки

	std::string &getResponce();

	static std::map<std::string, std::string> parceMap(std::string &request);

	const std::map<std::string, std::string> &getRequestMap() const; // для отладки

	void 	printMap();  //для отладки

	void 	manager();

    std::string     generateErrorPage(int error_code);

    const std::string &getListing() const;

	int		ext_write();

	Response    *getResponse();

};


#endif //GIT_SERV_HTTP_HPP
