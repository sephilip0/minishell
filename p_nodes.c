#include "./minishell.h"

//0 on SUCCESS
int	add_left(t_tree *tree, t_lexer *lexer, char *str, int id)
{
	t_tree	*new;

	if (str == NULL)
		return (1);
	new = (malloc(sizeof(t_tree)));
	if (new == NULL)
		return (1);
	if (lexer->in_redir)
		new->redir_type = lexer->in_redir;
	else
		new->redir_type = 0;
	new->pipe_flag = 0;
	new->id = id;
	new->content = str;
	new->right = NULL;
	new->left = NULL;
	tree->left = new;
	return (0);
}

int	add_right(t_tree *tree, t_lexer *lexer, char *str, int id)
{
	t_tree	*new;

	lexer->start = 0;
	lexer->end = 0;
	if (str == NULL)
		return (1);
	new = (malloc(sizeof(t_tree)));
	if (new == NULL)
		return (1);
	new->pipe_flag = 0;
	if (lexer->pipe_flag == 1)
		new->pipe_flag = 1;
	new->redir_type = 0;
	new->id = id;
	new->content = str;
	new->left = NULL;
	new->right = NULL;
	tree->right = new;
	return (0);
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

//STARTING POINT OF THE TREE
int	tree_init(t_tree **tree, char *str)
{
	int	i;

	i = 0;
	if (emptystring(str))
		return (0);
	*tree = malloc(sizeof(t_tree));
	if (tree == NULL)
		return (0);
	(*tree)->content = ft_strdup(str);
	(*tree)->id = JOB;
	(*tree)->redir_type = 0;
	(*tree)->left = NULL;
	(*tree)->right = NULL;
	(*tree)->pipe_flag = 0;
	while (str[i])
	{
		if (str[i] == '|')
			(*tree)->pipe_flag = 1;
		i++;
	}
	return (1);
}

//RETURN VALUE IS NOT USED YET
int	parser(t_tree *node, t_lexer *lexer)
{
	if (node->id == JOB && !(isjob(node->content)))
		node->id = CMD;
	if (add_left(node, lexer, iscmd(node->content, lexer), CMD) == 0)
		add_right(node, lexer, remain(node->content, lexer), JOB);
	else if (add_left(node, lexer, isredir(node->content, lexer), REDIR) == 0)
		add_right(node, lexer, remain(node->content, lexer), CMD);
	else if (add_left(node, lexer, isname(node->content, lexer), NAME) == 0)
		add_right(node, lexer, remain(node->content, lexer), CMD);
	else if (add_left(node, lexer, str_untier(node->content, lexer), TOKEN) == 0)
		(add_right(node, lexer, remain(node->content, lexer), CMD));
	else
		return (0);
	if (node->left && (node->left->id == CMD))
		parser(node->left, lexer);
	if (node->right && (node->right->id == CMD || node->right->id == JOB))
		parser(node->right, lexer);
	return (0);
}
