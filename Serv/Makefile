NAME := server

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

PRS := Config.cpp Location.cpp ServConf.cpp parser_utils.cpp
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
BIN := $(mkfile_dir)/bin/

OBJS := $(addprefix $(BIN), $(SRC:.cpp=.o))
INC := $(foreach d, $(DIR), -I$d )

CXX := clang++
CXXFLAGS := -std=c++98 $(INC) -Wall -Wextra -Werror

testr := $(mkfile_dir)/tester

conf := $(mkfile_dir)/conf/webserv.conf

vpath %.cpp $(DIR)

all: $(NAME)

$(NAME): $(OBJS)
	@clang++ $^ -o $@
	@printf "%s%-40s\n" "[$(DOTS)]" "Compiling $@"

run: $(NAME) config
	@./$< $(conf) 

test: $(testr)
	@if ! [[ -f $< ]]; \
		then curl https://projects.intra.42.fr/uploads/document/document/2464/tester --output $<; \
	fi
	chmod u+x $<
	$< http://127.0.0.1:1234

$(BIN)%.o: %.cpp | $(BIN)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	$(eval DOTS := .$(DOTS))
	@printf "%s%-40s\r" "[$(DOTS)]" "Compiling $(notdir $@)"


$(BIN):
	mkdir -p $@ && cd $(BIN) && mkdir $(DIR)

clean:
	rm -rf $(BIN) 

config:
	@cd conf && cat template.conf | sed "s#_PWD_#$(mkfile_dir)#" > $(conf)
	@printf "%40s\n" "Config created"

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_* multiple_same

re: fclean $(BIN) all

.PHONY: all clean fclean re run test debug config $(testr)

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
