
#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <map>
# include <iostream>

class Response
{   

    std::map<std::string, std::string> const    headerMap;
    std::string                                 respLine;
    std::string const                           bodyFilename;
    ssize_t                                     bodyLength;

    int     lseek_next_line(int fd, std::string &line);

public:
    void    setHeaderMap(std::map<std::string, std::string> const map);
    void    setRespLine(std::string const &respLine);
    int     send();
};

#endif
