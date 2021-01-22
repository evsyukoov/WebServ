

mkdir ./Server

cp -R ../Serv/website/ ./Server/website/ && \
 cp -R ../Serv/CGI_Scrypts/ ./Server/CGI_Scrypts/ && \
 cp ../Serv/Config.conf ./Server/ && \
 cp ../Serv/*.cpp ./Server/ && \
 cp ../Serv/*.hpp ./Server/ && \
 cp ../Serv/Makefile ./Server/ && \
 cp -R ../Serv/Parser ./Server/Parser


make -C ../Serv/ && \
 cp ../Serv/server ./Server/


docker build -t webserv .


make fclean -C ../Serv/ 

rm -rf Server

