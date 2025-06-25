NAME        =   minishell
CC          =   cc
CFLAGS      =   -Wall -Wextra -Werror -g
RM          =   rm -rf

# Directories
SRC_DIR     =   .
OBJ_DIR     =   obj

# Libft config
LIBFT_DIR   =   libft
LIBFT       =   $(LIBFT_DIR)/libft.a
LIBFT_INC   =   $(LIBFT_DIR)

# Readline
READLINE_INC =   -I/usr/include/readline
READLINE_LIB =   -lreadline

# Sources and Objects
SRC         =   tokenize.c main.c utils.c inputs.c commands.c expander.c
OBJS        =   $(SRC:%.c=$(OBJ_DIR)/%.o)

# Flags
CFLAGS      +=  -I$(LIBFT_INC) $(READLINE_INC)
LDFLAGS     =   $(LIBFT) $(READLINE_LIB)

# Targets
all:    $(NAME)

$(NAME):    $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)

# Compile source to obj/ directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
