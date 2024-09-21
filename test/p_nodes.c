#include "./minishell.h"

//0 on SUCCESS
int	add_left(t_tree *tree, t_lexer *lexer, char *str, int id)
{
	t_tree	*new;

	if (str == NULL)
		printf("ADD_LEFT EMPTY\n");
	else
		printf("ADD_LEFT: %s %d %ld\n", str, str[0], ft_strlen(str));
	if (str == NULL)// || ft_strlen(str) == 0)
		return (1);
	new = (malloc(sizeof(t_tree)));
	if (new == NULL)
	{
		lexer->report = 4;
		return (4);
	}
	if (lexer->in_redir)
		new->redir_type = lexer->in_redir;
	else
		new->redir_type = 0;
	new->pipe_flag = 0;
	new->pipefd[0] = 1;
	new->pipefd[1] = 0;
	new->herexpand = lexer->willexpand;
	//reset
	lexer->willexpand = 1;
	new->fid =  0;
	new->id = id;
	new->content = str;
	new->right = NULL;
	new->left = NULL;
	tree->left = new;
	return (0);
}

void	add_right(t_tree *tree, t_lexer *lexer, char *str, int id)
{
	t_tree	*new;

	printf("ADD_RIGHT: %s\n", str);
	lexer->start = 0;
	lexer->end = 0;
	if (str == NULL)
		return ;
	new = (malloc(sizeof(t_tree)));
	if (new == NULL)
	{
		lexer->report = 4;
		return ;
	}
	new->pipe_flag = 0;
	new->pipefd[0] = 1;
	new->pipefd[1] = 0;
	new->fid =  0;
	if (lexer->pipe_flag == 1)
		new->pipe_flag = 1;
	new->redir_type = 0;
	new->herexpand = 0;
	new->id = id;
	new->content = str;
	new->left = NULL;
	new->right = NULL;
	tree->right = new;
	return ;
}

//FREE THE ENTIRE TREE
void	tree_free(t_tree *tree)
{
	if (tree == NULL)
		return ;
	if (tree->left)
		tree_free(tree->left);
	if (tree->right)
		tree_free(tree->right);
	if (tree->content && *tree->content)
		free(tree->content);
	if (tree)
		free(tree);
}

int	pre_heredoc(char *str, int i)
{
	//ignore the dollar
	if (str[i + 1] && (str[i + 1] == 34 || str[i + 1] == 39))
	{
		str[i] = ' ';
		return (1);
	}
	i = i - 1;
	while(i >= 0)
	{
		printf("I + 1 = %d %d", i + 1, str[i + 1]);
		if (str[i] == '<')
		{
			printf("FIRST FOUND\n");
			if((i - 1 >= 0) && str[i - 1] == '<')
				 return (1);
		}
		if (str[i] == ' ')
		{
			printf("i: %d space\n", i);
			i--;
		}
		else
		{
			printf("something else: %d: %c\n", i, str[i]);
			break;
		}
	}
	return (0);
}


/*
//qualquer coisa qualquer coisa $hey hi
//qualquer coisa qualquer coisa ls -a hi
char	*strinside(char	*str, char *in, int i)
{
	char	*ret;
	int	dol_len;

	//i - 1 == $
	dol_len = i;
	while (str[dol_len] && ft_islnu(str[dol_len]))
	{
		new[dol_len] = ' ';	
		i++;
	}

	ret = ft_strlcpy(s)
}*/
//0, 34 = 34
//34, 34 = 0
//34, 39 = 34
//39, 34 = 39
int	intalternate(int nbr, int compare)
{
	if (nbr == compare)
		return (0);
	else if ((nbr != compare) && (nbr != 0) && (compare != 0))
		return (nbr);
	else
		return (compare);
}


char	*newexpander(char *new, t_lexer *lexer, int dolexpand)
{
	char	*str;
	char	*dollar;
	int		i;

	i = 0;
	printf("NEW EXPANDER: %s\n", new);
	str = "";
	while (new[i])
	{
		printf("new[i]: %d\n", new[i]);
		if (new[i] == '$' && dolexpand)
		{
			dollar = dollar_sign(&new[i], lexer);
			printf("VOLTA DO DOLLAR: %s", dollar);	
			if (!dollar)
			{
				i++;
				while (new[i] && ft_islnudsqi(new[i]))
					i++;
			}
			else
			{
				str = ft_neostrjoin(str, dollar, 3);
				i++;
				if (new[i] && new[i] == '?')
					i++;
				else
				{
					while (new[i] && ft_islnu(new[i]))
						i++;
				}
			}
		}
		else
		{
			str = ft_realloc(str, ft_strlen(str), ft_strlen(str) + 1);
			str[ft_strlen(str)] = new[i];
			i++;
		}
	}
	printf("EXIT EXPANDER: %s\n", str);
	free (new);
	return (str);
}

char	*expanddollar(char *new, t_lexer *lexer, int dolexpand)
{
	char	*str;
	char	*ret;
	int		i;
	int		inquote;

	i = 0;
	inquote = 0;
	ret = "";
	while(new[i])
	{
		if (new[i] == 34 || new[i] == 39)
			inquote = intalternate(inquote, new[i]);
		if (dolexpand)
			inquote = 0;
		if (new[i] == '$' && (!inquote && (pre_heredoc(new, i) == 0)))
		{
			str = dollar_sign(&new[i], lexer);
			fprintf(stderr, "str: %s\n", str);
			ret = ft_neostrjoin(ret, str, 3);
			i++;
			if (new[i] && new[i] == '?')
				i++;
			else
			{
				while (new[i] && ft_islnu(new[i]))
					i++;
			}
		}
		else
		{
			ret = ft_realloc(ret, ft_strlen(ret), ft_strlen(ret) + 1);
			ret[ft_strlen(ret)] = new[i];
			i++;
		}
	}
	free(new);
	return (ret);
}

int	checksyntax(char *str)
{
	int		i;
	int		quote;

	//CHECK QUOTES
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			quote = intalternate(quote, str[i]);
		i++;
	}
	if (quote != 0)
	{
		printf("WRONG NUMBER OF QUOTES\n");
		
		return (1);
	}
	return (0);
}

char	*emptyclean(char *str, t_lexer *lexer)
{
	char	*new;
	char	*dollar;
	int	quote;
	int	i;

	i = 0;
	new = ft_strdup(str);
	if (!new)
		return (NULL);
	if (checksyntax(new))
	{
		free(new);
		return (NULL);
	}
	while (new[i])
	{
		if (new[i] == 34 || new[i] == 39)
		{
			quote = new[i++];
			while(new[i] && new[i] != quote)
				i++;
		}
		if (new[i] == '$' && (pre_heredoc(new, i) == 0))
		{
			dollar = dollar_sign(&new[i], lexer);
			if (!dollar)
			{
				new[i++] = ' ';
				while(new[i] && ft_islnu(new[i]))
				{
					new[i] = ' ';	
					i++;
				}
			}
			else
			{
				if (new[i + 1] &&
					(new[i + 1] == 34 || new[i + 1] == 39))
					new[i] = ' ';
				free(dollar);
			}
		}
		i++;
	}
	printf("OLD BLYA: %s\n", new);
	new = expanddollar(new, lexer, 0);
	printf("NEW BLYA: %s\n", new);
	return (new);
}


int	charnstr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != c)
			return (1);
		i++;
	}
	return (0);
}

//STARTING POINT OF THE TREE
int	tree_init(t_tree **tree, t_lexer *lexer, char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	*tree = malloc(sizeof(t_tree));
	//MAYBE REDO MORE BEAUTIFULLY
	if (!tree)
	{
		free(str);
		free_mat(lexer->env);
		directexit("Cannot allocate memory\n", 4);
	}
	if (tree)
		(*tree)->content = emptyclean(str, lexer);
	if (!(*tree)->content)
	{
		free(*tree);
		return (0);
	}
	//TEMPORARY SOLUTION!
	/*if (!(*tree)->content)
	{
		free(str);
		free(*tree);
		free_mat(lexer->env);
		directexit("Cannot allocate memory\n", 4);
	}*/
	if (!charnstr((*tree)->content, 32))
		return (0);
	(*tree)->id = JOB;
	(*tree)->redir_type = 0;
	(*tree)->left = NULL;
	(*tree)->right = NULL;
	(*tree)->pipe_flag = 0;
	(*tree)->herexpand = 1;
	(*tree)->pipefd[0] = 1;
	(*tree)->pipefd[1] = 0;
	(*tree)->fid =  0;
	lexer->tree = *tree;
	//MAYBE EM OUTRA FUNCAO
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (str[i] == '|' && !in_single_quote && !in_double_quote)
			(*tree)->pipe_flag = 1;
		i++;
	}
	return (1);
}

t_tree *renode(t_tree *node, t_lexer *lexer)
{
	char	*new;

	new = remain(node->content, lexer);
	if (node->content)
		free(node->content);
	if (!new)
	{
		printf("EMPTY REMAIN\n");
		return (NULL);
	}
	else 
		printf("RENODE REMAIN: %s\n", new);
	node->content = new;
	return (node);
}


//RETURN VALUE IS NOT USED YET
void	parser(t_tree *node, t_lexer *lexer, int cmd_found)
{
	printf("[PARSER]: %s\n", node->content);
	if (lexer->report)
		return ;
	if (node->id == JOB && !(isjob(node->content)))
		node->id = CMD;
	if (add_left(node, lexer, iscmd(node->content, lexer), CMD) == 0)
	{
		printf("ADD_RIGHT JOB\n");
		add_right(node, lexer, remain(node->content, lexer), JOB);
	}
	else if (add_left(node, lexer, isredir(node->content, lexer), REDIR) == 0)
	{
		printf("ADD_RIGHT CMD\n");
		add_right(node, lexer, remain(node->content, lexer), CMD);
	}
	else if (cmd_found == 0 && add_left(node, lexer, isname(node->content, lexer), NAME) == 0)
	{
		printf("ADD_RIGHT CMD2\n");
		add_right(node, lexer, remain(node->content, lexer), CMD);
		cmd_found = 1;
	}
	else if (add_left(node, lexer, str_untier(node->content, lexer), TOKEN) == 0)
	{
		printf("ADD_RIGHT CMD3\n");
		//printf("START: %d END: %d\n", lexer->start, lexer->end);
		add_right(node, lexer, remain(node->content, lexer), CMD);
	}
	else
		return ;
		//parser(renode(node, lexer), lexer, cmd_found);
//		add_right(remain(node->content, lexer)
		//return ;
	if (node->left && (node->left->id == CMD))
		parser(node->left, lexer, cmd_found);
	if (node->right && (node->right->id == CMD || node->right->id == JOB))
		parser(node->right, lexer, cmd_found);
	return ;
}
