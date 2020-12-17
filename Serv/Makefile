NAME1=server
NAME2=client

SRC = Net.cpp \
	  lib.cpp \
	  HttpRequest.cpp \
	  HttpResponse.cpp \
	  CGI.cpp

FLAGS = -Wall -Wextra -Werror

OBJS=$(SRC:.cpp=.o)

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJS)
	clang++ $(OBJS) server_main.cpp -o $(NAME1)

$(NAME2): $(OBJS)
	clang++ $(OBJS) client_main.cpp -o $(NAME2)

clean:
	rm -f $(OBJS)

%.o: %.c
	clang++ -c -Wall -Wextra -Werror -c $< -o $@

fclean: clean
	rm -f $(NAME1)
	rm -f $(NAME2)

re: fclean all

.PHONY: all clean fclean re
