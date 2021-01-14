//
// Created by Casie Carl on 12/25/20.
//

#include "utils.hpp"

long  findFileSize(int fd)
{
    struct stat file_stat;
    int res = fstat(fd, &file_stat);
    if (res < 0)
        return (-1);
    return file_stat.st_size;
}

void    inet_toip4(void *sAddr, std::string &buffer)
{
    unsigned char *sss = reinterpret_cast<unsigned char *>(sAddr);

    buffer.clear();
    for (int i = 0; i < 4; ++i)
        buffer += std::to_string(int(sss[i])) + '.';
    buffer.pop_back();
}

std::string    stringExtract(std::string const &needle, std::string const &haystack)
{
    size_t delim = haystack.find(needle);
    std::string tmp, result;

    if (delim != std::string::npos)
    {
        tmp = haystack.substr(0, delim - 1);
        result = tmp + haystack.substr(delim + needle.size(), haystack.size());
    }
    return result;
}

std::string     trimAfter(std::string const &str, char const &c)
{
    size_t delim = str.find(c);

    if (delim != str.npos)
        return str.substr(delim + 1, str.npos); 
    return str;
}

std::string     trimBefore(std::string const &str, char const &c)
{
    size_t delim = str.rfind(c);
    
    if (delim != str.npos)
        return str.substr(0, str.rfind(c) + 1); 
    return str;
}

std::ostream    &operator<<(std::ostream &os, std::map<std::string, std::string> const &m)
{
    std::map<std::string, std::string>::const_iterator it = m.begin();
    while (it != m.end())
    {
        os << it->first << ": " << it->second << std::endl;
        ++it;
    }
    return os;
}

std::string       headerPairToStr(std::pair<std::string, std::string> const &pair)
{
    return (pair.first + ": " + pair.second + "\r\n");
}

std::string       headerPairToStr(std::string const &first, std::string const &second)
{
    return (first + ": " + second + "\r\n");
}

int                 lseek_next_line(int fd, std::string &line)
{
	static std::string  reaminder;
	size_t              endl = reaminder.find('\n');
	ssize_t             len;
	char                *buf;
	int                 rv;
	buf = new char[BUFFER_SIZE + 1];
	while ((rv = read(fd, buf, BUFFER_SIZE)) >= 0)
	{
		buf[rv] = '\0';
		reaminder += buf;
		if (endl != reaminder.npos || rv == 0)
		{
			delete buf;
			if (endl == line.npos)
			{
				if (reaminder.size() == 0)
					return (0);
				endl = line.size();
			}
			line = reaminder.substr(0, reaminder.find('\n'));
			len = line.size() - reaminder.size() + 1;
			reaminder.clear();
			lseek(fd, len, SEEK_CUR);
			return (1);
		}
	}
	delete buf;
	return (-1);
}

size_t ft_strlen(char const *s)
{
	char const *e = s;

	while (*e)
		e++;
	return (e - s);
}

char *ft_strdup(char const *s)
{
	char *res = (char *)malloc(ft_strlen(s) + 1);
	int i = 0;

	if (res)
	{
		for (i = 0; s[i]; ++i)
			res[i] = s[i];
		res[i] = 0;
	}
	return (res);
}
