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
# include <sys/stat.h>
# include <stdbool.h>

# define NL 0
# define JOB 1
# define CMD 2
# define REDIR 3
# define NAME 4
# define TOKEN 5
# define PATH_MAX 1024
# define BUFFERSIZE 42

extern int status;

typedef struct s_tree
{
	int				id;
	int				fid;
	int				herexpand;
	//malloc it in the creation;
	int				pipefd[2];
	char			*content;
	int				pipe_flag;
	int				redir_type;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_lexer
{
	int		report;
	int		start;
	int		end;
	int		in_quote;
	int		in_redir;
	int		willexpand;
	int		pipe_flag;
	int		sflag;
	int		status;
	char	**env;
	t_tree	*tree;
	//1 = < in
	//2 = > out
	//3 = << heredoc
	//4 = >> std_output
}	t_lexer;

typedef enum e_signals
{
	MAIN,
	CHILD,
	HEREDOC,
	IGNORE
}	t_signals;

//E_REFORKED.C
char	*newexpander(char *new, t_lexer *lexer, int dolexpand);


void	print_error(char *str);
char	*set_command(char *str);
char	**join_tokens(t_tree *tree);
int		pipecount(t_tree *tree);
int		openinfile(t_tree *tree, t_lexer *lexer);
int		openoutfile(t_tree *tree);
int		openfiles(t_tree *tree, t_tree *cmd, t_lexer *lexer);
int		closeclean(int fd, int force);
int		ft_heredoc(char *eof, t_lexer *lexer, int dolexpand);
int		error_return(char *str, int exit_status);
int		setfds(t_tree *tree, int npipes, t_lexer *lexer);
void	closeall_fd(t_tree *tree);
void	waiter(t_tree *cmd);
int		executive(t_tree *tree, t_tree *reset, t_lexer *lexer);
void	print_mat(char **mat);
int		find_nid(t_tree *tree, int id);

char	*ft_getenv(char **envir, char *str);

//P_BIGIS.C
int		isjob(char *str);
char	*iscmd(char *str, t_lexer *lexer);
int		quote_skipper(char *str);
int		str_skipper(char *str);
char	*str_untier(char *str, t_lexer *lexer);
char	*quote_untier(char *str, t_lexer *lexer);

//P_DOLLAR.C
char	**matcpy(char **mat);
int		ft_islnu(int c);
int		addtostr(char **ret, char **from, t_lexer *lexer);
char	*dollar_sign(char *str, t_lexer *lexer);
char	*weak_quote(char *str, t_lexer *lexer);
char	*stro_quote(char *str);

//P_ENVIR.C
char	*ret_free(char **str, char **mat);
char	**set_pather(char **mat);
char	*check_env(char **cmd, char **pather);
char	*cmd_path(char *cmd, char **pather);

//P_NODES.C
char	*expanddollar(char *new, t_lexer *lexer, int dolexpand);
int		add_left(t_tree *tree, t_lexer *lexer, char *str, int id);
void	add_right(t_tree *tree, t_lexer *lexer, char *str, int id);
void	tree_free(t_tree *tree);
int		tree_init(t_tree **tree, t_lexer *lexer, char *str);
void	parser(t_tree *node, t_lexer *lexer, int cmd_found);

//P_REDIR.C
char	*isname(char *str, t_lexer *lexer);
int		ft_islnudsq(int c);
int		ft_islnudsqi(int c);
int		set_redir(char *str, t_lexer *lexer);
char	*isredir(char *str, t_lexer *lexer);

//P_REMAIN.C
void	free_mat(char **mat);
void	strnotstr(char *dst, char *src, int start, int end);
int		emptystring(char *str);
char	*remain(char *str, t_lexer *lexer);

//MAIN.C
void	tree_reader(t_tree *tree, int n);
void	directexit(char *str, int status);

//BUILTINS
void	pipeignore(int fd);
void	builtins(t_tree *reset, char **mat, t_lexer *lexer, int pipenbr);
int		b_echo(char **mat);
int		b_pwd();
// CD
//int		is_cd(char *line, char **param);
//void	built_cd(char *param);
//int		ft_isspace(int c);
int	b_cd(char **mat, int tofree);
// EXPORT
int		exports_loop(char	**mat, t_lexer *lexer);
int		export(char *key_value, char ***envp);
int		append_env(char *key_value, char ***envp);
void	unset_env(char *key, char ***envp);
// EXPORT_UTILS
int		free_and_error(char ***envp, char *error);
int		is_in_env(char *key, char **envp);
int		is_valid_key_value(char *arg);
// UNSET
int		unset_loop(char **mat, t_lexer *lexer);
int		unset(char *key_value, char ***envp);

//READ_LOOP
char	*get_cur_directory(t_lexer *lexer, int dollar);

// SIGACTION
void	setup_signal_handlers();
void	get_status(int	sts);

#endif
