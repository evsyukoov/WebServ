NAME := webserv

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

SRC := Config.cpp Location.cpp ServConf.cpp parser_utils.cpp \
        HTTP.cpp CGI.cpp File.cpp \
	  utils.cpp utils_for_file.cpp \
	  Server.cpp Client.cpp Response.cpp \
	  main.cpp

DIR := Parser Request-Response Request_processing Utils
BIN := $(mkfile_dir)bin/

CXX := clang++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror

testr := $(mkfile_dir)tester

conf := $(mkfile_dir)conf/webserv.conf

ifndef WITH_BONUS
    DIR += Server
    BIN := $(mkfile_dir)bin/
else
	BIN := $(mkfile_dir)bin_bonus/
    SRC += Worker.cpp
    DIR += Bonus
    CXXFLAGS += -D BONUS=1
endif

OBJS := $(addprefix $(BIN), $(SRC:.cpp=.o))
INC := -I. $(foreach d, $(DIR), -I$d )

vpath %.cpp $(DIR)

all: bindir $(NAME)

$(NAME): $(OBJS)
	@clang++ $^ -o $@
	@printf "%s%-40s\n" "[$(DOTS)]" "Compiling $@"

run: $(NAME)
	@./$< ./conf/main.conf

test: $(testr)
	@if ! [[ -f $< ]]; \
		then curl https://projects.intra.42.fr/uploads/document/document/2464/tester --output $<; \
	fi
	chmod u+x $<
	$< http://127.0.0.1:1234

$(BIN)%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	$(eval DOTS := .$(DOTS))
	@printf "%s%-40s\r" "[$(DOTS)]" "Compiling $(notdir $@)"

bindir:
	@mkdir -p $(BIN)

bonus:
	$(MAKE) 'WITH_BONUS=1' all

clean:
	rm -rf $(BIN) 

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_* multiple_same

re: fclean all

.PHONY: all clean fclean re run test debug $(testr) bonus bindir
