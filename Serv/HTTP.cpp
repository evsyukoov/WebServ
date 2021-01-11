

#include <dirent.h>
#include "HTTP.hpp"
#include "Debug.hpp"

HTTP::HTTP(int client, char *buf, const ServConf &servConf): client_fd(client), buff_req(buf), servConf(servConf) {

}

HTTP::HTTP() {
    initErrorMap();
}

const std::map<std::string, std::string> &HTTP::getRequestMap() const {
	return (reqMap);
}

int 		HTTP::initListingHTML(std::string path, const std::string &root)
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
		std::cerr << "Error open dir" << std::endl;
		return (0);
	}
	struct dirent *dir_info;
	std::string ref;

	int i = 0;
	int j = 0;
	//обрежем путь
	if (path.rfind('/') == path.size() - 1)
	    path = path.substr(0, path.size() - 1);
	while (root[j] == path[j] && root[j] && path[j])
	    j++;
	path = path.substr(j);
	while ((dir_info = readdir(dir)) != NULL) {
        if (i)
        {
            ref += "<p><a href=" + path + "/" + std::string(dir_info->d_name) + ">" + std::string(dir_info->d_name) +
                   "</a></p>\n";
        }
        i++;
    }
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
	if (reqMap.find(LENGTH) != reqMap.end() &&
	reqMap.find(TRANSFER) != reqMap.end())
		return (false);
	return (true);
}

int HTTP::initMap() {

	size_t second_pos = 0;
	size_t blanc_pos = 0;
	size_t rev_pos = 0;
	std::string str;

	reqMap.clear();
	respMap.clear();
	respMap[SERVER] = "webserv/1.0";
	respMap[LENGTH] = "0";
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
		second_pos = buff_req.find('\n', second_pos) + 1;
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
#ifdef D_REQUEST 
	//printMap();
	std::cout << YELLOW << "Your request is finish !\n" << buff_req << RESET << std::endl;
#endif
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

	if (initMap() || !validateMethod())
	{
		std::string error(errorPageResponece(400));
		sendReq("HTTP/1.1 400 Bad Request\r\n" + responceMapToString() + "\r\n", error);
		return;
	}
	reqMap["location"] = removeAllUnnecessarySlash(reqMap["location"]);
	it = getMatchingLocation();
	if (!validateProtocol())
	{
		std::string error(errorPageResponece(505));
		sendReq("HTTP/1.1 505 HTTP Version Not Supported\r\n" + responceMapToString() + "\r\n", error);
	}
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
#ifdef D_REQUEST
	std::cout << "Your new request is: " << reqMap["location"] << std::endl;
#endif
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
		{
			rootWithSlash += reqMap["location"];
			if (rootWithSlash.back() != '/')
				rootWithSlash.push_back('/');
			return (rootWithSlash + loc_index);
		}
		else if (!serv_index.empty())
		{
			rootWithSlash += reqMap["location"];
			if (rootWithSlash.back() != '/')
				rootWithSlash.push_back('/');
			return (rootWithSlash + serv_index);
		}
		else if (it != servConf.getLocations().end() && it->isAutoindex())
			throw true;
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
	//	std::cout << "Body in getpost: " << reqMap["body"] << std::endl;
		reqMap["location"].erase(pos);
	//	std::cout << "Location in getPost: " << reqMap["location"] << std::endl;
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

bool HTTP::tryAutoindex(std::string &path)
{
	try
	{
		path = removeAllUnnecessarySlash(pathFormerer());
	}
	catch (bool is_autoindex)
	{
		path = postRoot();
		if (path.back() != '/')
			path.push_back('/');
		std::string root(path);
		path = removeAllUnnecessarySlash(root + reqMap["location"]);
		initListingHTML(path, root);
		respMap[LENGTH] = std::to_string(listing.size());
		respMap[TYPE] = "text/html";
		sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", listing);
		return (true);
	}
	return (false);
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
		std::string error(errorPageResponece(405));
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", error);
		return;
	}
	else if (postGet())
	{
		post();
		return;
	}
	if (tryAutoindex(path))
		return;
	accepts(lang_prior_map, AC_LANG);
	accepts(accept_charset_map, AC_CHARSET);
//	if (!acceptedLanguages(lang_prior_map))
//		lang_prior_map.clear();

 #ifdef D_GET
	printLMAP(lang_prior_map);
	printLMAP(accept_charset_map);
#endif
	if (path.empty())
	{
		std::string error(errorPageResponece(404));
		sendReq("HTTP/1.1 404 Not Found\r\n" + responceMapToString() + "\r\n", error);
	}
	else
	{
		if ((fd = open(path.c_str(), O_RDONLY)) < 0)
		{
			std::string error(errorPageResponece(404));
			sendReq("HTTP/1.1 404 Not Found\r\n" + responceMapToString() + "\r\n", error);
		}
		else if (!checkMatchingAccept(searchForMatchingAccept(lang_prior_map, removeAllUnnecessarySlash(path), compareContentLanguage, AC_LANG), LANG) ||
				 !checkMatchingAccept(searchForMatchingAccept(accept_charset_map, removeAllUnnecessarySlash(path), compareCharset, AC_CHARSET), TYPE))
		{
			std::string error(errorPageResponece(406));
			close(fd);
			sendReq("HTTP/1.1 406 Not Acceptable\r\n" + responceMapToString() + "\r\n", error);
		}
		else
		{
			fstat(fd, &structstat);
			readFile(structstat, fd, path);
		}
	}
}

void HTTP::formContentTypeLength(const std::string &path, size_t file_size)
{
	int16_t pos = 0;

	for (std::vector<File>::iterator iter = files.begin(); iter != files.end(); ++iter)
	{
		if (iter->getRoot() == path)
		{
			respMap[TYPE] = iter->getContentType();
			if (file_size != -1)
				respMap[LENGTH] = std::to_string(iter->getContentLength());
			if (!iter->getCharset().empty())
			{
				respMap[TYPE].append("; charset=" + iter->getCharset());
				return;
			}
		}
	}
	if ((pos = path.rfind('.')) == std::string::npos)
		respMap[TYPE] = File::getMime("");
	else
		respMap[TYPE] = File::getMime(path.substr(pos, path.size() - pos));
	if (file_size != -1)
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
}

void HTTP::readFile(struct stat &st, int fd, std::string &path)
{
//	char buf[st.st_size + 1];

	char *buf = new char[st.st_size + 1];
	buf[st.st_size] = '\0';
//	std::cout << st.st_size << std::endl;
	if (read(fd, buf, st.st_size) < 0)
	{
		close(fd);
		delete[] buf;
		return;
	}
	close(fd);
	formRespHeaderOK(path, st);
	sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", buf);
	delete[] buf;
}

std::string HTTP::errorPageResponece(int error_num)
{
	int fd;
	std::map<int, std::string>::const_iterator iter;
	struct stat st;

	if (error_num >= 400 && error_num < 600)
	{
		if ((iter = servConf.getErrorPages().find(error_num)) != servConf.getErrorPages().end())
		{
			if ((fd = open(iter->second.c_str(), O_RDWR, 0644)) < 0 || fstat(fd, &st) < 0)
				return ("");
			char buf[st.st_size];
			buf[st.st_size] = '\0';
			if (read(fd, buf, st.st_size) < 0)
				return ("");
			formContentTypeLength(iter->second, st.st_size);
			close(fd);
			return (buf);
		}
		else
		{
			std::string error_page(generateErrorPage(error_num));
			respMap[TYPE] = "text/html";
			respMap[LENGTH] = std::to_string(error_page.size());
			return (error_page);
		}
	}
	return ("");
}

int HTTP::sendReq(std::string header, std::string responce)
{
	int error_num;
	int fd;
	std::map<int, std::string>::const_iterator iter;
	struct stat st;

	if (reqMap["meethod"] == "HEAD")
		responce.clear();
	result = header + responce;
//	std::cout << "Result responce: " << result << std::endl;

//	int i = 1;
//	while (!write(client_fd, (char*)result.c_str(), 10000))
//	{
//		result = result.substr(10000 * i, result.size());
//		i++;
//	}
//
//	int ret;
//	int size = result.size();
//	while (size)
//	{
//		ret = write(client_fd, result.substr(0, 10000).c_str(), 10000);
//		result = result.substr(10000);
//		size -= ret;
//	}
	if (send(client_fd, (char *)result.c_str(), result.size(), 0) < 0)
		return (0);
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

#ifdef D_CGI
	std::cout << "Lentght: " << cgi->content_length << std::endl;
	std::cout << "Query string: " << cgi->query_string << std::endl;
	std::cout << "Request uri: " << cgi->request_uri << std::endl;
	std::cout << "Path translated: " << cgi->path_translated << std::endl;
	std::cout << "Content type: " << cgi->content_type << std::endl;
	std::cout << "Path info: " << cgi->path_info << std::endl;
#endif
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

bool HTTP::validateTransferEncoding()
{
	if (reqMap.find(TRANSFER) != reqMap.end())
	{
		std::vector<std::string> validation = ft_split(reqMap[TRANSFER], ",");
		if (std::find(validation.begin(), validation.end(), "chunked") != validation.end())
			return (true);
		return (false);
	}
	return (false);
}

bool HTTP::postRootConfig(std::string &post_root)
{
	former(post_root);
	if (!(validateExtencion(post_root)))
	{
		std::string error(errorPageResponece(405));
		respMap[ALLOW] = makeAllow("POST");
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", error);
		return (false);
	}
	return (true);
}

bool HTTP::postPutvalidation(std::string &put_post_root, File &file, bool post_flag)
{
	put_post_root = postRoot();
	std::string error;
	bool validate;

	if (put_post_root.back() != '/')
		put_post_root.push_back('/');
	if (post_flag)
	{
		if (!postRootConfig(put_post_root))
			return (false);
	}
	if (!checkForAllowedMethod())
	{
		respMap[ALLOW] = makeAllow("");
		error = errorPageResponece(405);
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", error);
		return (false);
	}
	if (!(validate = validateTransferEncoding()) && file.getContentLength() == -1)
	{
		error = errorPageResponece(411);
		sendReq("HTTP/1.1 411 Length Required\r\n" + responceMapToString() + "\r\n", error);
		return (false);
	}
	if (validate)
		file.setContentLength(reqMap["body"].size());
	if (it != servConf.getLocations().end())
	{
		if (it->getMaxBody() != -1 && it->getMaxBody() < file.getContentLength()) //|| it->getMaxBody() < reqMap.count("body"))
		{
			error = errorPageResponece(413);
			sendReq("HTTP/1.1 413 Payload Too Large\r\n" + responceMapToString() + "\r\n", error);
			return (false);
		}
	}
	return (true);
}

void HTTP::post()
{
//	long content_length = contentLength();
	File file(reqMap);
	t_cgi cgi;
	std::string post_root;

	if (!postPutvalidation(post_root, file, true))
		return;
//	reqMap["location"].erase(0, it->getLocation().size());
//	post_root += reqMap["location"];
//	post_root = removeAllUnnecessarySlash(post_root);
//	if (!(validateExtencion(post_root)))
//	{
//		std::string error(errorPageResponece(405));
//		respMap[ALLOW] = makeAllow("POST");
//		sendReq("HTTP/1.1 405 Method Not Allowed\r\n" + responceMapToString() + "\r\n", error);
//	}
	else
	{
		fill_cgi(&cgi, file, post_root);
		CGI worker_cgi(cgi, servConf, in);


#ifdef D_CGI
		std::cout << "cgi in" << std::endl;
#endif
		worker_cgi.run();
#ifdef D_CGI
		std::cout << "cgi out" << std::endl;
#endif
		respMap[LENGTH] = std::to_string(worker_cgi.getResponse().size());
		sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", worker_cgi.getResponse());
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

void HTTP::former(std::string &root)
{
	reqMap["location"].erase(0, it->getLocation().size());
	root += reqMap["location"];
	root = removeAllUnnecessarySlash(root);
}

void HTTP::put()
{
	int fd;
	std::string put_root;
	std::string location_saver(reqMap["location"]);
	File new_file(reqMap);

	if (!postPutvalidation(put_root, new_file, false))
		return;
	former(put_root);
//	reqMap["location"].erase(0, it->getLocation().size());
//	put_root += reqMap["location"];
//	put_root = removeAllUnnecessarySlash(put_root);
	if ((fd = open(put_root.c_str(), O_RDWR | O_TRUNC, 0644)) < 0)
	{
		fd = open(put_root.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0 || write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
		{
			std::string error(errorPageResponece(403));
			sendReq("HTTP/1.1 403 Forbidden\r\n" + responceMapToString() + "\r\n", error);
			close(fd);
			return;
		}
		formContentTypeLength(put_root, -1);
		respMap[LOCATION] = location_saver;
		sendReq("HTTP/1.1 201 Created\r\n" + responceMapToString() + "\r\n", "");
		new_file.setRoot(put_root);
		files.push_back(new_file);
		close(fd);
		return;
	}
	if (write(fd, reqMap["body"].c_str(), new_file.getContentLength()) < 0)
	{
		close(fd);
		std::string error(errorPageResponece(403));
		sendReq("HTTP/1.1 403 Forbidden\r\n" + responceMapToString() + "\r\n", error);
		return;
	}
	new_file.setRoot(put_root);
	rewriteFileToVector(new_file);
	formContentTypeLength(put_root, -1);
	sendReq("HTTP/1.1 200 OK\r\n" + responceMapToString() + "\r\n", "");
	close(fd);
}

std::string &HTTP::getResponce()
{
	return (result);
}

void HTTP::initErrorMap()
{
    errors[400] = "Bad Request";
    errors[401] = "Unauthorized";
    errors[402] = "Payment Required";
    errors[403] = "Forbidden";
    errors[404] = "Not Found";
    errors[405] = "Method Not Allowed";
    errors[406] = "Not Acceptable";
    errors[407] = "Proxy Authentication Required";
    errors[408] = "Request Timeout";
    errors[409] = "Conflict";
    errors[410] = "Gone";
    errors[411] = "Length Required";
    errors[412] = "Precondition Failed";
    errors[413] = "Payload Too Large";
    errors[414] = "URI Too Long";
    errors[415] = "Unsupported Media Type";
    errors[416] = "Range Not Satisfiable";
    errors[417] = "Expectation Failed";
    errors[418] = "I’m a teapot";
    errors[419] = "Authentication Timeout (not in RFC 2616)";
    errors[421] = "Misdirected Request";
    errors[422] = "Unprocessable Entity";
    errors[423] = "Locked";
    errors[424] = "Failed Dependency";
    errors[425] = "Too Early";
    errors[426] = "Upgrade Required";
    errors[428] = "Precondition Required";
    errors[429] = "Too Many Requests";
    errors[431] = "Request Header Fields Too Large";
    errors[449] = "Retry With";
    errors[451] = "Unavailable For Legal Reasons";
    errors[499] = "Client Closed Request";
    errors[505] = "HTTP Version Not Supported";
}

std::string     HTTP::generateErrorPage(int error_code)
{
    std::string error_html = "<!DOCTYPE html>\n"
                             "<html lang=\"en\">\n"
                             "<head>\n"
                             "<meta charset=\"UTF-8\">\n"
                             "<title>Error</title>\n";

    error_html += "<h2>" + std::to_string(error_code) + "</h2>\n";
    error_html += "<h4>" + errors[error_code] + "</h4>\n";
    error_html += "</head>\n"
                  "<body>\n"
                  "</body>\n"
                  "</html>";
    return error_html;
}

const std::string &HTTP::getListing() const {
    return listing;
}
