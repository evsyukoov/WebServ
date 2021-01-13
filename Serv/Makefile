NAME=server

SRC = HTTP.cpp \
	  CGI.cpp   \
	  Parser/Config.cpp \
	  Parser/Location.cpp \
	  Parser/ServConf.cpp \
	  File.cpp \
	  utils.cpp \
	  utils_for_file.cpp \
	  Client.cpp \
	  Parser/parser_utils.cpp \
	  Server.cpp \
	  server_main.cpp

CXX := clang++
CXXFLAGS := -std=c++98 #-Wall -Wextra -Werror

ifdef DEBUG
CXXFLAGS += -D WS_DEBUG
endif
ifeq ($(D_REQUEST), 1)
CXXFLAGS += -D D_REQUEST
else ifeq ($(D_CGI), 1)
CXXFLAGS += -D D_CGI
endif

OBJS=$(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) | config
	clang++ $(OBJS) -o $(NAME)

run: $(NAME)
	./server ./CGI_Scrypts/cgi_tester ./conf/webserv.conf

test: re
	./tester http://127.0.0.1:1234

debug: fclean
	$(MAKE) DEBUG=1

clean:
	rm -f $(OBJS)

config:
	cd conf && cat template.conf | sed "s#_PWD_#$(PWD)#" > webserv.conf

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
