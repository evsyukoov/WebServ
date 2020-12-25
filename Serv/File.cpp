

#include "File.hpp"

long File::contentLength(std::map<std::string, std::string> &reqMap)
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

void 	File::contentLanguage(std::map<std::string, std::string> &reqMap)
{
	std::map<std::string, std::string>::iterator iterator;
	size_t pos = 0;
	std::string 				transform;
	std::vector<std::string>	langauages;
	size_t s_pos = 0;

	if ((iterator = reqMap.find("Content-Language")) != reqMap.end())
	{//pos = reqMap["Content-Language"].find(",", pos)) != -1
		while (pos < reqMap["Content-Language"].size())
		{
			if (!std::isalpha(reqMap["Content-Language"][pos]) && std::isalpha(reqMap["Content-Language"][pos] != ' ')
			{
				if (reqMap["Content-Language"][pos] == ',')
				{
					transform = reqMap["Content-Language"].substr(s_pos, pos - s_pos);
					transform.erase(std::remove(transform.begin(),transform.end(),' '), transform.end());
					std::transform(transform.begin(), transform.end(), transform.begin(), tolower);
					langauages.push_back(transform);
					s_pos = pos + 1;
					pos += 1;
				}
				else
					return;

			}
			pos++;
		}
		content_language = langauages;
	//	pos = reqMap["Content-Language"].find(",", pos);
	}
}

void File::contentType(std::map<std::string, std::string> &reqMap)
{
	std::map<std::string, std::string>::iterator iterator;
	size_t pos = 0;
	std::string 				transform;
	std::vector<std::string>	langauages;
	size_t s_pos = 0;

	if ((iterator = reqMap.find("Content-Type")) != reqMap.end())
	{
		while (pos < reqMap["Content-Type"].size())
		{

		}
	}
}

File::File(std::map<std::string, std::string> &reqMap)
{
	content_length = contentLength(reqMap);
	contentLanguage(reqMap);

}

