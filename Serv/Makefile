NAME := server

PRS := $(addprefix Parser/, Config.cpp Location.cpp ServConf.cpp parser_utils.cpp)
ROOT := HTTP.cpp \
	  CGI.cpp   \
	  File.cpp \
	  utils.cpp \
	  utils_for_file.cpp \
	  Client.cpp \
	  Server.cpp \
	  Response.cpp \
	  server_main.cpp

SRC := $(PRS) $(ROOT)
	  
DIR := Parser
BIN := ./bin/

conf := $(PWD)/conf/webserv.conf

OBJS := $(addprefix $(BIN), $(SRC:.cpp=.o))
INC := $(addprefix -I , $(dir $(SRC)))

CXX := clang++
CXXFLAGS := -std=c++98 $(INC) -Wall -Wextra -Werror

vpath %.cpp Parser  

all: $(NAME)

$(NAME): $(OBJS)
	@clang++ $^ -o $@
	@printf "%s%-40s\n" "[$(DOTS)]" "Compiling $@"

run: $(NAME) config
	@./$< $(conf) 

test:
	./tester http://127.0.0.1:1234

$(BIN)%.o: %.cpp | $(BIN)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	$(eval DOTS := .$(DOTS))
	@printf "%s%-40s\r" "[$(DOTS)]" "Compiling $(notdir $@)"


$(BIN):
	mkdir -p $@ && cd $(BIN) && mkdir $(DIR)

clean:
	rm -rf $(BIN) 

config:
	@cd conf && cat template.conf | sed "s#_PWD_#$(PWD)#" > $(conf)
	@printf "%40s\n" "Config created"

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_* multiple_same

re: fclean $(BIN) all

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
