
#include "File.hpp"

int main()
{
	std::map<std::string, std::string> map;
	std::vector<File>		file_vector;
	std::vector<std::string>::const_iterator it;
	std::vector<File>::iterator f_it;

	map["Content-Language"] = "    en,d-54a,                     en-su, TYRW";
	map["Content-Length"] = "123549867945";
	map["Content-Encoding"] = ",  gzip, cpmpr";
	map["Content-Type"] = "type          /             subtype         ;          charset=tuf32-32";

	File new_file(map);
	file_vector.push_back(new_file);

	f_it = file_vector.begin();
	while (f_it != file_vector.end())
	{
		std::cout << "Lenth: " << f_it->getContentLength() << std::endl;
		it = f_it->getContentLanguage().begin();
		while (it != f_it->getContentLanguage().end())
		{
			std::cout << "Languages: " << *it << std::endl;
			it++;
		}
		it = f_it->getContentEncoding().begin();
		while (it != f_it->getContentEncoding().end())
		{
			std::cout << "Encoding: " << *it << std::endl;
			it++;
		}
		std::cout << "Content type: " << f_it->getContentType() << std::endl;
		std::cout << "Charset: " << f_it->getCharset() << std::endl;
		f_it++;
	}
}


