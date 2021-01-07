

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
	std::map<std::string, std::string>::iterator iter = reqMap.begin();

	while (iter != reqMap.end())
	{
		if (iter->first.find(' ') != std::string::npos)
			return (false);
		trimmer(iter->second);
		iter++;
	}
	return (true);
}

int HTTP::initMap() {

	size_t second_pos = 0;
	size_t blanc_pos = 0;
	size_t rev_pos = 0;
	std::string str;

	reqMap.clear();
	respMap.clear();
	timer();
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
		return (1);
	if (second_pos >= buff_req.size())
		return (0);
	if (buff_req[second_pos] != '\r' || (second_pos + 1 < buff_req.size() && buff_req[second_pos + 1] != '\n'))
		return (1);
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

std::string HTTP::makeAllow(std::string exept)
{
	std::string allow("GET, HEAD, POST, PUT");

	if (it != servConf.getLocations().end())
	{
		if (it->getMethods().empty())
			return (allow);
		else
		{
			allow.clear();
			for (std::vector<std::string>::const_iterator iter = it->getMethods().begin(); iter != it->getMethods().end(); ++iter)
			{
				if (*iter == exept)
					continue;
				allow.append(*iter + ", ");
			}
			allow.pop_back();
			allow.pop_back();
			return (allow);
		}
	}
	return (allow);
}

void HTTP::timer()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	formTime(tv.tv_sec, DATE);
}

void HTTP::manager() {

	if (initMap())
	{
		sendReq("HTTP/1.1 400 Bad Request\r\n" + responceMapToString() + "\r\n", "");
		return;
	}
	reqMap["location"] = removeAllUnnecessarySlash(reqMap["location"]);
	it = getMatchingLocation();
	if (!validateMethod())
	{
		respMap[ALLOW] = makeAllow("");
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", "");
	}
	else if (!validateProtocol())
		sendReq("HTTP/1.1 505 HTTP Version Not Supported\r\n" + responceMapToString() + "\r\n", "");
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

//	std::cout << root + reqMap["location"] << std::endl;
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

bool HTTP::findMethod(std::string find)
{
	if (!it->getMethods().empty())
	{
		if (std::find(it->getMethods().begin(), it->getMethods().end(), find) == it->getMethods().end())
			return (false);
		else
			return (true);
	}
	return (true);
}

bool HTTP::checkForAllowedMethod()
{
//	std::list<Location>::const_iterator it = getMatchingLocation(servConf);
	std::string post("POST");
	std::string put("PUT");
	std::string get("GET");
	std::string head("HEAD");

	if (it != servConf.getLocations().end())
	{
		if (reqMap["method"] == get)
			return (findMethod(get));
		else if (reqMap["method"] == head)
			return (findMethod(head));
		else if (reqMap["method"] == post)
			return (findMethod(post));
		else if (reqMap["method"] == put)
			return (findMethod(put));
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
	}
	return (true);
}

void printLMAP(std::map<std::string, float> map)
{
	for (std::map<std::string, float>::iterator iter = map.begin(); iter != map.end() ; ++iter)
		std::cout << RED << "MAP: " << iter->first << ": " << iter->second << std::endl;
	std::cout << RESET;
}

void HTTP::printVec(std::vector<std::string> vector)
{
	for (std::vector<std::string>::iterator iter = vector.begin(); iter != vector.end() ; ++iter)
		std::cout << GREEN << "Sorted vector: " << *iter << std::endl;
	std::cout << RESET;
}

std::vector<std::string> HTTP::passMap(std::map<std::string, float> accept)
{
	std::vector<std::string> sorted_map;

	while (!accept.empty())
	{
		std::map<std::string, float>::iterator max = std::max_element(accept.begin(), accept.end(), comparePriors);
		sorted_map.push_back(max->first);
		accept.erase(max);
	}
	return (sorted_map);
}

bool HTTP::compareCharset(std::vector<File>::iterator matching_file, std::string charset)
{
	return (matching_file->getCharset() == charset);
}

bool HTTP::compareContentLanguage(std::vector<File>::iterator matching_file, std::string language)
{
	std::vector<std::string>::const_iterator file_lang_it = matching_file->getContentLanguage().begin();
	std::vector<std::string>::const_iterator file_lang_it_end = matching_file->getContentLanguage().end();

	while (file_lang_it != file_lang_it_end) //content language
	{
		if (*file_lang_it == language || (!std::strncmp(file_lang_it->c_str(), language.c_str(), language.size()) &&
		file_lang_it->size() > language.size() && (*file_lang_it)[language.size()] == '-'))//file_lang_it->substr(0, file_lang_it->find_last_of('-')) == language)//)
			return (true);
		file_lang_it++;
	}
	return (false);
}

std::string HTTP::getMatchingAccept(std::map<std::string, float> accepts, bool (*func)(std::vector<File>::iterator, std::string), std::vector<File>::iterator iter)
{
	std::vector<std::string> sorted;

	sorted = passMap(accepts);
	printVec(sorted);
	for (std::vector<std::string>::iterator vec_it = sorted.begin(); vec_it != sorted.end(); ++vec_it)
	{
		if (*vec_it == "*")
			continue;
		if (func(iter, *vec_it) && accepts[*vec_it] != 0)
			return (*vec_it);
	}
	if (std::find(sorted.begin(), sorted.end(), "*") != sorted.end())
		return ("*");
	return ("not acceptable");
}

std::string HTTP::searchForMatchingAccept(std::map<std::string, float> accepts, std::string path, bool (*func)(std::vector<File>::iterator, std::string),
										  std::string base)
{
	std::vector<File>::iterator iter = files.begin();
	std::vector<std::string> sorted;

	std::cout << "Path: " + path << std::endl;
	while (iter != files.end())
	{
		if (iter->getRoot() == path)
			break;
		iter++;
	}
	if (iter == files.end())
		return ("no file");
	if (base == AC_LANG && !iter->getContentLanguage().empty() && !accepts.empty())
		return (getMatchingAccept(accepts, func, iter));
	else if (base == AC_CHARSET && !iter->getCharset().empty() && !accepts.empty())
		return (getMatchingAccept(accepts, func, iter));
	return ("no file");
}

std::string HTTP::responceMapToString()
{
	std::string headers;

	for (std::map<std::string, std::string>::iterator iter = respMap.begin();  iter != respMap.end() ; ++iter)
		headers.append(iter->first + ": " + iter->second + "\r\n");
	return (headers);
}

bool HTTP::checkMatchingAccept(std::string matching, std::string base)
{
	if (matching == "not acceptable")
		return (false);
	else if (matching == "*" || matching == "no file")
		return (true);
	else
	{
		if (base == LANG)
			respMap[base] = matching;
		return (true);
	}
}

std::string HTTP::removeAllUnnecessarySlash(std::string path)
{
	ssize_t pos = 0;

	while ((pos = path.find('/', pos)) != std::string::npos)
	{
		pos++;
		while  (pos < path.size() && path[pos] == '/')
			path.erase(pos, 1);
	}
	return (path);
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
		respMap[ALLOW] = makeAllow("");
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", "");
		return;
	}
	else if (postGet())
	{
		post();
		return;
	}
	path = removeAllUnnecessarySlash(pathFormerer());
	accepts(lang_prior_map, AC_LANG);
	accepts(accept_charset_map, AC_CHARSET);
//	if (!acceptedLanguages(lang_prior_map))
//		lang_prior_map.clear();

	printLMAP(lang_prior_map);
	printLMAP(accept_charset_map);
	if (path.empty())
		sendReq("HTTP/1.1 404 Not Found\r\n" + responceMapToString() + "\r\n", "");
	else
	{
		if ((fd = open(path.c_str(), O_RDONLY)) < 0)
			sendReq("HTTP/1.1 404 Not Found\r\n" + responceMapToString() + "\r\n", "");
		else if (!checkMatchingAccept(searchForMatchingAccept(lang_prior_map, removeAllUnnecessarySlash(path), compareContentLanguage, AC_LANG), LANG) ||
				 !checkMatchingAccept(searchForMatchingAccept(accept_charset_map, removeAllUnnecessarySlash(path), compareCharset, AC_CHARSET), TYPE))
		{
			close(fd);
			sendReq("HTTP/1.1 406 Not Acceptable\r\n" + responceMapToString() + "\r\n", "");
		}
		else
		{
			fstat(fd, &structstat);
			readFile(structstat, fd, path);
		}
	}
}

void HTTP::formContentTypeLength(std::string &path, size_t file_size)
{
	int16_t pos = 0;

	for (std::vector<File>::iterator iter = files.begin(); iter != files.end(); ++iter)
	{
		if (iter->getRoot() == path)
		{
			respMap[TYPE] = iter->getContentType();
			respMap[LENGTH] = iter->getContentLength();
			if (!iter->getCharset().empty())
			{
				respMap[TYPE].append("; charset=" + iter->getCharset());
				return;
			}
		}
	}
	if ((pos = reqMap["location"].rfind('.')) == std::string::npos)
		respMap[TYPE] = File::getMime("");
	else
		respMap[TYPE] = File::getMime(path.substr(pos, path.size() - pos));
	respMap[LENGTH] = std::to_string(file_size);
}

void HTTP::formTime(long long time_sec, std::string base)
{
//	struct timeval tv;
	struct tm time;
	std::string t_str;
	char buf[29];

//	gettimeofday(&tv, NULL);
	t_str = std::to_string(time_sec);
	strptime(const_cast<char*>(t_str.c_str()), "%s", &time);
	strftime(buf, 29, "%a, %d %b %Y %T GMT", &time);
	respMap[base] = buf;
}

void HTTP::formRespHeaderOK(std::string &path, struct stat st)
{
	formContentTypeLength(path, st.st_size);
	formTime(st.st_mtimespec.tv_sec, LAST_MOD);
	respMap[SERVER] = "webserv/1.0";
}

void HTTP::readFile(struct stat &st, int fd, std::string &path)
{
	char buf[st.st_size + 1];

	buf[st.st_size] = '\0';
	if (read(fd, buf, st.st_size) < 0)
	{
		close(fd);
		return;
	}
	close(fd);
	formRespHeaderOK(path, st);
	sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", buf);
}

int HTTP::sendReq(std::string header, std::string responce)
{
	int error_num;
	int fd;
	std::map<int, std::string>::const_iterator iter;
	struct stat st;

	error_num = std::atoi(header.substr(9, 3).c_str());

	if (error_num >= 400 && error_num < 600)
	{
		if ((iter = servConf.getErrorPages().find(error_num)) != servConf.getErrorPages().end())
		{
			if ((fd = open(iter->second.c_str(), O_RDWR, 0644)) < 0 || fstat(fd, &st) < 0)
				return (-1);
			char buf[st.st_size];
			buf[st.st_size] = '\0';
			if (read(fd, buf, st.st_size) < 0)
				return (-1);
			close(fd);
			responce = buf;
		}
	}

	std::cout << "Error num: " << error_num << std::endl;
	if (reqMap["method"] == "HEAD")
		responce = header;
	result = header + responce;
	std::cout << "Result responce: " << result << std::endl;

	send(client_fd, (char *)result.c_str(), result.size(), 0);
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
		respMap[ALLOW] = makeAllow("");
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", "");
		return (false);
	}
	if (file.getContentLength() == -1)
	{
		sendReq("HTTP/1.1 411 Length Required\r\n" + responceMapToString() + "\r\n", "");
		return (false);
	}
	if (it != servConf.getLocations().end())
	{
		if (it->getMaxBody() != -1 && it->getMaxBody() < file.getContentLength())
		{
			sendReq("HTTP/1.1 413 Payload Too Large\r\n" + responceMapToString() + "\r\n", "");
			return (false);
		}
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
	post_root = removeAllUnnecessarySlash(post_root);
	if (!(validateExtencion(post_root)))
	{
		respMap[ALLOW] = makeAllow("POST");
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", "");
	}
	else
	{
		fill_cgi(&cgi, file, post_root);
		sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", "");
	}
//	CGI cgi();
}

void HTTP::rewriteFileToVector(File &file)
{
	std::vector<File>::iterator iter = files.begin();

	while (iter != files.end())
	{
		if (iter->getRoot() == file.getRoot())
		{
			files.erase(iter);
			files.push_back(file);
			break;
		}
		iter++;
	}
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
	put_root = removeAllUnnecessarySlash(put_root);
	if ((fd = open(put_root.c_str(), O_RDWR | O_TRUNC, 0644)) < 0)
	{
		fd = open(put_root.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0 || write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
		{
			sendReq("HTTP/1.1 403 Forbidden\r\n" + responceMapToString() + "\r\n", "");
			close(fd);
			return;
		}
		sendReq("HTTP/1.1 201 Created\r\n" + responceMapToString() + "\r\n", "");
		new_file.setRoot(put_root);
		files.push_back(new_file);
		close(fd);
		return;
	}
	if (write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
	{
		close(fd);
		sendReq("HTTP/1.1 403 Forbidden\r\n" + responceMapToString() + "\r\n", "");
		return;
	}
	new_file.setRoot(put_root);
	rewriteFileToVector(new_file);
	sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", "");
	close(fd);
}

std::string &HTTP::getResponce()
{
	return (result);
}