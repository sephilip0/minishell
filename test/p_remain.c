#include "./minishell.h"


//NOT USED IN THIS FILE, SHOULD BE IN LIBFT/
void	free_mat(char **mat)
{
	int	i;

	i = 0;
	while (mat && mat[i])
	{
		free(mat[i]);
		i++;
	}
	if (mat)
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
		if (str[i] != ' ')
		isempty = 0;
		i++;
	}
	return (isempty);
}

//CALCULATE THE REMAIN TO CONTINUE IN THE RIGHT NODE
char	*remain(char *str, t_lexer *lexer)
{
	char	*ret;
	int		j;
	int		in_single_quote;
	int		in_double_quote;

	ret = NULL;
	printf("REMAIN: %s\n", str);
	printf("START: %d\n", lexer->start);
	printf("END: %d\n", lexer->end);
//	printf("!!!!!FORCED LEXER END: %ld\n", ft_strlen(str));
//	lexer->end = ft_strlen(str);
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
	//	printf("LEN: %ld END: %d START: %d\n", ft_strlen(str), lexer->end, lexer->start);
	//	printf("CALLOC PARA %ld\n", ft_strlen(str) - (lexer->end - lexer->start + 1) + 1);
		ret = ft_calloc(ft_strlen(str) - (lexer->end - lexer->start + 1) + 1, sizeof(char));
		if (ret == NULL)
			return (NULL);
		printf("RETPOSCALLOR: %s\n", ret);
		strnotstr(ret, str, lexer->start, lexer->end);
		printf("RETSTRNOTSTR: %s\n", ret);
	}
	if (emptystring(ret))
		return(ret_free(&ret, NULL));
	j = 0;
	//printf("RET: %s\n", ret);
	lexer->pipe_flag = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (ret[j])
	{
		if (ret[j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (ret[j] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (ret[j] == '|' && !in_single_quote && !in_double_quote)
			lexer->pipe_flag = 1;
		j++;
	}
	lexer->in_redir = 0;
	return (ret);
}

