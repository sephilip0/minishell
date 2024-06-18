# include "./minishell.h"

//just so you can return in one line
char	*ret_free(char **str)
{
	free(*str);
	return(NULL);
}

//we now have every environment variable in a matrix
void	set_environment(t_lexer *lexer, char *envp[])
{
	int	i;

	i = 0;
	lexer->env = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			lexer->env = ft_split(&(envp[i][6]), ':');
			break ;
		}
		i++;
	}
}

//check if cmd is already accessible
char	*check_env(char **cmd, t_lexer *lexer)
{
	printf("CHECK_ENV: %s\n", *cmd);
	if (*cmd == NULL)
		return (NULL);
	if (!ft_strncmp(*cmd, "cd", 3))
	{
		printf("MF DID IT\n");
		return (*cmd);
	}
	printf("strange\n");
	if (access(*cmd, F_OK | R_OK | X_OK) != -1)
		return (*cmd);
	printf("strange\n");
	if (!(lexer->env))
		free(*cmd);
	return (NULL);
}

//if cmdpath is impossible to find, it returns NULL
char	*cmd_path(char *cmd, t_lexer *lexer)
{
	int		i;
	char	*path;

	i = 0;
	printf("CMD_PATH: %s\n", cmd);
	path = check_env(&cmd, lexer);
	if (!(*cmd) || !(lexer->env) || path != NULL)
		return(path);
	printf("what the fuck\n");
	while (lexer->env[i])
	{
		path = (ft_neostrjoin(lexer->env[i], "/", 0));
		if (!path)
			return (ret_free(&cmd));
		path = (ft_neostrjoin(path, cmd, 1));
		if (!path)
			return (ret_free(&cmd));
		if (access(path, F_OK | R_OK | X_OK) != -1)
		{
			free(cmd);
			return (path);
		}
		else
			free(path);
		i++;
	}
	return (ret_free(&cmd));
}

