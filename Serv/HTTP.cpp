

#include <dirent.h>
#include "HTTP.hpp"

HTTP::HTTP(int client, char *buf, const ServConf &servConf): client_fd(client), buff_req(buf), servConf(servConf) {}

HTTP::HTTP() {}

const std::map<std::string, std::string> &HTTP::getRequestMap() const {
	return (reqMap);
}

int 		HTTP::initListingHTML(const std::string &path)
{
	//сюда запишем ссылки на директории по указанному path чтобы потом отправить клиенту
	listing = "<!DOCTYPE html>\n"
							"<html lang=\"en\">\n"
							"<head>\n"
							"<meta charset=\"UTF-8\">\n"
							"<title>Listing</title>\n"
	   						"<p><strong>Index of " + path + "</strong>\n\n";
	DIR *dir = opendir(path.c_str());
	if (!dir)
	{
		std::cout << "Error open dir" << std::endl;
		return (0);
	}
	struct dirent *dir_info;
	std::string ref;

	//<p><a href=dir_name> dir_name </a></p>
	while ((dir_info = readdir(dir)) != NULL)
		ref += "<p><a href=" + std::string(dir_info->d_name) + ">" + std::string(dir_info->d_name) + "</a></p>\n";
	listing += ref;
	listing += "</head>\n"
			   "<body>\n"
			   "</body>\n"
			   "</html>";
	closedir(dir);
	return (1);
}

void HTTP::setFields(int client, char *buf, const ServConf &serv, struct input &income) {
	buff_req = buf;
	servConf = serv;
	client_fd = client;
    this->in = income;
}


bool HTTP::validateMethod()
{
	std::string methods[] = { "GET", "PUT", "POST", "DELETE",
						   "HEAD", "CONNECT", "OPTIONS", "TRACE"};
	for (int  i = 0;  i < 8; ++i)
	{
		if (reqMap["method"] == methods[i])
			return (true);
	}
	return (false);
}

bool HTTP::validateProtocol()
{
	if (reqMap["protocol"] == "HTTP/1.1")
		return (true);
	return (false);
}

bool HTTP::validateRequestLine()
{
	if (reqMap["method"].empty() || reqMap["location"].empty() || reqMap["protocol"].empty() ||
	reqMap["protocol"].find(' ') != std::string::npos)
		return (false);
	return (true);
}

bool HTTP::parceRequestLine(size_t &second_pos, size_t &rev_pos)
{
	if ((rev_pos = buff_req.find(' ')) == std::string::npos)
		return (false);
	reqMap["method"] = buff_req.substr(0, rev_pos);
	if ((second_pos = buff_req.find(' ', rev_pos + 1)) == std::string::npos)
		return (false);
	reqMap["location"] = buff_req.substr(rev_pos + 1, second_pos - rev_pos - 1);
	if ((rev_pos = buff_req.find("\r", second_pos + 1)) == std::string::npos)
		return (false);
	reqMap["protocol"] = buff_req.substr(second_pos + 1, rev_pos - second_pos - 1);
	return (true);
}

bool HTTP::validateHeaderMap()
{
	std::map<std::string, std::string>::iterator it = reqMap.begin();

	while (it != reqMap.end())
	{
		if (it->first.find(' ') != std::string::npos)
			return (false);
		trimmer(it->second);
		it++;
	}
	return (true);
}

int HTTP::initMap() {

	size_t second_pos = 0;
	size_t blanc_pos = 0;
	size_t rev_pos = 0;
	std::string str;

	reqMap.clear();
	if (!parceRequestLine(second_pos, rev_pos) || !validateRequestLine())
		return (1);
	second_pos = rev_pos + 2;
	while ((str = buff_req.substr(second_pos, buff_req.find("\n", second_pos) - second_pos)) != "\r" && second_pos < buff_req.size()
	&& second_pos != 0)
	{
	    //std::cout << "Loop" << std::endl;
		blanc_pos = str.find(":");
		rev_pos = str.find("\r", blanc_pos + 1);
		reqMap[str.substr(0, blanc_pos)] = str.substr(blanc_pos + 1, rev_pos - blanc_pos - 1);
		second_pos = buff_req.find("\n", second_pos) + 1;
		//std::cout << buff_req.find("\n", second_pos) << std::endl;
	}
	if (!validateHeaderMap())
		return (1);
	if (!second_pos)
		return (0);
	if (second_pos >= buff_req.size())
		return (0);
	if (buff_req[second_pos] != '\r' || (second_pos + 1 < buff_req.size() && buff_req[second_pos + 1] != '\n'))
		return (0);
//	size_t len = buff_req.size();
	second_pos += 2;
//	while ((buff_req[second_pos] == '\r' || buff_req[second_pos] == '\n') && second_pos < buff_req.size())
//	{
//		//std::cout <<
//		second_pos++;
//	}
	if (buff_req.size() > second_pos)
		reqMap["body"] = buff_req.substr(second_pos, buff_req.size() - second_pos);
	printMap();
	std::cout << YELLOW << "Your request is:\n" << buff_req << RESET << std::endl;
	return (0);
}

void 	HTTP::printMap()
{
	std::cout << "Your parced map is: " << std::endl;
	std::map<std::string, std::string>::iterator i1 = reqMap.begin();
	std::map<std::string, std::string>::iterator i2 = reqMap.end();
	while (i1 != i2)
	{
		std::cout << i1->first << ": " << i1->second << std::endl;
		i1++;
	}
}

void HTTP::manager() {

	if (initMap())
	{
		sendReq("HTTP/1.1 400 Bad Request\r\n\r\n", "");
		return;
	}
	it = getMatchingLocation();
	if (!validateMethod())
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
	else if (!validateProtocol())
		sendReq("HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n", "");
	else if (reqMap["method"] == "GET" || reqMap["method"] == "HEAD")
		get();
	else if (reqMap["method"] == "POST")
		post();
	else if (reqMap["method"] == "PUT")
		put();
}

bool HTTP::locationMatch(const std::string& location)
{
	if (!(std::strncmp(location.c_str(), reqMap["location"].c_str(), location.size())))
		return (true);
	return (false);
}

void 	HTTP::locationToRootReplcaer(std::string& root_with_slash)
{
	//reqMap["location"].replace(0, it->getLocation().size(), root_with_slash);
	reqMap["location"].erase(0, it->getLocation().size());
//	if (reqMap["location"][0] == '/')
//		reqMap["location"].erase(0, 1);
	std::cout << "Your new request is: " << reqMap["location"] << std::endl;
}

int HTTP::checkDirectory(const std::string& root)
{
	struct stat structstat;

	std::cout << root + reqMap["location"] << std::endl;
//	if (stat("/Users/zcolleen/Desktop/webserv2/Serv/testfiles/test.txt", &structstat))
//		std::cout << "working" << std::endl;
	if (!stat((root + reqMap["location"]).c_str(), &structstat))
	{
		if (S_ISREG(structstat.st_mode))
			return (0);
		else
			return (1);
	}
	return (2);
}

std::string HTTP::rootSwitcher(const std::string& root, const std::string& serv_index, const std::string& loc_index)
{
	int checkDirectoryRet;
	std::string rootWithSlash(root);

	if (rootWithSlash.back() != '/')
		rootWithSlash.push_back('/');
	reqMap["location"].erase(0, it->getLocation().size());
	if ((checkDirectoryRet = checkDirectory(rootWithSlash)) == 0)
		return (rootWithSlash + reqMap["location"]);
	else if (checkDirectoryRet == 1)
	{
		if (!loc_index.empty())
			return (rootWithSlash + loc_index);
		else if (!serv_index.empty())
			return (rootWithSlash + serv_index);
		else
			return ("");
	}
	else
		return ("");
}

std::list<Location>::const_iterator HTTP::getMatchingLocation()
{
	it = servConf.getLocations().begin();

	while (it != servConf.getLocations().end())
	{
		if ((locationMatch((*it).getLocation())))
			return (it);
		it++;
	}
	return (it);
}

std::string HTTP::pathFormerer() {

//	it = getMatchingLocation(servConf);
	char buf[PATH_MAX];

	if (it != servConf.getLocations().end())
	{
		if (!it->getRoot().empty())
			return (rootSwitcher(it->getRoot(), servConf.getIndex(), it->getIndex()));
		else if (!servConf.getRoot().empty())
			return (rootSwitcher(servConf.getRoot(), servConf.getIndex(), it->getIndex()));
		else
			return (rootSwitcher(getcwd(buf, PATH_MAX), servConf.getIndex(), it->getIndex()));
	}
	else
	{
		if (!servConf.getRoot().empty())
			return (rootSwitcher(servConf.getRoot(), servConf.getIndex(), ""));
		else
			return (rootSwitcher(getcwd(buf, PATH_MAX), servConf.getIndex(), ""));
	}
}

bool HTTP::checkForAllowedMethod()
{
//	std::list<Location>::const_iterator it = getMatchingLocation(servConf);

	if (it != servConf.getLocations().end())
	{
		if (reqMap["method"] == "GET")
		{
			if (std::find(it->getMethods().begin(), it->getMethods().end(), "GET") == it->getMethods().end())
				return (false);
			else
				return (true);
		}
		else if (reqMap["method"] == "HEAD")
		{
			if (std::find(it->getMethods().begin(), it->getMethods().end(), "HEAD") == it->getMethods().end())
				return (false);
			else
				return (true);
		}
		else if (reqMap["method"] == "POST")
		{
			if (std::find(it->getMethods().begin(), it->getMethods().end(), "POST") == it->getMethods().end())
				return (false);
			else
				return (true);
		}
	}
	return (true);
}

bool HTTP::postGet()
{
	size_t pos = 0;

	if ((pos = reqMap["location"].find('?')) != std::string::npos)
	{
		reqMap["body"] = reqMap["location"].substr(pos + 1, reqMap["locaton"].size() - pos);
		std::cout << "Body in getpost: " << reqMap["body"] << std::endl;
		reqMap["location"].erase(pos);
		std::cout << "Location in getPost: " << reqMap["location"] << std::endl;
		return (true);
	}
	return (false);
}

bool HTTP::priorityValidation(std::string prior)
{
	for (size_t i = 0; i < prior.size(); i++)
	{
		if (i == 0 && (prior[i] != '1' && prior[i] != '0'))
			return (false);
		if (i == 1 && prior[i] != '.')
			return (false);
		if (i > 4)
			return (false);
		if (i != 0 && i != 1 && !isdigit(prior[i]))
			return (false);
	}
	return (true);
}

bool HTTP::putInPriorMap(std::map<std::string, float>& prior_map, std::string lang)
{
	std::vector<std::string> priority;
	std::vector<std::string> q_value;
	float 					value;

	if (lang.find(';') != std::string::npos)
	{
		priority = ft_split(lang, ";");
		trimmer(priority[0]);
		trimmer(priority[1]);
		if (priority[1].find(' ') != std::string::npos || priority[0].find(' ') != std::string::npos)
			return (false);
		q_value = ft_split(priority[1], "=");
		if (q_value.size() != 2 || (q_value[0] != "q" && q_value[0] != "Q") || !priorityValidation(q_value[1]))
			return (false);
		value = std::strtof(q_value[1].c_str(), nullptr);
		if (value > 1)
			return (false);
		prior_map[priority[0]] = value;
	}
	else
		prior_map[lang] = 1;
	return (true);
}

bool HTTP::accepts(std::map<std::string, float>& prior_map, std::string base)
{
	std::vector<std::string> languages;

	if (reqMap.find(base) != reqMap.end())
	{
		languages = ft_split(reqMap[base], ",");
		for (size_t pos = 0; pos < languages.size(); pos++)
		{
			trimmer(languages[pos]);
			std::transform(languages[pos].begin(), languages[pos].end(), languages[pos].begin(), tolower);
			if (std::find_if_not(languages[pos].begin(), languages[pos].end(), acceptPredicete)
				!= languages[pos].end())
				return (false);
			if (!putInPriorMap(prior_map, languages[pos]))
				return (false);
		}
		//std::sort(prior_map.begin(), prior_map.end(), std::greater<float>());
	}
	return (true);
}

void printLMAP(std::map<std::string, float> map)
{
	for (std::map<std::string, float>::iterator it = map.begin(); it != map.end() ; ++it)
	{
		std::cout << RED << "MAP: " << it->first << ": " << it->second << std::endl;
	}
	std::cout << RESET;
}

// GET запрос
void HTTP::get()
{
	int fd;
	struct stat structstat;
	std::string path;
	std::map<std::string, float> lang_prior_map;
	std::map<std::string, float> accept_charset_map;

	if (!checkForAllowedMethod())
	{
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
		return;
	}
	else if (postGet())
	{
		post();
		return;
	}
	path = pathFormerer();
	accepts(lang_prior_map, AC_LANG);
	accepts(accept_charset_map, AC_CHARSET);
//	if (!acceptedLanguages(lang_prior_map))
//		lang_prior_map.clear();
	printLMAP(lang_prior_map);
	printLMAP(accept_charset_map);
	if (path.empty())
		sendReq("HTTP/1.1 404 Not Found\r\n\r\n", "");
	else
	{
		if ((fd = open(path.c_str(), O_RDONLY)) < 0)
			sendReq("HTTP/1.1 404 Not Found\r\n\r\n", "");
		else
		{
			fstat(fd, &structstat);
			readFile(structstat.st_size, fd);
		}
	}
}

void HTTP::readFile(int file_size, int fd)
{
	char buf[file_size + 1];
//	std::string responce;
//	std::list<Location>::const_iterator it = getMatchingLocation(servConf);

	if (it != servConf.getLocations().end())
	{
		if (it->getMaxBody() != -1 && it->getMaxBody() < file_size)
		{
			sendReq("HTTP/1.1 413 Payload Too Large\r\n\r\n", "");
			return;
		}
	}
	buf[file_size] = '\0';
	if (read(fd, buf, file_size) < 0)
		return;
//	responce = buf;
//	responce += "\r\n\r\n";
	sendReq("HTTP/1.1 200 OK\r\n\r\n", buf);
}

int HTTP::sendReq(std::string header, std::string responce)
{
//	std::string result;
	int error_num;
	int fd;
	std::map<int, std::string>::const_iterator it;
	struct stat st;

	error_num = std::atoi(header.substr(9, 3).c_str());

	if (error_num >= 400 && error_num < 600)
	{
		if ((it = servConf.getErrorPages().find(error_num)) != servConf.getErrorPages().end())
		{
			if ((fd = open(it->second.c_str(), O_RDWR, 0644)) < 0 || fstat(fd, &st) < 0)
				return (-1);
			char buf[st.st_size];
			buf[st.st_size] = '\0';
			if (read(fd, buf, st.st_size) < 0)
				return (-1);
			responce = buf;
		}
	}

	std::cout << "Error num: " << error_num << std::endl;
	if (reqMap["method"] == "HEAD")
		responce = header;
	result = header + responce;
	std::cout << "Result responce: " << result << std::endl;

	//write(client_fd, (char *)result.c_str(), result.size());
	send(client_fd, (char *)result.c_str(), result.size(), 0);
//	net.send(client_fd, (char *)result.c_str(), result.size());
	return (0);
}

void HTTP::fill_cgi(t_cgi *cgi, File &file, std::string &root)
{
	cgi->content_length = file.getContentLength();
	cgi->content_type = file.getContentType();
	cgi->reques_method = reqMap["method"];
	cgi->query_string = reqMap["body"];
	cgi->request_uri = "/" + reqMap["location"];
	cgi->path_translated = root;
	cgi->path_info = "./" + reqMap["location"];
	std::cout << "Lentght: " << cgi->content_length << std::endl;
	std::cout << "Query string: " << cgi->query_string << std::endl;
	std::cout << "Request uri: " << cgi->request_uri << std::endl;
	std::cout << "Path translated: " << cgi->path_translated << std::endl;
	std::cout << "Content type: " << cgi->content_type << std::endl;
	std::cout << "Path info: " << cgi->path_info << std::endl;
}

std::string HTTP::postRoot()
{
	std::string post_root;
	char buf[PATH_MAX];

	if (it != servConf.getLocations().end())
	{
		if (!(it->getRoot().empty()))
			post_root = it->getRoot();
		else if (!(servConf.getRoot().empty()))
			post_root = servConf.getRoot();
		else
			post_root = getcwd(buf, PATH_MAX);
	}
	else
	{
		if (!servConf.getRoot().empty())
			post_root = servConf.getRoot();
		else
			post_root = getcwd(buf, PATH_MAX);
	}
	return (post_root);
}


bool HTTP::validateExtencion(std::string &root) //необходимое расширение должно начинаться с точки, надо валидировать в конфиге
{
	size_t pos = 0;

	if ((pos = root.rfind('.')) == std::string::npos)
		return (false);
	if (it != servConf.getLocations().end())
	{
		if (root.substr(pos, root.size() - pos) == it->getCgiExtension())
			return (true);
	}
	return (false);
}

bool HTTP::postPutvalidation(std::string &put_post_root, File &file)
{
	put_post_root = postRoot();

	if (!checkForAllowedMethod())
	{
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
		return (false);
	}
	if (file.getContentLength() == -1)
	{
		sendReq("HTTP/1.1 411 Length Required\r\n\r\n", "");
		return (false);
	}
	if (put_post_root.back() != '/')
		put_post_root.push_back('/');
	return (true);
}

void HTTP::post()
{
	int fd;
//	long content_length = contentLength();
	File file(reqMap);
	t_cgi cgi;
	std::string post_root;

	if (!postPutvalidation(post_root, file))
		return;
	reqMap["location"].erase(0, it->getLocation().size());
	post_root += reqMap["location"];
	if (!(validateExtencion(post_root)))
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
	else
	{
		fill_cgi(&cgi, file, post_root);
		sendReq("HTTP/1.1 200 OK\r\n\r\n", "");
	}
//	CGI cgi();
}

void HTTP::put()
{
	int fd;
	std::string put_root;
	File new_file(reqMap);

	if (!postPutvalidation(put_root, new_file))
		return;
	reqMap["location"].erase(0, it->getLocation().size());
	put_root += reqMap["location"];
	if ((fd = open(put_root.c_str(), O_RDWR | O_TRUNC, 0644)) < 0)
	{
		fd = open(put_root.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0 || write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
		{
			sendReq("HTTP/1.1 403 Forbidden\r\n\r\n", "");
			return;
		}
		sendReq("HTTP/1.1 201 Created\r\n\r\n", "");
		new_file.setRoot(put_root);
		files.push_back(new_file);
		return;
	}
	if (write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
	{
		sendReq("HTTP/1.1 403 Forbidden\r\n\r\n", "");
		return;
	}
	files.push_back(new_file);
	new_file.setRoot(put_root);
	sendReq("HTTP/1.1 200 OK\r\n\r\n", "");
}

std::string &HTTP::getResponce()
{
	return (result);
}