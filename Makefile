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
SRC		=	tokenize.c main.c utils.c inputs.c commands.c expander.c parsing.c
OBJS	=	$(SRC:%.c=$(OBJ_DIR)/%.o)

# Flags
CFLAGS		+=	-Iinclude -I$(LIBFT_INC) $(READLINE_INC)
LDFLAGS		=	$(LIBFT) $(READLINE_LIB)

# Targets
all:
	@echo "\033[1;32m[MINISHELL] Building project...\033[0m"
	@$(MAKE) $(NAME)

$(NAME):	$(OBJS) $(LIBFT)
	@echo "\033[1;36m[MINISHELL] Linking executable...\033[0m"
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)

# Compile source to obj/ directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "\033[1;33m[MINISHELL] Cleaning object files...\033[0m"
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\033[1;31m[MINISHELL] Removing executable...\033[0m"
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re:
	@echo "\033[1;35m[MINISHELL] Rebuilding everything...\033[0m"
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY:	all clean fclean re
