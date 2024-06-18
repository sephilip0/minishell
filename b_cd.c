# include "./minishell.h"

// -1 error
// 0 ok
// 1 not builtin
int	builtins(char **mat, int fd)
{
	int	value;

	value = 1;
	printf("BUILTINS: %s\n", *mat);
	if (!ft_strncmp(*mat, "cd", 3))
		value = b_cd(mat);
	else if (!ft_strncmp(*mat, "pwd", 4))
		value = b_pwd(fd);
	else if (!ft_strncmp(*mat, "echo", 5))
		value = b_echo(mat, fd);
	return (value);
}

int	b_pwd(int fd)
{
	char	*ptr;
	int	size;

	ptr = NULL;
	size = 1024;
	ptr = (char *)ft_calloc(size, sizeof(char));
	if (ptr == NULL)
		return (-1);
	ptr = getcwd(ptr, 1024);
	ft_putstr_fd(ptr, fd);
	free(ptr);
	return (0);
}

int	b_cd(char **mat)
{
	if(mat[1] && mat[2])
	{
		printf("B_CD: TOO MANY ARGUMENTS\n");
		return (-1);
	}
	if (mat[1] == NULL || !strncmp(mat[1], "~", 2) || !strncmp(mat[1], "~/", 3))
		chdir(getenv("HOME"));
	else if (chdir(mat[1]) == -1)
	{
		printf("B_CD: NO SUCH FILE OR DIRECTORY\n");
		return (-1);
	}
	return (0);
}
