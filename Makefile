#<><><><><><><> Files <><><><><><><><><><><><><><><><><><><>
ROOT		= $(abspath $(CURDIR))

SRC_DIR 	= $(ROOT)/src/
BUILD_DIR 	= $(ROOT)/build/
INC_DIR 	= $(ROOT)/include/
LIBFT_DIR 	= $(ROOT)/libft/
TEST_DIR	= $(ROOT)/test/

LOGFILE		:= $(ROOT)/nm.log

TESTFILE	:= $(TEST_DIR)test.c
TESTNAME	:= $(TEST_DIR)test_elf

# .c files for source code
SRC_FILES_NAMES = main.c

SRC_FILES_NAMES += parser/parse_cli.c
SRC_FILES_NAMES += parser/parse_elf.c
SRC_FILES_NAMES += parser/parse_elf_symbols.c

SRC_FILES_NAMES += utils/utils.c
SRC_FILES_NAMES += utils/map_utils.c

# Full path to .c files
SRC_FILES = $(addprefix $(SRC_DIR), $(SRC_FILES_NAMES))

# .o files for compilation
OBJ_FILES = $(patsubst $(SRC_DIR)%.c, $(BUILD_DIR)%.o, $(SRC_FILES))

# .d files for header dependency
DEP_FILES := $(OBJ_FILES:.o=.d)


#<><><><><><><> Variables <><><><><><><><><><><><><><><><><>
NAME 	:= ft_nm
CC 	:= cc
IFLAGS	:= -I $(INC_DIR) -I $(LIBFT_DIR)/includes/
CFLAGS 	:= -gdwarf-4 -Wall -Wextra -Werror $(IFLAGS) -MMD -MP -DLOGFILE_PATH=\"$(LOGFILE)\" -g3
LFLAGS 	:= libft/libft.a 
MKDIR 	:= mkdir -p
RM_RF 	:= rm -rf
ECHO	:= printf '%b\n'

BLUE	:= \033[34m
BROWN	:= \033[33m
GREEN	:= \033[32m
RED	:= \033[31m
NC	:= \033[0m

#<><><><><><><> Recipes <><><><><><><><><><><><><><><><><><>

# Modifying Implicit conversion rules to build in custom directory
$(BUILD_DIR)%.o : $(SRC_DIR)%.c
	@$(MKDIR) $(dir $@)
	@$(ECHO) "$(BLUE)[CMP] Compiling \b$(subst $(ROOT)/, ,$<)...$(NC)"
	@$(CC) -c $(CFLAGS) $< -o $@ 


# This is to add the .d files as dependencies for linking
-include $(DEP_FILES)

re : fclean all 

$(NAME) : $(OBJ_FILES)
	@$(ECHO) "$(BROWN)[BLD] Building libft static library...$(NC)"
	@$(MAKE) --no-print-directory -s -C $(LIBFT_DIR) all
	@$(ECHO) "$(GREEN)[BLD] successfully built libft.$(NC)"	

	@$(ECHO) "$(BROWN)[BLD] Building $(NAME) executable...$(NC)"
	@$(CC) $^ -o $(NAME) $(LFLAGS)
	@$(ECHO) "$(GREEN)[BLD] Executable built successfully.$(NC)"


all : $(NAME) 

test :
	@$(ECHO) "$(BROWN)[BLD] Building test binary...$(NC)"
	@$(ECHO) "$(BLUE)[CMP] Compiling \b$(subst $(ROOT)/, ,$(TESTFILE))...$(NC)"
	@$(CC) -g3 -O0 $(TESTFILE) -o $(TESTNAME)
	@$(ECHO) "$(GREEN)[BLD] successfully built test binary...$(NC)"


clean : 
	@$(ECHO) "$(BROWN)[CLN] Cleaning object and dependency files...$(NC)"
	@$(RM) $(DEP_FILES) $(OBJ_FILES)
	@$(ECHO) "$(GREEN)[CLN] Clean complete.$(NC)"

fclean : 
	@$(ECHO) "$(BROWN)[CLN] Cleaning object, dependency files, and library...$(NC)"
	@$(RM_RF) $(BUILD_DIR) $(NAME)
	@$(ECHO) "$(GREEN)[CLN] Clean complete.$(NC)"
	@$(ECHO) # for newline

.DEFAULT_GOAL := all
.PHONY : all clean fclean re test
