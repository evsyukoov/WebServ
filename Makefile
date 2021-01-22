NAME := webserv

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

SRC := Config.cpp Location.cpp ServConf.cpp parser_utils.cpp \
        HTTP.cpp CGI.cpp File.cpp \
	  utils.cpp utils_for_file.cpp \
	  Server.cpp Client.cpp Response.cpp \
	  main.cpp

DIR := Parser Server Request-Response Request_processing Utils
BIN := $(mkfile_dir)bin/

OBJS := $(addprefix $(BIN), $(SRC:.cpp=.o))
INC := $(foreach d, $(DIR), -I$d )

CXX := clang++
CXXFLAGS := -std=c++98 $(INC) -Wall -Wextra -Werror

testr := $(mkfile_dir)tester

conf := $(mkfile_dir)conf/webserv.conf

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
	@printf "%s\n" "Config created"

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_* multiple_same

re: fclean $(BIN) all

.PHONY: all clean fclean re run test debug config $(testr)
