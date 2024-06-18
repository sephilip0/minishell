#include "./minishell.h"

// check if its really a job or a infiltrated cmd
int	isjob(char *str)
{
	int	jobbable;
	int	i;

	i = 0;
	jobbable = 0;
	while (str[i] && str[i] != '|')
		i++;
	if (str[i] && str[i] == '|')
	{
		i++;
		while (str[i])
		{
			if (str[i] != ' ')
				jobbable = 1;
			i++;
		}
	}
	return (jobbable);
}

//CONSIDER: GNL GOT THE EXTRA NL SYMBOL THAT RL DOESNT. CAREFULL WITH LEXER->END
char	*iscmd(char *str, t_lexer *lexer)
{
	int		i;
	char	*ret;

	i = 0;
	lexer->pipe_flag = 0;
	while(str[i] && str[i] != '|')
		i++;
	if (str[i] == '|')
		lexer->pipe_flag = 1;
	lexer->start = i + 1;
	lexer->end = ft_strlen(str);
	if (ft_strlen(str) == (size_t)i)
		return (NULL);
	while(--i >= 0 && str[i] == ' ');
	if (i == -1)
		return (NULL);
	ret = ft_calloc(i + 2, sizeof(char));
	if (ret == NULL)
		return (NULL);
	while (i >= 0)
	{
		ret[i] = str[i];
		i--;
	}
	return (ret);
}

// "abc"'ghi'
//skips quotes that are inside or tied together. return int to skip later
int	quote_skipper(char *str)
{
	int	i;
	int	topquote;

	i = 0;
	while (str[i])
	{
		topquote = str[i];
		i++;
		while(str[i] && (str[i] != topquote))
			i++;
		if (!str[i])
			return(i);
		if (!str[i + 1] || (str[i + 1] != 39 && str[i + 1] != 34))
		{
			i++;
			break;
		}
		i++;
	}
	return (i);
}

// "abc"d'efg'
int	str_skipper(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 39 || str[i == 34])
			i += quote_skipper(&str[i]);
		if (str[i] && str[i] != ' ')
			i++;
		else
			break;
 	}
	return (i);
}

char	*str_untier(char *str, t_lexer *lexer)
{
	int	i;
	char	*untied;
	char	*dollar;

	i = 0;
	untied = "";
	while(str[i] && str[i] == ' ')
		i++;
	while(str[i] && str[i] != ' ')
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (*untied)
				untied = ft_neostrjoin(untied, quote_untier(&str[i], lexer), 3);
			else
				untied = quote_untier(&str[i], lexer);
			i += quote_skipper(&str[i]);
		}
		else if (str[i] == '$')
		{
			dollar = dollar_sign(&str[i], lexer);
			if (addtostr(&untied, &dollar, lexer) == 1)
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
			untied = ft_realloc(untied, ft_strlen(untied),
				ft_strlen(untied) + 1);
			untied[ft_strlen(untied)] = str[i++];
		}
	}
	lexer->start = i;
	lexer->end = ft_strlen(str);
	return (untied);
}

//HANDLES QUOTES OR QUOTES + QUOTES WITHOUT SPACE: "aaa"'bbb'
char *quote_untier(char *str, t_lexer *lexer)
{
	int	i;
	char	*untied;

	i = 0;
	untied = "";
	while (str[i] && (str[i] == 34 || str[i] == 39))
	{
		if (str[i + 1] && (str[i + 1] == 34 || str[i + 1] == 39))
			i += 2;
		if (str[i] == 34) //""
		{
			if (*untied)
				untied = ft_neostrjoin(untied, weak_quote(&str[i], lexer), 3);
			else
				untied = weak_quote(&str[i], lexer);
			while (str[++i] && str[i] != 34);
			i++;
		}
		else if (str[i] == 39) // ''
		{
			if (*untied)
				untied = ft_neostrjoin(untied, stro_quote(&str[i]), 3);
			else
				untied = stro_quote(&str[i]);
			while (str[++i] && str[i] != 39);
			i++;
		}
	}
	return (untied);
}

