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


FLAGS = -Wall -Wextra -Werror

OBJS=$(SRC:.cpp=.o)

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJS)
	clang++ $(OBJS) -o $(NAME1)


clean:
	rm -f $(OBJS)

%.o: %.c
	clang++ -c -Wall -Wextra -Werror -c $< -o $@

fclean: clean
	rm -f $(NAME1)
	rm -f $(NAME2)

re: fclean all

.PHONY: all clean fclean re
