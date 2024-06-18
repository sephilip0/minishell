#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
//COMPILE THIS SHIT WITH -lreadline

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdbool.h>


# define NL 0
# define JOB 1
# define CMD 2
# define REDIR 3
# define NAME 4
# define TOKEN 5

typedef struct s_tree
{
	int				id;
	char			*content;
	int				pipe_flag;
	int				redir_type;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_lexer
{
	int		start;
	int		end;
	int		in_quote;
	int		in_redir;
	int		pipe_flag;
	int		sflag;
	int		status;
	char	**env;
	//1 = < in
	//2 = > out
	//3 = << heredoc
	//4 = >> std_output
}	t_lexer;

//P_BIGIS.C
int		isjob(char *str);
char	*iscmd(char *str, t_lexer *lexer);
int	quote_skipper(char *str);
int	str_skipper(char *str);
char	*str_untier(char *str, t_lexer *lexer);
char	*quote_untier(char *str, t_lexer *lexer);

//P_DOLLAR.C
int	ft_islnu(int c);
int	addtostr(char **ret, char **from, t_lexer *lexer);
char	*dollar_sign(char *str, t_lexer *lexer);
char	*weak_quote(char *str, t_lexer *lexer);
char	*stro_quote(char *str);

//P_ENVIR.C
char	*ret_free(char **str);
void	set_environment(t_lexer *lexer, char *envp[]);
char	*check_env(char **cmd, t_lexer *lexer);
char	*cmd_path(char *cmd, t_lexer *lexer);

//P_NODES.C
int	add_left(t_tree *tree, t_lexer *lexer, char *str, int id);
int	add_right(t_tree *tree, t_lexer *lexer, char *str, int id);
void	tree_free(t_tree *tree);
int	tree_init(t_tree **tree, char *str);
int	parser(t_tree *node, t_lexer *lexer);

//P_REDIR.C
char	*isname(char *str, t_lexer *lexer);
int	ft_islnudsq(int c);
int	set_redir(char *str, t_lexer *lexer);
char	*isredir(char *str, t_lexer *lexer);

//P_REMAIN.C
void	free_mat(char **mat);
void	strnotstr(char *dst, char *src, int start, int end);
int		emptystring(char *str);
char	*remain(char *str, t_lexer *lexer);

//E_MACHINE.C
void	the_machine(t_tree *tree, t_lexer *lexer);
void	run_tree(t_tree *tree, t_lexer *lexer);
void	set_redirection(t_tree *tree, t_lexer *lexer);
int		is_executable(t_tree *tree);
int		fork1(void);
void	print_error(char *str);

//E_EXEC_CMD.C
void	exec_cmd(t_tree *tree, t_lexer *lexer);

//E_REDIRECTION.C
void	read_here_doc(int write_pipe, char *eof);
void	open_here_doc(char	*eof, t_lexer *lexer);
void	set_redirection(t_tree *tree, t_lexer *lexer);

//E_PIPE.C
void	set_pipe(t_tree *tree, t_lexer *lexer);

//MAIN.C
void	tree_reader(t_tree *tree, int n);

//BUILTINS
int	b_echo(char **mat, int fd);
int	builtins(char **mat, int fd);
int	b_pwd(int fd);
int	b_cd(char **mat);


#endif
