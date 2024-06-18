# include "./minishell.h"

void	tree_reader(t_tree *tree, int n)
{
	printf("[%i]------------------------------------\n", n);
	printf("ID: %d ", tree->id);
	if (tree->id == NL)
		printf("[NULL]\n");
	else if (tree->id == JOB)
		printf("[JOB]\n");
	else if (tree->id == CMD)
		printf("[CMD]\n");
	else if (tree->id == REDIR)
		printf("[REDIR]\n");
	else if (tree->id == NAME)
		printf("[NAME]\n");
	else if (tree->id == TOKEN)
		printf("[TOKEN]\n");
	printf("tree->content:%s\n", tree->content);
	printf("pipe_flag: %i\n", tree->pipe_flag);
	printf("redir type: %d\n", tree->redir_type);
	if(tree->left)
	{
		printf("\033[0;33mVAI PARA ESQ\033[0m\n");
		tree_reader(tree->left, n + 1);
	}
	if(tree->right)
	{
		printf("\033[0;31mVAI PARA DIR\033[0m\n");
		tree_reader(tree->right, n + 1);
	}
}

//TEST BUILTINS
// int	main(int argc, char *argv[])
// {
// 	int	i;
// 	char **mat;

// 	argc++;
// 	mat = ft_split(argv[1], '#');
// 	i = builtins(mat, 0);
// 	printf("MAIN i: %d\n", i);
// 	b_pwd(0);
// 	free_mat(mat);
// 	return (0);
// }

//what about ls ..

int	main(int argc, char *argv[], char *envp[])
{
//	char *dol;
	t_lexer	lexer;
	t_tree	*tree;
	char	*line;

	argc++;
	argv[0] = NULL;
	lexer.start = 0;
	lexer.end = 0;
	lexer.in_redir = 0;
	lexer.status = 0;
	lexer.sflag = 0;
	set_environment(&lexer, envp);
// VERSÃO COM READLINE
	while (1)
	{
		printf("PWD: %i\n", b_pwd(0));
		printf("Lexer Status: %i\n", lexer.status);
		line = readline("sh$ ");
		if (line == NULL)
			break ;
	if  ((ft_strncmp(line, "exit", ft_strlen(line)) == 0) || (ft_strncmp(line, "exit ", 6) == 0))
		{
			free(line);
			break;
		}
		add_history(line);
		if (tree_init(&tree, line))
		{
			parser(tree, &lexer);
			tree_reader(tree, 0);
			printf("<------------------|--|------------------>\n");
			the_machine(tree, &lexer);
			tree_free(tree);
		}
	}
	free_mat(lexer.env);
//-----------------------------------------------------
// VERSÃO SEM READLINE
/* 	if (tree_init(&tree, "cd libft"))  // echo $?hi | cat
 	{
 	 	parser(tree, &lexer);
 	 	tree_reader(tree, 0);
 	 	printf("<------------------|--|------------------>\n");
 	 	the_machine(tree, &lexer);
 		tree_free(tree);
 	
 	free_mat(lexer.env);*/
 	return (0);
}
