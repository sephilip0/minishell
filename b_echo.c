# include "./minishell.h"

// -n and -nnnnnnnnn work the same
int	b_echo(char **mat, int fd)
{
	int	i;
	int	j;
	int	nl;
	
	i = 1;
	j = 1;
	nl = 1;
	if (mat[1] && mat[1][0] == '-')
	{
		if (mat[1][1] == 'n')
		{
			nl = 0;
			while(mat[1][j])
			{
				if (mat[1][j] != 'n')
					nl = 1;
				j++;
			}
		}
	}
	if (nl == 0)
		i++;
	while (mat[i])
		ft_putstr_fd(mat[i++], fd);
	if (nl)
		ft_putchar_fd(10, fd);
	return (0);
}
