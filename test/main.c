# include "./minishell.h"

int status = 0;

void	tree_reader(t_tree *tree, int n)
{
	printf("[%i]------------------------------------\n", n);
	printf("ID: %d ", tree->id); if (tree->id == NL)
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
	printf("pipe_fd[0]: %i pipe_fd[1]: %i\n", tree->pipefd[0], tree->pipefd[1]);
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

char	*get_cur_directory(t_lexer *lexer, int dollar)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	if (cwd == NULL)
	{
		if (lexer->env)
			free_mat(lexer->env);
		directexit("Cannot allocate memory\n", 4);
	}
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		free(cwd);
		if (lexer->env)
			free_mat(lexer->env);
		directexit("getcwd error()\n", 1);
	}
	if (dollar)
		ft_strlcat(cwd, "$ ", PATH_MAX);
	return (cwd);
}

void	checkreport(t_tree *tree, t_lexer *lexer, char *line, int force)
{
	if (force == -1 && lexer->report == 0)
		return ;
	tree_free(tree);
	free_mat(lexer->env);
	free(line);
	if (force == 0)
		directexit("", 0);
	if (force == 4 || lexer->report == 4)
		directexit("Cannot allocate memory\n", 4);
}


void	tree_sorter(t_tree *tree, t_lexer *lexer, char *line)
{
	char **jt;

	checkreport(tree, lexer, line, -1);
	//FROM THIS POINT WE START USING LEXER STATUS INSTEAD OF REPORT
	tree_reader(tree, 0);
	if (find_nid(tree, JOB) == 0 &&
		(!ft_strncmp(tree->left->content, "cd", 3)))
		status = b_cd(join_tokens(tree), 1);
	else if (find_nid(tree, JOB) == 0 &&
		(!ft_strncmp(tree->left->content, "export", 7)))
	{
		jt = join_tokens(tree);
		if (!jt)
			checkreport(tree, lexer, line, 4);
		status = exports_loop(jt, lexer);
		free_mat(jt);
	}
	else if (find_nid(tree, JOB) == 0 &&
		(!ft_strncmp(tree->left->content, "unset", 6)))
	{
		jt = join_tokens(tree);
		if (!jt)
			checkreport(tree, lexer, line, 4);
		status = unset_loop(jt, lexer);
		free_mat(jt);
	}
	else
	{
		if (executive(tree, tree, lexer) == -1)
			status = 1;
	}
	tree_free(tree);
}

//USIGN LEXER->REPORT FOR THE PROGRAMM ITSELF(SHELL)
//USING LEXER->STATUS FOR THE STATUS INSIDE SHEEL (USER INPUT)
void	read_loop(t_lexer *lexer)
{
	char	*cur_dir;
	char	*line;
	t_tree	*tree;

	while (!lexer->report)
	{
		setup_signal_handlers(MAIN);
		cur_dir = get_cur_directory(lexer, 1);
		line = readline(cur_dir);
		free(cur_dir);
		if (line == NULL)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		else if (*line)
		{
			if  ((ft_strncmp(line, "exit", ft_strlen(line)) == 0) || (ft_strncmp(line, "exit ", 6) == 0))
			{
				free(line);
				break;
			}
			add_history(line);
			if (tree_init(&tree, lexer, line))
			{
				parser(tree, lexer, 0);
				tree_sorter(tree, lexer, line);
			}
			free(line);
		}
	}
	rl_clear_history();
	free_mat(lexer->env);
	exit(lexer->report);
}

void	directexit(char *str, int status)
{
	if (*str)
		ft_putstr_fd(str, 2);
	exit(status);
}

void	increment_shlvl(char **envp)
{
	char	**key_value;
	char	*str_sh;
	int		sh_value;
	int		i;


	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "SHLVL", 5))
		{
			key_value = ft_split(envp[i], '=');
			free(envp[i]);
			sh_value = ft_atoi(key_value[1]) + 1;
			if (key_value)
				free_mat(key_value);
			str_sh = ft_itoa(sh_value);
			envp[i] = ft_strjoin("SHLVL=", str_sh);
			free(str_sh);
		}
		i++;
	}
}

char	**ft_enviroment(char **envp)
{
	char	**artificial_envp;

	if (envp[0])
		return (matcpy(envp));
	artificial_envp = (char **) malloc(sizeof(char *) * 3);
	if (!artificial_envp)
		error_return(": error malloc allocation", 1);
	artificial_envp[0] = ft_strjoin("PATH=", get_cur_directory(NULL, 0));
	artificial_envp[1] = ft_strdup("SHLVL=1");\
	artificial_envp[2] = 0;
	return (artificial_envp);
}

int	main(int argc, char *argv[], char *envp[])
{
//	char *dol;
	t_lexer	lexer;

	argc++;
	argv[0] = NULL;
	lexer.report = 0;
	lexer.start = 0;
	lexer.end = 0;
	lexer.in_redir = 0;
	lexer.status = 0;
	lexer.willexpand = 1;
	lexer.sflag = 0;
	lexer.env = ft_enviroment(envp);
	if (envp[0])
		increment_shlvl(lexer.env);
	read_loop(&lexer);
	free_mat(lexer.env);
}
