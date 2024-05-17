# include "./libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>


char	*dollar_sign(char *str);
int	ft_islnu(int c);

char *ft_realloc(char *str, int len, int new_size)
{
	char *ret;

	ret = NULL;

	printf("CALLED REALLOC: %p, len %d, new_size: %d\n", str, len, new_size);
	if (!str)
	{
		printf("se fudeu\n");
		ret = (char *)ft_calloc(new_size, sizeof(char));
		return (ret);
	}
	ret = (char *)ft_calloc(new_size + 1, sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, str, len + 1);
	if (*str)
		free(str);
//	printf("VAI RETORNAR: %s\n", ret);
	return (ret);
}




//DIDNT HANDLE FOR $$
char *weak_quote(char *str)
{
	int	i;
	char	*ret;
	char	*dollar;

	i = 1;
	dollar = NULL;
	ret = "";
	printf("WEAK_QUOTE START: %s\n", str);
	if (str[i] && str[i] == 34)
		return (NULL);
	while(str[i] && str[i] != 34)
	{
		if (str[i] == 36)
			dollar = dollar_sign(&str[i]);
		if (dollar != NULL)
		{
				ret = ft_realloc(ret, ft_strlen(ret),
		    			ft_strlen(dollar) + ft_strlen(ret));
				ft_strlcat(ret, dollar, ft_strlen(dollar) + ft_strlen(ret) + 1);
				i++;
				while(str[i] && ft_islnu(str[i]))
					i++;
				dollar = NULL;
		}
		else
		{
			ret = ft_realloc(ret, ft_strlen(ret),
				ft_strlen(ret) + 1);
			ret[ft_strlen(ret)] = str[i];
			i++;
		}
	}
	printf("WEAK_QUOTE RETURN: %s\n", ret);
	return (ret);
}

//is letter number or underscore
int	ft_islnu(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c == '_'));
}

// $PATH -> environment
// $(ls) -> command substitution NOT YET HANDLED
char	*dollar_sign(char *str)
{
	int	i;
	char	*ret;
	char	*path;

	//we skip the dollar sign
	i = 1;
	//path is temporary, we instantly free it 
	path = NULL;
	ret = NULL;
	//printf("DOLLAR SIGN: %s\n", &str[1]);
	while(str[i] && ft_islnu(str[i]))
		i++;
	ret = (char *)ft_calloc(i, sizeof(char));
	ft_strlcpy(ret, &str[1], i);
	path = getenv(ret);
	free(ret);
	printf("path: %s\n", path);
	return (path);
}

char *stro_quote(char *str)
{
	int	i;
	int	j;
	int	ret_size;
	char	*ret;

	i = 1;
	j = 0;
	while(str[i] && str[i] != 39)
		i++;
	//nao precisa do + 1 pois ja comecamos no + 1
	ret = (char *)ft_calloc(i, sizeof(char));
	if (ret == NULL)
		return (NULL);
	while(j < i)
	{
		ret[j] = str[j + 1];
		j++;
	}
	return(ret);
}


char *quote_untier(char *str)
{
	int	i;
	char	*untied;

	i = 0;
	untied = "";
	while (str[i] && str[i] != ' ')
	{
		printf("quote_untier: STD[%d] = %c\n", i, str[i]);
		if (str[i] == 34)
		{
			printf("weak\n");
			if (*untied)
			{
				printf("there is something\n");
				untied = ft_neostrjoin(untied, weak_quote(&str[i]), 1);
			}
			else
				untied = weak_quote(&str[i]);
			i++;
			while (str[i] && str[i] != 34)
				i++;
			printf("weak result: %s\n", untied);
 		}
		else if(str[i] == 39)
		{
			printf("strong\n");
			if (*untied)
				untied = ft_neostrjoin(untied, stro_quote(&str[i]), 1);
			else
				untied = stro_quote(&str[i]);
			i++;
			while (str[i] && str[i] != 39)
				i++;
			printf("stro result: %s\n", untied);
		}
		i++;
		sleep(1);
	}
	return (untied);
}

int	main(int argc, char *argv[])
{
	char *path;
	char *gnl;
	gnl = get_next_line(0);
	//path = quote_untier(gnl);
	//path = dollar_sign(gnl);
	path = weak_quote(gnl);
	printf("size: %d path: %s\n", ft_strlen(path), path);
//	free(path);
	free(gnl);
	return (0);
}
