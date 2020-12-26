

#include "File.hpp"

long File::contentLength(std::map<std::string, std::string> &reqMap)
{
	size_t length;

	if (reqMap.find(LENGTH) != reqMap.end())
	{
		for (int i = 0; i < reqMap[LENGTH].size(); ++i)
		{
			if (!std::isdigit(reqMap[LENGTH][i]))
				return (-1);
		}
		length = std::strtoll(reqMap[LENGTH].c_str(), NULL, 0);
		if (errno == ERANGE)
			return (-1);
		return (length);
	}
	return (-1);
}

void 	File::contentWithComma(std::map<std::string, std::string> &reqMap, std::string base)
{
	std::vector<std::string>	valid_vector;

	if (reqMap.find(base) != reqMap.end())
	{
		valid_vector = ft_split(reqMap[base], ",");
		for (size_t pos = 0; pos < valid_vector[pos].size(); pos++)
		{
			trimmer(valid_vector[pos]);
			std::transform(valid_vector[pos].begin(), valid_vector[pos].end(), valid_vector[pos].begin(), tolower);
			if (std::find_if_not(valid_vector[pos].begin(), valid_vector[pos].end(), predicate)
			!= valid_vector[pos].end())
				return;
		}
		if (base == LANG)
			content_language = valid_vector;
		else if (base == ENCODE)
			content_encoding = valid_vector;
	}
}

bool File::typeValidity(std::vector<std::string> &charset_vector, std::vector<std::string> &type_vector)
{
	if (charset_vector.size() != 2)
		return (false);
	if (charset_vector.size() > 0 && charset_vector[0] != "charset")
		return (false);
	if (charset_vector.size() > 1 && std::find_if_not(charset_vector[1].begin(), charset_vector[1].end(), charsetPredicate)
	!= charset_vector[1].end())
		return (false);
	if (type_vector.size() != 2)
		return (false);
	if (std::find_if_not(type_vector[0].begin(), type_vector[0].end(), charsetPredicate)
		!= type_vector[0].end() || std::find_if_not(type_vector[1].begin(), type_vector[1].end(), charsetPredicate)
		!= type_vector[1].end())
		return (false);
	return (true);


}

void File::contentType(std::map<std::string, std::string> &reqMap)
{
//	std::map<std::string, std::string>::iterator iterator;
	std::vector<std::string>    start_vector;
	std::vector<std::string>	type_vector;
	std::vector<std::string>	charset_vector;

	if (reqMap.find(TYPE) != reqMap.end())
	{
		start_vector = ft_split(reqMap[TYPE], ";");
		if (start_vector.size() > 0)
		{
			type_vector = ft_split(start_vector[0], "/");
			if (type_vector.size() == 2)
			{
				trimmer(type_vector[0]);
				trimmer(type_vector[1]);
			}
			if (start_vector.size() == 2)
			{
				trimmer(start_vector[1]);
				charset_vector = ft_split(start_vector[1], "=");
			}
			if (!typeValidity(charset_vector, type_vector))
				return;
			content_type = type_vector[0] + "/" + type_vector[1];
			if (charset_vector.size() > 1)
				charset = charset_vector[1];
		}
	}
}

File::File(std::map<std::string, std::string> &reqMap)
{
	content_length = contentLength(reqMap);
	contentWithComma(reqMap, LANG);
	contentWithComma(reqMap, ENCODE);
	contentType(reqMap);
}

const long &File::getContentLength() { return (content_length); }

const std::vector<std::string> &File::getContentLanguage() { return (content_language); }

const std::vector<std::string> &File::getContentEncoding() { return (content_encoding); }

const std::string &File::getContentType() { return (content_type); }

const std::string &File::getCharset() { return (charset); }
