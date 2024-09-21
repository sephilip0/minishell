# include "./minishell.h"

//just so you can return in one line
char	*ret_free(char **str, char **mat)
{
	free(*str);
	if (mat)
		free_mat(mat);
	return(NULL);
}

//we now have every environment variable in a matrix
char	**set_pather(char **mat)
{
	int	i;
	char	**envir;

	i = 0;
	envir = NULL;
	while (mat[i])
	{
		if (ft_strncmp(mat[i], "PATH=", 5) == 0)
		{
			envir = ft_split(&(mat[i][6]), ':');
			break ;
		}
		i++;
	}
	return (envir);
}

//check if cmd is already accessible
char	*check_env(char **cmd, char **pather)
{
	struct stat st;

	if (*cmd == NULL)
		return (NULL);
	if (!ft_strncmp(*cmd, "cd", 3))
		return (*cmd);
	if (!ft_strncmp(*cmd, "echo", 5))
		return (*cmd);
	if (!ft_strncmp(*cmd, "export", 7))
		return (*cmd);
	if (!ft_strncmp(*cmd, "unset", 6))
		return (*cmd);
	if (!ft_strncmp(*cmd, "env", 4))
		return (*cmd);
	if (!ft_strncmp(*cmd, "exit", 5))
		return (*cmd);
	if (access(*cmd, F_OK | R_OK | X_OK) != -1)
	{
		if (stat(*cmd, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				return (NULL);
		}
		return (*cmd);
	}
	if (!pather)
		free(*cmd);
	return (NULL);
}

//if cmdpath is impossible to find, it returns NULL
char	*cmd_path(char *cmd, char **pather)
{
	int		i;
	char	*path;

	i = 0;
	//printf("CMD_PATH: %s\n", cmd);
	path = check_env(&cmd, pather);
	if (!(*cmd) || !pather || path != NULL)
	{
		free_mat(pather);
		return(path);
	}
	while (pather[i])
	{
		path = (ft_neostrjoin(pather[i], "/", 0));
		if (!path)
			return (ret_free(&cmd, pather));
		path = (ft_neostrjoin(path, cmd, 1));
		if (!path)
			return (ret_free(&cmd, pather));
		if (access(path, F_OK | R_OK | X_OK) != -1)
		{
			free(cmd);
			free_mat(pather);
			return (path);
		}
		else
			free(path);
		i++;
	}
	return (ret_free(&cmd, pather));
}

