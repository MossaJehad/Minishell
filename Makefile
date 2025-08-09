NAME	=	minishell
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -g -Iinclude
RM		=	rm -rf
HEADERS	=	include/minishell.h

# Directories
SRC_DIR	=	./src
OBJ_DIR	=	obj

# Libft config
LIBFT_DIR	=	libft
LIBFT		=	$(LIBFT_DIR)/libft.a
LIBFT_INC	=	$(LIBFT_DIR)

# Readline
READLINE_INC	=	-I/usr/include/readline
READLINE_LIB	=	-lreadline

# Sources and Objects
SRC		=	main.c \
			parsing.c \
			tokenize.c \
			commands.c \
			inputs.c \
			expander.c \
			utils.c \
			signals.c \
			exit_utils.c \
			utils_utils.c \
			signals_utils.c \
			main_utils_plus.c

OBJS	=	$(SRC:%.c=$(OBJ_DIR)/%.o)

# Flags
CFLAGS		+=	-Iinclude -I$(LIBFT_INC) $(READLINE_INC)
LDFLAGS		=	$(LIBFT) $(READLINE_LIB)

# Colors
GREEN   = \033[1;32m
CYAN    = \033[1;36m
YELLOW  = \033[1;33m
RED     = \033[1;31m
RESET   = \033[0m

# Error handler
define check_error
	@if [ $$? -ne 0 ]; then \
		echo "$(RED)[MINISHELL] ❌ Build failed!$(RESET)"; \
		exit 1; \
	fi
endef

# Targets
all:
	@echo "$(GREEN)[MINISHELL] 🚀 Building project...$(RESET)"
	@$(MAKE) --no-print-directory $(NAME) || { echo "$(RED)[MINISHELL] ❌ Build failed$(RESET)"; exit 1; }

$(NAME):	$(OBJS) $(LIBFT)
	@echo "$(CYAN)[MINISHELL] 🔗 Linking executable...$(RESET)"
	@$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)
	$(call check_error)
	@echo "$(GREEN)[MINISHELL] ✅ Build complete!$(RESET)"

# Compile source to obj/ directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call check_error)

# Build libft
$(LIBFT):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

clean:
	@echo "$(YELLOW)[MINISHELL] 🧹 Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)[MINISHELL] 🗑️  Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:
	@echo "$(CYAN)[MINISHELL] ♻️  Rebuilding everything...$(RESET)"
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

.PHONY:	all clean fclean re
