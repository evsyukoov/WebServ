NAME1=server

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
#SRC := $(shell find . -name \*.cpp | grep -v client_main | grep -v test_for_file)

CXX := clang++

ifndef DEBUG
CXXFLAGS := -std=c++98 #-Wall -Wextra -Werror
else
CXXFLAGS := -std=c++98 -D D_REQUEST #-Wall -Wextra -Werror
endif

OBJS=$(SRC:.cpp=.o)

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJS)
	clang++ $(OBJS) -o $(NAME1)

run:
	$(MAKE) DEBUG=1
	./server info.php Config.conf

clean:
	rm -f $(OBJS)

#%.o: %.cpp
	#clang++ -c -Wall -Wextra -Werror -c $< -o $@

fclean: clean
	rm -f $(NAME1)
	rm -f $(NAME2)

re: fclean all

.PHONY: all clean fclean re
