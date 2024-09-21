# include "./minishell.h"

//is letter number or underscore
int	ft_islnu(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c == '_'));
}

//RETURN 1 IN CASE OF BAD MALLOC
int	addtostr(char **ret, char **from, t_lexer *lexer)
{
	printf("CHHHHOOOOOOO?: \n");
	if (*from == NULL)
		return (0);
	*ret = ft_realloc(*ret, ft_strlen(*ret),
		ft_strlen(*from) + ft_strlen(*ret));
	if (*ret == NULL)
	{
		if (ft_strlen(*from) == 1 && **from == '$')
			free(*from);
		return (1);
	}
	ft_strlcat(*ret, *from, ft_strlen(*from) + ft_strlen(*ret) + 1);
	printf("NEW DOOLLARRRR: %s\n", *ret);
	if (lexer->sflag || (ft_strlen(*from) == 1 && **from == '$'))
		free(*from);
	return (0);
}

//testar para env -i
char	**matcpy(char **mat)
{
	char	**full;
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!mat)
		return (NULL);
	while (mat[i])
		i++;
	full = ft_calloc((i + 1), sizeof(char *));
	if (!full) directexit("Cannot allocate memory\n", 4);
	while (j < i)
	{
		full[j] = ft_substr(mat[j], 0, ft_strlen(mat[j]));
		if (!full[j])
		{
			free_mat(full);
			directexit("Cannot allocate memory\n", 4);
		}
		j++;
	}
	full[j] = 0;
	return (full);
}

char	*ft_getenv(char **envir, char *str)
{
	int	i;
	int	j;
	char	*ret;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
 	while (envir[i])
	{
		j = 0;
		while (envir[i][j] && str[j] && envir[i][j] != '=' &&
			envir[i][j] == str[j])
			j++;
		if (!str[j] && envir[i][j] == '=')
		{
			ret = ft_substr(envir[i], j + 1, ft_strlen(envir[i]) - j - 1);
			return (ret);
		}
		i++;
	}
	return (NULL);
}

// $PATH -> environment
// $(ls) -> command substitution NOT YET HANDLED
// CHECK FOR FREE (DONT MIND THE PATH, IT IS FREED HERE)
char	*dollar_sign(char *str, t_lexer *lexer)
{
	int	i;
	char	*ret;
	char	*path;

	i = 1;
	path = NULL;
	ret = NULL;
	if(str[i] && str[i] == '?')
	{
		lexer->sflag = 1;
		return(ft_itoa(status));
	}
	while(str[i] && ft_islnu(str[i]))
		i++;
	ret = (char *)ft_calloc(i + 1, sizeof(char));
	if (i == 1)
	{
		printf("-------------\n");
		ft_strlcpy(ret, &str[0], 2);
		printf("retldkfjaldkfjaslkdfja: %s\n", ret);
		return (ret);
	}
	ft_strlcpy(ret, &str[1], i);
	printf("PRE RET: %s\n", ret);
	path = ft_getenv(lexer->env, ret);
//	path = getenv(ret);
	free(ret);
	//printf("RETURN DO DOLLAR_SIGN: %s\n", path);
	printf("PATH: %s\n", path);
	return (path);
}

//CHECK FOR ERROR MALLOC RETURN
//DONT HANDLE $$
char *weak_quote(char *str, t_lexer *lexer)
{
	int	i;
	char	*ret;
	char	*dollar;

	i = 1;
	dollar = NULL;
	ret = "";
	if (str[i] && str[i] == 34)
		return (NULL);
	while(str[i] && str[i] != 34)
	{
		if (str[i] == 36 && lexer->in_redir != 3)
		{
			dollar = dollar_sign(&str[i], lexer);
			if (addtostr(&ret, &dollar, lexer) == 1)
				return (NULL);
			while(str[++i] && (ft_islnu(str[i])));
			if (lexer->sflag)
			{
				i++;
				lexer->sflag = 0;
			}
		}
		else
		{
			ret = ft_realloc(ret, ft_strlen(ret),
				ft_strlen(ret) + 1);
			ret[ft_strlen(ret)] = str[i++];
		}
	}
	return (ret);
}

char *stro_quote(char *str)
{
	int		i;
	int		j;
	char	*ret;

	i = 1;
	j = 0;
	if (str[i] && str[i] == 39)
		return (NULL);
	while(str[i] && str[i] != 39)
		i++;
	ret = (char *)ft_calloc(i, sizeof(char));
	if (ret == NULL)
		return (NULL);
	while(j < i - 1)
	{
		ret[j] = str[j + 1];
		j++;
	}
	return(ret);
}

