

#include <dirent.h>
#include "HTTP.hpp"
#include "Debug.hpp"

static void printLMAP(std::map<std::string, float> &map)
{
    /*
    std::map<std::string, float>::iterator iter;
    for (iter = map.begin(); iter != map.end() ; ++iter)
        std::cout << RED << "MAP: " << iter->first << ": " << iter->second << std::endl;
    std::cout << RESET;
     */
    (void)map;
}

static void printVec(std::vector<std::string> &vector)
{
    std::vector<std::string>::iterator iter;
    for (iter = vector.begin(); iter != vector.end() ; ++iter)
        std::cout << GREEN << "Sorted vector: " << *iter << std::endl;
    std::cout << RESET;
}


HTTP::HTTP(int client, char *buf, const ServConf &_servConf) {
	body = buf;
	this->servConf = _servConf;
	client_fd = client;
}

HTTP::HTTP() {
    initErrorMap();
}

const std::map<std::string, std::string> &HTTP::getRequestMap() const {
	return (reqMap);
}

int 		HTTP::initListingHTML(std::string &path)
{
	std::string location;
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

	int flag = 0;
	if (reqMap["location"].size() > 0 && reqMap["location"][0] != '/')
		location = std::string("/") + reqMap["location"];
	else
		location = reqMap["location"];
	if (location.back() != '/')
		location.push_back('/');

	while ((dir_info = readdir(dir)) != nullptr) {
		if (flag)
		{
			ref += "<p><a href=" + location + std::string(dir_info->d_name) + '>' + std::string(dir_info->d_name) +
				   "</a></p>\n";
		}
		flag = 1;
    }
	listing += ref;
	listing += "</head>\n"
			   "<body>\n"
			   "</body>\n"
			   "</html>";
	closedir(dir);
	return (1);
}

void HTTP::setFields(int client, std::string buf, const ServConf &serv, struct input &income, std::map<std::string, std::string> map) {
	body = buf;
	servConf = serv;
	client_fd = client;
    this->in = income;
    reqMap = map;
}


int HTTP::validateMethod()
{
	std::string valid_methods[] = { "GET", "PUT", "POST", "HEAD"};
	std::string unimplemented_methods[] = { "DELETE", "CONNECT", "OPTIONS", "TRACE"};
	for (int  i = 0;  i < 4; ++i)
	{
		if (reqMap["method"] == unimplemented_methods[i])
			return (2);
		if (reqMap["method"] == valid_methods[i])
			return (1);
	}
	return (0);
}

bool HTTP::validateRequestLine(std::map<std::string, std::string> &map)
{
	if (map["method"].empty() || map["location"].empty() || map["protocol"].empty() ||
	map["protocol"].find(' ') != std::string::npos)
		return (false);
	return (true);
}

bool HTTP::parceRequestLine(std::map<std::string, std::string> &map, std::string request)
{
	size_t rev_pos = 0;
	size_t second_pos = 0;

	if ((rev_pos = request.find(' ')) == std::string::npos)
		return (false);
	map["method"] = request.substr(0, rev_pos);
	if ((second_pos = request.find(' ', rev_pos + 1)) == std::string::npos)
		return (false);
	map["location"] = request.substr(rev_pos + 1, second_pos - rev_pos - 1);
	if ((rev_pos = request.find('\r', second_pos + 1)) == std::string::npos)
		return (false);
	map["protocol"] = request.substr(second_pos + 1, rev_pos - second_pos - 1);
	return (true);
}

bool HTTP::validateHeaderMap(std::map<std::string, std::string> &map)
{
	std::map<std::string, std::string>::iterator iter = map.begin();

	while (iter != map.end())
	{
		if (iter->first.find(' ') != std::string::npos)
			return (false);
		trimmer(iter->second);
		iter++;
	}
	if (map.find(LENGTH) != map.end() &&
	map.find(TRANSFER) != map.end())
		return (false);
	return (true);
}

bool HTTP::doubleHostLength(bool &host, bool &name, std::string &header)
{
	if (header == LENGTH && !name)
		name = true;
	else if (header == LENGTH && name)
		return (false);
	else if (header == HOST && !host)
		host = true;
	else if (header == HOST && host)
		return (false);
	return (true);
}

std::map<std::string, std::string> HTTP::clear(std::map<std::string, std::string> &map)
{
	map.clear();
	return (map);
}

std::map<std::string, std::string> HTTP::parceMap(std::string &request)
{
	std::pair<std::string, std::string> header_pair;
	std::pair<std::string, std::string> cut_pair;
	std::map<std::string, std::string> map;
	bool flag_host = false;
	bool flag_legth = false;
	std::string str;

	//std::cout << request << std::endl;
	if (!parceRequestLine(map, request) || !validateRequestLine(map))
		return (clear(map));
	cut_pair = splitPair(request, '\n');
	request = cut_pair.second;
	while ((cut_pair = splitPair(request, '\n')).first != "\r")
	{
		str = cut_pair.first;
		request = cut_pair.second;
		header_pair = splitPair(str, ':');
		if (header_pair.first == str)
			return (clear(map));
		header_pair.second.pop_back();
		map[header_pair.first] = header_pair.second;
		if (!doubleHostLength(flag_host, flag_legth, header_pair.first))
			return (clear(map));
	}
	if (!validateHeaderMap(map))
		return (clear(map));
	request = cut_pair.second;
	return (map);
}

int HTTP::initMap() {

	respMap.clear();
	respMap[SERVER] = "webserv/1.0";
	respMap[LENGTH] = '0';
	timer();
	if (reqMap.empty())
		return (1);
	if (!body.empty())
		reqMap["body"] = body;
	return (0);
}

void 	HTTP::printMap()
{
	//std::cout << "Your parced map is: " << std::endl;
	std::map<std::string, std::string>::iterator i1 = reqMap.begin();
	std::map<std::string, std::string>::iterator i2 = reqMap.end();
	while (i1 != i2)
	{
		//std::cout << i1->first << ": " << i1->second << std::endl;
		i1++;
	}
}

std::string HTTP::makeAllow(std::string const &except)
{
	std::string allowed_methods[4] = {"GET", "HEAD", "POST", "PUT"};
	std::string allow;

	if (it != servConf.getLocations().end())
	{
		if (it->getMethods().empty())
		{
			for (int i = 0; i < 4 ; ++i)
			{
				if (allowed_methods[i] == except)
					continue;
				allow += allowed_methods[i] + ", ";
			}
		}
		else
		{
            std::vector<std::string>::const_iterator iter;
			for (iter = it->getMethods().begin(); iter != it->getMethods().end(); ++iter)
			{
				if (*iter == except)
					continue;
				allow.append(*iter + ", ");
			}
		}
	}
	if (allow.size() > 2)
	{
		allow.pop_back();
		allow.pop_back();
	}
	return (allow);
}

void HTTP::timer()
{
	struct timeval tv;

	gettimeofday(&tv, nullptr);
	formTime(tv.tv_sec, DATE);
}

bool HTTP::validateHost()
{
	std::pair<std::string, std::string> pair;

	if (reqMap.find(HOST) != reqMap.end())
	{
		pair = splitPair(reqMap[HOST], ':');
		if ((pair.first != servConf.getServerName() && pair.first != "localhost" && pair.first != "127.0.0.1") || (!pair.second.empty() && pair.second != std::to_string(servConf.getPort())) ||
		(pair.second.empty() && servConf.getPort() != 80))
			return (false);
		return (true);
	}
	return (false);
}

void HTTP::manager() {

	if (initMap() || !validateMethod()) //|| !validateHost())
	{
		std::string error(errorPageResponece(400));
		sendReq(responceMapToString(400), error);
		return;
	}
	if (validateMethod() == 2)
	{
		std::string error(errorPageResponece(501));
		sendReq(responceMapToString(501), error);
		return;
	}
	reqMap["location"] = removeAllUnnecessarySlash(reqMap["location"]);
	it = getMatchingLocation();
	if (reqMap["protocol"] != "HTTP/1.1")
	{
		std::string error(errorPageResponece(505));
		sendReq(responceMapToString(505), error);
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
	else if (location.size() > 0 && location[location.size() - 1] == '/' &&
	!(std::strncmp(location.c_str(), reqMap["location"].c_str(), location.size() - 1)))
		return (true);
	return (false);
}

static int checkDirectory(const std::string& root)
{
	struct stat structstat;

	if (!stat(root.c_str(), &structstat))
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
	if ((checkDirectoryRet = checkDirectory(rootWithSlash + reqMap["location"])) == 0)
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
		if ((locationMatch((*it).getLocation()))) {
            return (it);
        }
		it++;
	}
	return (it);
}

std::string HTTP::pathFormerer() {

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
	if (it == servConf.getLocations().end())
		return (true);

	std::vector<std::string>::const_iterator found;
	std::vector<std::string> const &AllowedMethods(it->getMethods());

	if (AllowedMethods.empty())
	    return (true);
	found = std::find(AllowedMethods.begin(), AllowedMethods.end(), reqMap["method"]);
	/* if found == AllowedMethods.end - no method was found */
	return (found != AllowedMethods.end());
}

bool HTTP::postGet()
{
	size_t pos = reqMap["location"].find('?');

	if (pos != std::string::npos)
	{
		reqMap["body"] = reqMap["location"].substr(pos + 1, reqMap["locaton"].size() - pos);
		reqMap["location"].erase(pos);
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

bool HTTP::putInPriorMap(std::map<std::string, float>& prior_map, std::string const &lang)
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

bool HTTP::accepts(std::map<std::string, float>& prior_map, std::string const &base)
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

bool HTTP::compareCharset(std::vector<File>::iterator matching_file, std::string const &charset)
{
	return (matching_file->getCharset() == charset);
}

bool HTTP::compareContentLanguage(std::vector<File>::iterator matching_file, std::string const &language)
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

std::string HTTP::getMatchingAccept(std::map<std::string, float> accepts, bool (*func)(std::vector<File>::iterator, const std::string &), std::vector<File>::iterator iter)
{
	std::vector<std::string> sorted;

	sorted = passMap(accepts);
	printVec(sorted);
    std::vector<std::string>::iterator vec_it;
	for (vec_it = sorted.begin(); vec_it != sorted.end(); ++vec_it)
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

std::string HTTP::searchForMatchingAccept(std::map<std::string, float> &accepts, std::string const &path,
            bool (*func)(std::vector<File>::iterator, std::string const &), std::string const &base)
{
	std::vector<File>::iterator iter = files.begin();
	std::vector<std::string> sorted;

	while (iter != files.end())
	{
		if (iter->getRoot() == path)
			break;
		iter++;
	}
	if (iter == files.end())
		return ("no file");
	if (base == AC_LANG  && !iter->getContentLanguage().empty() && !accepts.empty())
		return (getMatchingAccept(accepts, func, iter));
	else if (base == AC_CHARSET && !iter->getCharset().empty() && !accepts.empty())
		return (getMatchingAccept(accepts, func, iter));
	return ("no file");
}

std::string HTTP::responceMapToString(int statusCode)
{
	std::string headers;
	std::string resLine = "HTTP/1.1 " + std::to_string(statusCode) + ' ' + errors[statusCode] + "\r\n";

    std::map<std::string, std::string>::iterator iter;
	for (iter = respMap.begin();  iter != respMap.end() ; ++iter)
		headers.append(iter->first + ": " + iter->second + "\r\n");
	headers.append("\r\n");
	return (resLine + headers);
}

bool HTTP::checkMatchingAccept(std::string const &matching, std::string const &base)
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
	size_t pos = 0;

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
		initListingHTML(path);
		respMap[LENGTH] = std::to_string(listing.size());
		respMap[TYPE] = "text/html";
		sendReq(responceMapToString(200), listing);
		return (true);
	}
	return (false);
}

// GET запрос
int HTTP::get()
{
	int fd;
	struct stat structstat;
	std::string path;
	std::map<std::string, float> lang_prior_map;
	std::map<std::string, float> accept_charset_map;

	if (!checkForAllowedMethod())
	{
		respMap[ALLOW] = makeAllow();
		std::string error(errorPageResponece(405));
		sendReq(responceMapToString(405), error);
		return (0);
	}
	else if (postGet())
	{
		post(true);
		return (0);
	}
	if (tryAutoindex(path))
		return (0);
	accepts(lang_prior_map, AC_LANG);
	accepts(accept_charset_map, AC_CHARSET);
//	if (!acceptedLanguages(lang_prior_map))
//		lang_prior_map.clear();

	printLMAP(lang_prior_map);
	//printLMAP(accept_charset_map);

	if (path.empty())
	{
		std::string error(errorPageResponece(404));
		sendReq(responceMapToString(404), error);
	}
	else
	{
		if ((fd = open(path.c_str(), O_RDONLY)) < 0)
		{
			std::string error(errorPageResponece(404));
			sendReq(responceMapToString(404), error);
		}
		else if (!checkMatchingAccept(searchForMatchingAccept(lang_prior_map, removeAllUnnecessarySlash(path), compareContentLanguage, AC_LANG), LANG) ||
				 !checkMatchingAccept(searchForMatchingAccept(accept_charset_map, removeAllUnnecessarySlash(path), compareCharset, AC_CHARSET), TYPE))
		{
			std::string error(errorPageResponece(406));
			close(fd);
			sendReq(responceMapToString(406), error);
		}
		else
		{
			fstat(fd, &structstat);
			readFile(structstat, fd, path);
		}
	}
	return (0);
}

void HTTP::formContentTypeLength(const std::string &path, ssize_t file_size)
{
	size_t pos;
    std::vector<File>::iterator iter;
	for (iter = files.begin(); iter != files.end(); ++iter)
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

void HTTP::formTime(long long time_sec, std::string const &base)
{
	struct tm time;
	std::string t_str;
	char buf[29];

	t_str = std::to_string(time_sec);
	strptime(const_cast<char*>(t_str.c_str()), "%s", &time);
	strftime(buf, 29, "%a, %d %b %Y %T %Z", &time);
	respMap[base] = buf;
}

void HTTP::formRespHeaderOK(std::string &path, struct stat st)
{
	formContentTypeLength(path, st.st_size);
	formTime(st.st_mtimespec.tv_sec, LAST_MOD);
}

void HTTP::readFile(struct stat &st, int fd, std::string &path)
{
	char *buf = new char[st.st_size + 1];
	buf[st.st_size] = '\0';
	if (read(fd, buf, st.st_size) < 0)
	{
		close(fd);
		delete[] buf;
		return;
	}
	close(fd);
	formRespHeaderOK(path, st);
	sendReq(responceMapToString(200), buf);
	delete[] buf;
}

std::string HTTP::errorPageResponece(int error_num)
{
	int fd;
	std::map<int, std::string>::const_iterator iter;
	struct stat st;

	if (error_num >= 400 && error_num < 600)
	{
		if ((iter = servConf.getErrorPages().find(error_num)) != servConf.getErrorPages().end()
		&& (fd = open(iter->second.c_str(), O_RDWR, 0644)) >= 0 && !fstat(fd, &st))
		{
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


int HTTP::x_write(int fd, std::string const &buf, size_t len)
{
	int ret = write(fd, (char *)buf.c_str(), len);

	if (ret == -1)
		return (-1);
	return (0);
}


int HTTP::x_write(std::map<std::string, std::string> responseMap)
{
	int			localfd = open(responseMap["#file"].c_str(), O_RDONLY);
	ssize_t		length = findFileSize(localfd) - std::stoul(respMap["#lseek"]);

	std::string respLine = "HTTP/1.1 " + respMap["Status"] + "\r\n";
	respMap.erase(respMap.find("Status"));
	lseek(localfd, std::stoul(respMap["#lseek"]), SEEK_SET); // start of body
	std::map<std::string, std::string>::iterator iter = this->respMap.begin();
	while (iter != respMap.end())
	{
		if (iter->first[0] != '#')
		{
			std::string field = headerPairToStr(*iter);
			respLine += field;
		}
		++iter;
	}
	respLine += "\r\n";
//	PRINT(i++)
	PRINT(respLine)
	to_send = new FileResponse(this->client_fd, respLine, localfd, length);
	to_send->setTmpFile(respMap["#file"]);
	//@todo unlink(respMap["#file"].c_str());
	return (0);
}

Response *HTTP::getResponse()
{
	return to_send;
}


int HTTP::sendReq(std::string const &header, std::string responce)
{
	if (reqMap["method"] == "HEAD")
		responce.clear();
	result = header + responce;

//	PRINT(
//        "result:\n" << result);
	to_send = new StringResponse(client_fd, header, responce);
	return (1);
}

void HTTP::cgiFiller(File &file, std::string &location)
{
	if (file.getContentLength() != -1)
		reqMap[LENGTH] = std::to_string(file.getContentLength());
	else
		reqMap[LENGTH] = std::to_string(reqMap["body"].size());
	reqMap[TYPE] = file.getContentType();
	reqMap[LOCATION] = location;
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
	size_t dot_pos;
	size_t slash_pos;

	if ((dot_pos = root.rfind('.')) == std::string::npos)
		return (false);
	if (it != servConf.getLocations().end())
	{
		if (root.substr(dot_pos, root.size() - dot_pos) == it->getCgiExtension())
			return (true);
		slash_pos = root.find('/', dot_pos);
		if (slash_pos != std::string::npos && root.substr(dot_pos, slash_pos - dot_pos) == it->getCgiExtension())
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
	if (checkDirectory(post_root) == 1)
	{
		if (post_root.back() != '/')
			post_root.push_back('/');
		post_root += it->getCgiIndex();
		reqMap["location"].push_back('/');
		reqMap["location"] += it->getCgiIndex();
		reqMap["location"] = removeAllUnnecessarySlash(reqMap["location"]);
		post_root = removeAllUnnecessarySlash(post_root);
	}
	if (!(validateExtencion(post_root)))
	{
		std::string error(errorPageResponece(405));
		respMap[ALLOW] = makeAllow("POST");
		sendReq(responceMapToString(405), error);
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
		file.setAll(reqMap);
	}
	if (!checkForAllowedMethod())
	{
		respMap[ALLOW] = makeAllow();
		error = errorPageResponece(405);
		sendReq(responceMapToString(405), error);
		return (false);
	}
	if (!(validate = validateTransferEncoding()) && file.getContentLength() == -1)
	{
		error = errorPageResponece(411);
		sendReq(responceMapToString(411), error);
		return (false);
	}
	if (validate)
		file.setContentLength(reqMap["body"].size());
	if (it != servConf.getLocations().end())
	{
		if (it->getMaxBody() != -1 && it->getMaxBody() < file.getContentLength()) //|| it->getMaxBody() < reqMap.count("body"))
		{
			error = errorPageResponece(413);
			sendReq( responceMapToString(413), error);
			return (false);
		}
	}
	return (true);
}

void	HTTP::hardcodeMap(std::map<std::string, std::string> responseMap)
{
	int			localfd = open(responseMap["#file"].c_str(), O_RDONLY);
	ssize_t		length = findFileSize(localfd) - std::strtol(responseMap["#lseek"].c_str(), nullptr, 0);
	this->respMap["Content-Length"] = std::to_string(length);
	//this->respMap["Transfer-Encoding"] = "chunked";

	std::map<std::string, std::string>::iterator iter = responseMap.begin();

	while (iter != responseMap.end())
	{
		//if (it->first[0] != '#')
		this->respMap[iter->first] = iter->second;
		++iter;
	}
	close(localfd);
}

bool HTTP::postGetValidation(std::string &root)
{
	root = postRoot();
	int dir_check = checkDirectory(root);

	if (root.back() != '/')
		root.push_back('/');
	former(root);
	if (dir_check == 2)
	{
		std::string error(errorPageResponece(404));
		sendReq(responceMapToString(404), error);
		return (false);
	}
	if (!(validateExtencion(root)))
	{
		std::string error(errorPageResponece(405));
		respMap[ALLOW] = makeAllow();
		sendReq(responceMapToString(405), error);
		return (false);
	}
	return (true);
}

void HTTP::post(bool post_put_flag)
{

	File file;
	std::string post_root;
	std::string save_lock(reqMap["location"]);

	if (!post_put_flag && !postPutvalidation(post_root, file, true))
		return;
	else
	{
		if (post_put_flag && !postGetValidation(post_root))
			return;
		cgiFiller(file, save_lock);
		in.requestMap = &reqMap;
		in.root = post_root;
		CGI worker_cgi(servConf, *it ,in);
		if (!(worker_cgi.run()))
        {
		    std::string error(errorPageResponece(500));
            sendReq(responceMapToString(500), error);
		    return ;
        }
		hardcodeMap(worker_cgi.getRespMap());
		//if (respMap.count(TYPE) == 0)
		//	respMap[TYPE] = "application/octet-stream";
		x_write(respMap);
	}
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

bool HTTP::putManager(std::string &put_root, File &file, std::string const &uri, int flag,  const std::string &responce)
{
	int fd;

	if ((fd = open(put_root.c_str(), O_RDWR | O_TRUNC, 0644)) < 0)
	{
		fd = open(put_root.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0 || x_write(fd, reqMap["body"], file.getContentLength()) < 0)
		{
			std::string error(errorPageResponece(403));
			sendReq(responceMapToString(403), error);
			close(fd);
			return (false);
		}
		formContentTypeLength(put_root, -1);
		respMap[LOCATION] = uri;
		sendReq(responceMapToString(201), responce);
		file.setRoot(put_root);
		files.push_back(file);
		close(fd);
		return (true);
	}
	if (x_write(fd, reqMap["body"], file.getContentLength()) < 0)
	{
		close(fd);
		std::string error(errorPageResponece(403));
		sendReq(responceMapToString(403), error);
		return (false);
	}
	file.setRoot(put_root);
	rewriteFileToVector(file);
	formContentTypeLength(put_root, -1);
	if (flag == 1)
		respMap[CONT_LOC] = uri;
	sendReq(responceMapToString(200), responce);
	close(fd);
	return (true);
}

void HTTP::put()
{
	std::string put_root;
	std::string location_saver(reqMap["location"]);
	if (location_saver.empty())
		location_saver = "/";
	File new_file(reqMap);

	if (!postPutvalidation(put_root, new_file, false))
		return;
	former(put_root);
	putManager(put_root, new_file, location_saver);
}

std::string &HTTP::getResponce()
{
	return (result);
}

void HTTP::initErrorMap()
{
	errors[200] = "OK";
	errors[201] = "Created";
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
    errors[500] = "Internal Server Error";
    errors[501] = "Not Implemented";
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
