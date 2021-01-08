

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
	if (charset_vector.size() != 2 && charset_vector.size() != 0)
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
	else
		content_type = "application/octet-stream";
}

std::string File::getMime()
{
	std::map<std::string, std::string> mime_map;

	mime_map["audio/aac"] = ".aac";
	mime_map["application/x-abiword"] = ".abw";
	mime_map["application/x-freearc"] = ".arc";
	mime_map["video/x-msvideo"] = ".avi";
	mime_map["application/vnd.amazon.ebook"] = ".azw";
	mime_map["application/octet-stream"] = ".bin";
	mime_map["image/bmp"] = ".bmp";
	mime_map["application/x-bzip"] = ".bz";
	mime_map["application/x-bzip2"] = ".bz2";
	mime_map["application/x-csh"] = ".csh";
	mime_map["text/css"] = ".css";
	mime_map["text/csv"] = ".csv";
	mime_map["application/msword"] = ".doc";
	mime_map["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = ".docx";
	mime_map["application/vnd.ms-fontobject"] = ".eot";
	mime_map["application/epub+zip"] = ".epub";
	mime_map["application/gzip"] = ".gz";
	mime_map["image/gif"] = ".gif";
	mime_map["text/html"] = ".html";
	mime_map["image/vnd.microsoft.icon"] = ".ico";
	mime_map["text/calendar"] = ".ics";
	mime_map["application/java-archive"] = ".jar";
	mime_map["image/jpeg"] = ".jpg";
	mime_map["text/javascript"] = ".js";
	mime_map["application/json"] = ".json";
	mime_map["application/ld+json"] = ".jsonld";
	mime_map["audio/midi"] = ".mid";
	mime_map["audio/x-midi"] = ".midi";
	mime_map["text/javascript"] = ".mjs";
	mime_map["audio/mpeg"] = ".mp3";
 	mime_map["video/mpeg"] = ".mpeg";
 	mime_map["application/vnd.apple.installer+xml"] = ".mpkg";
 	mime_map["application/vnd.oasis.opendocument.presentation"] = ".odp";
 	mime_map["application/vnd.oasis.opendocument.spreadsheet"] = ".ods";
 	mime_map["application/vnd.oasis.opendocument.text"] = ".odt";
 	mime_map["audio/ogg"] = ".oga";
 	mime_map["video/ogg"] = ".ogv";
 	mime_map["application/ogg"] = ".ogx";
 	mime_map["audio/opus"] = ".opus";
 	mime_map["font/otf"] = ".otf";
 	mime_map["image/png"] = ".png";
 	mime_map["application/pdf"] = ".pdf";
 	mime_map["application/php"] = ".php";
 	mime_map["application/vnd.ms-powerpoint"] = ".ppt";
 	mime_map["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = ".pptx";
 	mime_map["application/vnd.rar"] = ".rar";
 	mime_map["application/rtf"] = ".rtf";
 	mime_map["application/x-sh"] = ".sh";
 	mime_map["image/svg+xml"] = ".svg";
 	mime_map["application/x-shockwave-flash"] = ".swf";
 	mime_map["application/x-tar"] = ".tar";
 	mime_map["image/tiff"] = ".tif";
 	mime_map["video/mp2t"] = ".ts";
 	mime_map["font/ttf"] = ".ttf";
 	mime_map["text/plain"] = ".txt";
 	mime_map["application/vnd.visio"] = ".vsd";
 	mime_map["audio/wav"] = ".wav";
 	mime_map["audio/webm"] = ".weba";
 	mime_map["image/webp"] = ".webp";
 	mime_map["font/woff"] = ".woff";
 	mime_map["font/woff2"] = ".woff2";
 	mime_map["application/xhtml+xml"] = ".xhtml";
 	mime_map["application/vnd.ms-excel"] = ".xls";
 	mime_map["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = ".xlsx";
 	mime_map["application/xml"] = ".xml";
 	mime_map["text/xml"] = ".xml";
 	mime_map["application/vnd.mozilla.xul+xml"] = ".xul";
 	mime_map["application/zip"] = ".zip";
 	mime_map["video/3gpp"] = ".3gp";
 	mime_map["audio/3gpp"] = ".3gp";
 	mime_map["video/3gpp2"] = ".3g2";
 	mime_map["audio/3gpp2"] = ".3g2";
 	mime_map["application/x-7z-compressed"] = ".7z";
 	mime_map["application/octet-stream"] = "";

 	if (mime_map.find(content_type) != mime_map.end())
		return (mime_map[content_type]);
	return ("");
}

File::File(std::map<std::string, std::string> &reqMap)
{
	content_length = contentLength(reqMap);
	contentWithComma(reqMap, LANG);
	contentWithComma(reqMap, ENCODE);
	contentType(reqMap);
}

long File::getContentLength() { return (content_length); }

const std::vector<std::string> &File::getContentLanguage() { return (content_language); }

const std::vector<std::string> &File::getContentEncoding() { return (content_encoding); }

const std::string &File::getContentType() { return (content_type); }

const std::string &File::getCharset() { return (charset); }
