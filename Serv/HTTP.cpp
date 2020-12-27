

#include "HTTP.hpp"

HTTP::HTTP(int client, char *buf, const ServConf &servConf): client_fd(client), buff_req(buf), servConf(servConf) {}

HTTP::HTTP() {}

const std::map<std::string, std::string> &HTTP::getRequestMap() const {
	return (reqMap);
}

void HTTP::setFields(int client, char *buf, const ServConf &serv) {
	buff_req = buf;
	servConf = serv;
	client_fd = client;
	initMap();
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

int HTTP::initMap() {

	size_t second_pos;
	size_t blanc_pos;
	size_t rev_pos;
	std::string str;

	reqMap.clear();
	rev_pos = buff_req.find(" ");
	reqMap["method"] = buff_req.substr(0, rev_pos);
	second_pos = buff_req.find(" ", rev_pos + 1);
	reqMap["location"] = buff_req.substr(rev_pos + 1, second_pos - rev_pos - 1);
	rev_pos = buff_req.find("\r", second_pos + 1);
	reqMap["protocol"] = buff_req.substr(second_pos + 1, rev_pos - second_pos - 1);

	second_pos = rev_pos + 2;
	while ((str = buff_req.substr(second_pos, buff_req.find("\n", second_pos) - second_pos)) != "\r" && second_pos < buff_req.size()
	&& second_pos != 0)
	{
	    //std::cout << "Loop" << std::endl;
		blanc_pos = str.find(" ");
		rev_pos = str.find("\r", blanc_pos + 1);
		reqMap[str.substr(0, blanc_pos - 1)] = str.substr(blanc_pos + 1, rev_pos - blanc_pos - 1);
		second_pos = buff_req.find("\n", second_pos) + 1;
		//std::cout << buff_req.find("\n", second_pos) << std::endl;
	}
	if (second_pos == 0 || second_pos >= buff_req.size())
		return (1);
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

void HTTP::manager() {

//	ServConf servConf = getServerNum(server_num);
	it = getMatchingLocation();

	if (!validateMethod() || !validateProtocol())
		sendReq("HTTP/1.1 405 Method Not Allowed\r\nConnection: Closed\r\n\r\n", "");
	else if (reqMap["method"] == "GET" || reqMap["method"] == "HEAD")
		get();
	else if (reqMap["method"] == "POST")
		post();
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

// GET запрос
void HTTP::get()
{
	std::string path = pathFormerer();
//	std::list<Location>::const_iterator it = getMatchingLocation(servConf);
	int fd;
	struct stat structstat;
//	std::vector<std::string>::const_iterator vector_iter = std::find(it->getMethods().begin(), it->getMethods().end(), "GET");

	if (!checkForAllowedMethod())
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
	else if (path == "")
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
	sendReq("HTTP/1.1 200 OK\r\nConnection: Closed\r\n\r\n", buf);
}

int HTTP::sendReq(std::string header, std::string responce)
{
//	std::string result;

	if (reqMap["method"] == "HEAD")
		responce = header;
	result = header + responce;
	std::cout << "Result responce: " << result << std::endl;

	//write(client_fd, (char *)result.c_str(), result.size());
	send(client_fd, (char *)result.c_str(), result.size(), 0);
//	net.send(client_fd, (char *)result.c_str(), result.size());
	return (0);
}

long HTTP::contentLength()
{
	std::map<std::string, std::string>::iterator iterator;

	if ((iterator = reqMap.find("Content-Length")) != reqMap.end())
	{
		for (int i = 0; i < iterator->second.size(); ++i)
		{
			//std::cout << iterator->second.size() << std::endl
			if (!std::isdigit(iterator->second[i]))
				return (-1);
		}
		return (std::strtol(iterator->second.c_str(), NULL, 0));
	}
	return (-1);
}

void HTTP::post()
{
	int fd;
//	long content_length = contentLength();
	File file(reqMap);
	std::string post_root;
	char buf[PATH_MAX];

	if (!checkForAllowedMethod())
	{
		sendReq("HTTP/1.1 405 Method Not Allowed\r\n\r\n", "");
		return;
	}
	if (file.getContentLength() == -1)
	{
		sendReq("HTTP/1.1 411 Length Required\r\n\r\n", "");
		return;
	}
	if (!(it->getRoot().empty()))
		post_root = it->getRoot();
	else if (!(servConf.getRoot().empty()))
		post_root = servConf.getRoot();
	else
		post_root = getcwd(buf, PATH_MAX);
	if (post_root.back() != '/')
		post_root.push_back('/');
	reqMap["location"].erase(0, it->getLocation().size());
	post_root += reqMap["location"];
//	if (file.getMime() != "not_found")
	post_root += file.getMime();
//	else
//	{
//		sendReq("", "");
//	}
std::cout << post_root << std::endl;
	if ((fd = open(post_root.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644)) < 0 ||
			(write(fd, reqMap["body"].c_str(), file.getContentLength())) < 0)
	{
		sendReq("HTTP/1.1 403 Forbidden\r\n\r\n", "");
		return;
	}
	sendReq("HTTP/1.1 200 OK\r\nConnection: Closed\r\n\r\n", "");
}

std::string &HTTP::getResponce()
{
	return (result);
}
