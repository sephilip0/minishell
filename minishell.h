#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
//COMPILE THIS SHIT WITH -lreadline

# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <stdbool.h>


typedef struct s_tree
{
	char	*id;
	char	*content;
	struct s_tree	*left;	
	struct s_tree	*right;	
}	t_tree;

typedef struct s_ms
{
	char	**env;
	t_tree	*tree;

}	t_ms;



typedef struct s_lexer
{
	//to expand
	bool	in_strong;
	bool	in_weak;
	//use as index;
	int	in_path;

	//just to check if every pipe got two sides?
	bool	in_pipe;
	// < > >>
	int	in_redir;
	// 1 2 3 (4 <<)
}	t_lexer;

//FUNCTIONS

#endif
