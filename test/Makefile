NAME = minishell

SRC = p_remain.c p_redir.c p_nodes.c p_envir.c p_dollar.c p_bigis.c \
	e_redirection.c e_pipe.c e_machine.c e_exec_cmd.c main.c \
	b_echo.c b_cd.c b_built_ins.c b_export.c b_unset.c b_export_utils.c \
	e_reforked.c s_sigaction.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra -g #-lreadline -g #fsanitize=address

LIBS = -lreadline  -L/usr/local/lib

INCLUDES = -Ilibft/

LIBFT = libft/libft.a

GREEN = \033[0;32m
CYAN = \033[0;36m
RED = \033[0;31m
NC = \033[0;33m

all: $(LIBFT) $(OBJ) $(NAME)

$(LIBFT):
	@echo "$(CYAN) \n-Compiling Libft...\n$(NC)"
	@$(MAKE) -C libft

	@echo "$(CYAN) \n-Compiling $(NAME)...\n$(NC)"
	@echo "cc $(CFLAGS) -g $(INCLUDES) -c *.c -o *.o"

$(OBJ): %.o: %.c
	@echo "$(CYAN) Compiling $@...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	@echo "$(CYAN)\n-Linking $(NAME) with libft...\n$(NC)"
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME) $(LIBS)
	@make clean
	@echo "$(GREEN)\n--------------------------------$(NC)"
	@echo "$(GREEN) $(NAME) is ready!$(NC)"
	@echo "$(GREEN)--------------------------------$(NC)"

clean:
	@echo "$(CYAN)\n-Cleaning Libft...\n$(NC)"
	$(MAKE) -C libft clean
	@echo "$(CYAN)\n-Cleaning object files...\n$(NC)"
	@rm -f $(OBJ)
	@echo "$(GREEN)\n--------------------------------$(NC)"
	@echo "$(GREEN) $(NAME) object files cleaned!$(NC)"
	@echo "$(GREEN)--------------------------------$(NC)"

fclean: clean
	@echo "$(CYAN)\n-Cleaning $(NAME) and libft...\n$(NC)"
	rm -f $(NAME)
	$(MAKE) -C libft fclean
	@echo "$(GREEN)\n--------------------------------$(NC)"
	@echo "$(GREEN) $(NAME) object files cleaned!$(NC)"
	@echo "$(GREEN)--------------------------------$(NC)"

re: fclean all

r: all
	./minishell
s: all
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
v: all
	valgrind -s --leak-check=full --show-leak-kinds=all ./minishell

# run: all
# 	./pipex infile cat "grep a" outfile

.PHONY: all clean fclean re r
