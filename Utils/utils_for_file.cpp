

#include "utils_for_file.hpp"

bool predicate(char c)
{
	if (std::isalnum(c) || c == '-')
		return (true);
	return (false);
}

bool acceptPredicete(char c)
{
	if (std::isalnum(c) || c == '-' || c == '=' || c == ';' || c == ' ' || c == '.' || c == '*')
		return (true);
	return (false);
}

bool charsetPredicate(char c)
{
	if (std::isprint(c) && c != ' ')
		return (true);
	return (false);
}

bool comparePriors(const std::pair<std::string, float>& a, const std::pair<std::string, float>& b)
{
	return (a.second < b.second);
}

void trimmer(std::string &str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ') + 1, str.size() - 1);
}

std::vector<std::string>	ft_split(std::string str, std::string delimetr)
{
	std::vector<std::string> vect;
	size_t pos = str.find(delimetr);
	size_t s_pos = 0;

	while (pos != std::string::npos)
	{
		vect.push_back(str.substr(s_pos, pos - s_pos));
		s_pos = pos + 1;
        pos = str.find(delimetr, pos + 1);
	}
	vect.push_back(str.substr(s_pos, str.size() - s_pos));
	return (vect);
}