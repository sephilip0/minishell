#include "./minishell.h"


//NOT USED IN THIS FILE, SHOULD BE IN LIBFT/
void	free_mat(char **mat)
{
	int	i;

	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
}

//COPY EVERYTHING MINUS SOMETHING SPECIFIC
void	strnotstr(char *dst, char *src, int start, int end)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (i >= start && i <= end)
			i++;
		else
		{
			dst[j] = src[i];
			i++;
			j++;
		}
	}
}

//STRING JUST MADE OF SPACES
int	emptystring(char *str)
{
	int	i;
	int	isempty;

	isempty = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ') isempty = 0;
		i++;
	}
	return (isempty);
}

//CALCULATE THE REMAIN TO CONTINUE IN THE RIGHT NODE
char	*remain(char *str, t_lexer *lexer)
{
	char	*ret;
	int		j;

	ret = NULL;
	if (lexer->start >= lexer->end)
		return (NULL);
	if (str == NULL)
		return (NULL);
	if (lexer->in_redir == 0)
	{
		ret = ft_calloc(ft_strlen(str) - lexer->start + 2, sizeof(char));
		if (ret == NULL)
			return (NULL);
		ft_strlcpy(ret, &str[lexer->start], lexer->end);
	}
	else //lexer->in_redir != 0
	{
		ret = ft_calloc(ft_strlen(str) - (lexer->end - lexer->start + 1) + 1, sizeof(char));
		if (ret == NULL)
			return (NULL);
		strnotstr(ret, str, lexer->start, lexer->end);
	}
	if (emptystring(ret))
		return(ret_free(&ret));
	j = 0;
	lexer->pipe_flag = 0;
	while (ret[j])
	{
		if (ret[j] == '|')
			lexer->pipe_flag = 1;
		j++;
	}
	lexer->in_redir = 0;
	return (ret);
}

