NAME			=	minishell
CC				=	cc
LEAKSANITIZER	=	-g3 -fsanitize=leak -fsanitize=address -fno-omit-frame-pointer
CFLAGS			=	-Wall -Wextra -Werror -g -Iinclude
RM				=	rm -rf
HEADERS			=	include/minishell.h

# Directories
SRC_DIR		=	src
OBJ_DIR		=	obj
INC_DIR		=	include

# Core directories
CORE_DIR	=	$(SRC_DIR)/core
PARSER_DIR	=	$(SRC_DIR)/parser
TOKEN_DIR	=	$(SRC_DIR)/tokenizer
BUILTIN_DIR	=	$(SRC_DIR)/builtins
EXEC_DIR	=	$(SRC_DIR)/executor
SIGNAL_DIR	=	$(SRC_DIR)/signals
EXPANDER_DIR=	$(SRC_DIR)/expander
UTILS_DIR	=	$(SRC_DIR)/utils
IO_DIR		=	$(SRC_DIR)/io

# Libft config
LIBFT_DIR	=	libft
LIBFT		=	$(LIBFT_DIR)/libft.a
LIBFT_INC	=	$(LIBFT_DIR)

# Readline
READLINE_INC	=	-I/usr/include/readline
READLINE_LIB	=	-lreadline

# Source files organized by functionality
# Core files - main entry point and shell loop
CORE_SRC	=	main.c \
			shell_loop.c \
			init.c \
			prepare_cmd.c

# Parser files - input parsing and validation
PARSER_SRC	=	parsing.c \
			syntax_checker.c \
			quote_handler.c

# Tokenizer files - converting input to tokens
TOKEN_SRC	=	tokenize.c \
			token_utils.c \
			syntax_check.c \
			process_redirection.c \
			tokenize_utils.c

# Built-in command files - all shell built-ins
BUILTIN_SRC	=	echo.c \
			cd.c \
			pwd.c \
			env.c \
			export.c \
			unset.c \
			exit.c \
			builtins_utils.c

# Executor files - command execution and process management
EXEC_SRC	=	command_lookup.c \
			executor.c \
			command_handler.c \
			pipe_handler.c \
			process_manager.c \
			parse_commands.c \
			heredoc_utils.c

# Signal handling files
SIGNAL_SRC	=	signals.c \
			signals_utils.c

# Variable expansion files
EXPANDER_SRC =	expander.c \
			variable_expansion.c \
			quote_expansion.c

# I/O and redirection files
IO_SRC		=	input_handler.c \
			redirection.c \
			heredoc.c

# Utility files - helper functions
UTILS_SRC	=	env_utils.c \
			utils.c \
			string_utils.c \
			validation.c

# Combine all sources with their directory paths
CORE_FILES		=	$(addprefix $(CORE_DIR)/, $(CORE_SRC))
PARSER_FILES	=	$(addprefix $(PARSER_DIR)/, $(PARSER_SRC))
TOKEN_FILES		=	$(addprefix $(TOKEN_DIR)/, $(TOKEN_SRC))
BUILTIN_FILES	=	$(addprefix $(BUILTIN_DIR)/, $(BUILTIN_SRC))
EXEC_FILES		=	$(addprefix $(EXEC_DIR)/, $(EXEC_SRC))
SIGNAL_FILES	=	$(addprefix $(SIGNAL_DIR)/, $(SIGNAL_SRC))
EXPANDER_FILES	=	$(addprefix $(EXPANDER_DIR)/, $(EXPANDER_SRC))
IO_FILES		=	$(addprefix $(IO_DIR)/, $(IO_SRC))
UTILS_FILES		=	$(addprefix $(UTILS_DIR)/, $(UTILS_SRC))

# All source files
SRC_FILES	=	$(CORE_FILES) \
			$(PARSER_FILES) \
			$(TOKEN_FILES) \
			$(BUILTIN_FILES) \
			$(EXEC_FILES) \
			$(SIGNAL_FILES) \
			$(EXPANDER_FILES) \
			$(IO_FILES) \
			$(UTILS_FILES)

# Object files (maintain directory structure in obj/)
CORE_OBJS		=	$(CORE_FILES:$(CORE_DIR)/%.c=$(OBJ_DIR)/core/%.o)
PARSER_OBJS		=	$(PARSER_FILES:$(PARSER_DIR)/%.c=$(OBJ_DIR)/parser/%.o)
TOKEN_OBJS		=	$(TOKEN_FILES:$(TOKEN_DIR)/%.c=$(OBJ_DIR)/tokenizer/%.o)
BUILTIN_OBJS	=	$(BUILTIN_FILES:$(BUILTIN_DIR)/%.c=$(OBJ_DIR)/builtins/%.o)
EXEC_OBJS		=	$(EXEC_FILES:$(EXEC_DIR)/%.c=$(OBJ_DIR)/executor/%.o)
SIGNAL_OBJS		=	$(SIGNAL_FILES:$(SIGNAL_DIR)/%.c=$(OBJ_DIR)/signals/%.o)
EXPANDER_OBJS	=	$(EXPANDER_FILES:$(EXPANDER_DIR)/%.c=$(OBJ_DIR)/expander/%.o)
IO_OBJS			=	$(IO_FILES:$(IO_DIR)/%.c=$(OBJ_DIR)/io/%.o)
UTILS_OBJS		=	$(UTILS_FILES:$(UTILS_DIR)/%.c=$(OBJ_DIR)/utils/%.o)

# All object files
OBJS		=	$(CORE_OBJS) \
			$(UTILS_OBJS) \
			$(IO_OBJS) \
			$(SIGNAL_OBJS) \
			$(PARSER_OBJS) \
			$(EXEC_OBJS) \
			$(TOKEN_OBJS) \
			$(BUILTIN_OBJS) \
			$(EXPANDER_OBJS) \
			

# Compilation flags
CFLAGS		+=	-I$(INC_DIR) -I$(LIBFT_INC) $(READLINE_INC)
LDFLAGS		=	$(LIBFT) $(READLINE_LIB)

# Colors for output
GREEN	= \033[1;32m
CYAN	= \033[1;36m
YELLOW	= \033[1;33m
RED		= \033[1;31m
BLUE	= \033[1;34m
MAGENTA	= \033[1;35m
RESET	= \033[0m

# Progress tracking
TOTAL_FILES := $(words $(SRC_FILES))
COMPILED_FILES = 0

define update_progress
	$(eval COMPILED_FILES := $(shell echo $$(($(COMPILED_FILES) + 1))))
	@printf "$(CYAN)[%2d/%d]$(RESET) Compiling $(notdir $<)...\n" $(COMPILED_FILES) $(TOTAL_FILES)
endef

# Error handler
define check_error
	@if [ $$? -ne 0 ]; then \
		echo "$(RED)[MINISHELL] ❌ Build failed!$(RESET)"; \
		exit 1; \
	fi
endef

# Main targets
all:
	@echo "$(GREEN)╔══════════════════════════════════════════════╗$(RESET)"
	@echo "$(GREEN)║                MINISHELL BUILD               ║$(RESET)"
	@echo "$(GREEN)╚══════════════════════════════════════════════╝$(RESET)"
	@$(MAKE) --no-print-directory $(NAME) || { echo "$(RED)[MINISHELL] ❌ Build failed$(RESET)"; exit 1; }

$(NAME):	$(OBJS) $(LIBFT)
	@echo "$(BLUE)🔗 Linking executable...$(RESET)"
	@$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)
	$(call check_error)
	@echo "$(GREEN)✅ Build complete! Executable: $(NAME)$(RESET)"
	@echo "$(GREEN)╔══════════════════════════════════════════════╗$(RESET)"
	@echo "$(GREEN)║                 BUILD SUCCESS                ║$(RESET)"
	@echo "$(GREEN)╚══════════════════════════════════════════════╝$(RESET)"

# Core module compilation
$(OBJ_DIR)/core/%.o: $(CORE_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Parser module compilation
$(OBJ_DIR)/parser/%.o: $(PARSER_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Tokenizer module compilation
$(OBJ_DIR)/tokenizer/%.o: $(TOKEN_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Builtins module compilation
$(OBJ_DIR)/builtins/%.o: $(BUILTIN_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Executor module compilation
$(OBJ_DIR)/executor/%.o: $(EXEC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Signals module compilation
$(OBJ_DIR)/signals/%.o: $(SIGNAL_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Expander module compilation
$(OBJ_DIR)/expander/%.o: $(EXPANDER_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# I/O module compilation
$(OBJ_DIR)/io/%.o: $(IO_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Utils module compilation
$(OBJ_DIR)/utils/%.o: $(UTILS_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(call update_progress)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Build libft
$(LIBFT):
	@echo "$(MAGENTA)📚 Building libft...$(RESET)"
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

# Utility targets
clean:
	@echo "$(YELLOW)🧹 Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)🗑️  Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:
	@echo "$(CYAN)♻️  Rebuilding everything...$(RESET)"
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

# Development helpers
setup_dirs:
	@echo "$(BLUE)📁 Creating project structure...$(RESET)"
	@mkdir -p $(CORE_DIR) $(PARSER_DIR) $(TOKEN_DIR) $(BUILTIN_DIR)
	@mkdir -p $(EXEC_DIR) $(SIGNAL_DIR) $(EXPANDER_DIR) $(IO_DIR) $(UTILS_DIR)
	@echo "$(GREEN)✅ Directory structure created!$(RESET)"

info:
	@echo "$(CYAN)Project Information:$(RESET)"
	@echo "	Name: $(NAME)"
	@echo "	Source files: $(words $(SRC_FILES))"
	@echo "	Modules: core, parser, tokenizer, builtins, executor, signals, expander, io, utils"
	@echo "	Compiler: $(CC)"
	@echo "	Flags: $(CFLAGS)"

# Debug target
debug: CFLAGS += -DDEBUG -g3
debug: $(NAME)

# Show help
help:
	@echo "$(GREEN)Available targets:$(RESET)"
	@echo "	all			- Build the project"
	@echo "	clean		- Remove object files"
	@echo "	fclean		- Remove object files and executable"
	@echo "	re			- Rebuild everything"
	@echo "	setup_dirs	- Create project directory structure"
	@echo "	debug		- Build with debug flags"
	@echo "	info		- Show project information"
	@echo "	help		- Show this help"

.PHONY: all clean fclean re setup_dirs info debug help
