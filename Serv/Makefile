NAME=server

PRS := $(addprefix Parser/, Config.cpp Location.cpp ServConf.cpp parser_utils.cpp)
ROOT := HTTP.cpp \
	  CGI.cpp   \
	  File.cpp \
	  utils.cpp \
	  utils_for_file.cpp \
	  Client.cpp \
	  Server.cpp \
	  server_main.cpp

SRC = $(PRS) $(ROOT)
	  
DIR = Parser
BIN = ./bin/

conf = $(PWD)/conf/webserv.conf

OBJS = $(addprefix $(BIN), $(SRC:.cpp=.o))
INC = $(addprefix -I , $(SRC:.cpp=.hpp))

CXX := clang++
CXXFLAGS := -std=c++98 $(INC) -Wall -Wextra -Werror

vpath %.cpp Parser  

all: $(NAME)

$(NAME): $(OBJS) | config
	@clang++ $(OBJS) -o $(NAME)

run: $(NAME)
	./server ./CGI_Scrypts/cgi_tester $(conf) 

test:
	./tester http://127.0.0.1:1234

$(BIN)%.o: %.cpp | $(BIN)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "Compiling $(notdir $@)\r"

$(BIN):
	mkdir -p $@ && cd $(BIN) && mkdir $(DIR)

clean:
	rm -rf $(BIN) 

config:
	@cd conf && cat template.conf | sed "s#_PWD_#$(PWD)#" > $(conf)
	@printf "Config created\n"

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_*

re: fclean all

.PHONY: all clean fclean re run test debug config

#debug: fclean
	#$(MAKE) DEBUG=1

#ifdef DEBUG
#CXXFLAGS += -D WS_DEBUG
#endif
#ifeq ($(D_REQUEST), 1)
#CXXFLAGS += -D D_REQUEST
#else ifeq ($(D_CGI), 1)
#CXXFLAGS += -D D_CGI
#endif
