NAME := webserv

# Makefile absolute path
mkfile_path := $(abspath $(MAKEFILE_LIST))
mkfile_dir := $(dir $(mkfile_path))

SRC := Config.cpp Location.cpp ServConf.cpp parser_utils.cpp \
      HTTP.cpp CGI.cpp File.cpp \
	  utils.cpp utils_for_file.cpp \
	  Server.cpp Client.cpp Response.cpp \
	  main.cpp

# All project directories
DIR := Parser Request-Response Request_processing Utils

CXX := clang++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror -MMD

testr := $(mkfile_dir)tester
conf := $(mkfile_dir)conf/webserv.conf

# Add Server to directories if normal, add Bonus if it's bonus
ifndef WITH_BONUS
    DIR += Server
    BIN := $(mkfile_dir)bin/
else
	BIN := $(mkfile_dir)bin/bonus/
    SRC += Worker.cpp
    DIR += Bonus
    CXXFLAGS += -D BONUS=1
endif

# Add prefix to obj. Result: ./bin/%.o
OBJS := $(addprefix $(BIN), $(SRC:.cpp=.o))
INC := $(foreach d, $(DIR), -I$d )

# Include header dependencies if any
-include $(OBJS:.o=.d)

# Look for SRC files in any project directories
vpath %.cpp $(DIR)

# Make an obj directory and run name
all: bindir $(NAME)

# Compile webserv. depends on OBJ files
$(NAME): $(OBJS)
	@clang++ $^ -o $@
	@printf "%s%-40s\n" "[$(DOTS)]" "Compiling $@"

run: $(NAME)
	@./$< ./conf/main.conf

# Check if tester is here and download it if not.
test: $(testr)
	@if ! [[ -f $< ]]; then curl https://projects.intra.42.fr/uploads/document/document/2464/tester --output $<; fi
	chmod u+x $<
	$< http://127.0.0.1:1234

# Compile .cpp files ($<), output to .o files ($@)
$(BIN)%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<
	$(eval DOTS := .$(DOTS))
	@printf "%s%-30s\r" "[$(DOTS)]" "Compiling $(notdir $@)"

config:
	sed s#_PWD_#$(PWD)# ./conf/template.conf > $(conf)

bindir: $(BIN)

$(BIN):
	@mkdir -p $@

# Run make with BONUS variable enabled
bonus:
	$(MAKE) 'WITH_BONUS=1' all

clean:
	rm -rf $(BIN) 

fclean: clean
	rm -f $(NAME) $(conf) file_should_exist_after ws_* multiple_same

re: fclean all

.PHONY: all clean fclean re run test debug $(testr) bonus bindir
