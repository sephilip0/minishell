#include "./minishell.h"

// check if its really a job or a infiltrated cmd
// int	isjob(char *str)
// {
// 	int	jobbable;
// 	int	i;

// 	i = 0;
// 	jobbable = 0;
// 	while (str[i] && str[i] != '|')
// 		i++;
// 	if (str[i] && str[i] == '|')
// 	{
// 		i++;
// 		while (str[i])
// 		{
// 			if (str[i] != ' ')
// 				jobbable = 1;
// 			i++;
// 		}
// 	}
// 	return (jobbable);
// }
// IS_JOB VERSÃO DINIS
int	isjob(char *str)
{
	int	jobbable;
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	jobbable = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote) //Toggle single quote
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '|' && !in_single_quote && !in_double_quote)
		{
			i++;
			while (str[i])
			{
				if (str[i] != ' ')
					jobbable = 1;
				i++;
			}
			break ;
		}
		i++;
	}
	return (jobbable);
}

//IS_CMD VERSÃO DINIS
char	*iscmd(char *str, t_lexer *lexer)
{
	int		i;
	char	*ret;
	int		in_single_quote;
	int		in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	lexer->pipe_flag = 0;
	while(str[i])  // <-- apaguei aqui while (str[i] != ' ')
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '|' && !in_single_quote && !in_double_quote)
			break;
		i++;
	}
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

// //CONSIDER: GNL GOT THE EXTRA NL SYMBOL THAT RL DOESNT. CAREFULL WITH LEXER->END
// char	*iscmd(char *str, t_lexer *lexer)
// {
// 	int		i;
// 	char	*ret;

// 	i = 0;
// 	lexer->pipe_flag = 0;
// 	while(str[i] && str[i] != '|')
// 		i++;
// 	if (str[i] == '|')
// 		lexer->pipe_flag = 1;
// 	lexer->start = i + 1;
// 	lexer->end = ft_strlen(str);
// 	if (ft_strlen(str) == (size_t)i)
// 		return (NULL);
// 	while(--i >= 0 && str[i] == ' ');
// 	if (i == -1)
// 		return (NULL);
// 	ret = ft_calloc(i + 2, sizeof(char));
// 	if (ret == NULL)
// 		return (NULL);
// 	while (i >= 0)
// 	{
// 		ret[i] = str[i];
// 		i--;
// 	}
// 	return (ret);
// }

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
/*
int	just_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
	{
		printf("just char empty str\n");
		return (1);
	}
	while (str[i])
	{
		printf("just char: %d\n", str[i]);
		if (str[i] != c)
			return (0);
		i++;
	}
	return (1);
}*/

char	*str_untier(char *str, t_lexer *lexer)
{
	int	i;
	char	*untied;
	char	*dollar;

	i = 0;
	untied = "";
	printf("STRUNTIER: %s\n", str);
	while(str[i] && str[i] == ' ')
		i++;
	printf("INREDIR: %d\n", lexer->in_redir);
	while(str[i] && str[i] != ' ')
	{
		if (str[i] == 34 || str[i] == 39) // " or '
		{
			printf("GO GO QUOTE\n");
			if (*untied)
				untied = ft_neostrjoin(untied, quote_untier(&str[i], lexer), 3);
			else
				untied = quote_untier(&str[i], lexer);
			i += quote_skipper(&str[i]);
		}
		else if (str[i] == '$' && lexer->in_redir != 3)
		{
			dollar = dollar_sign(&str[i], lexer);
			printf("DOLLAR: %s\n", dollar);
			printf("#######################UNTIED PRE: %s\n", untied);
			if (addtostr(&untied, &dollar, lexer) == 1)
				return (NULL);
			printf("#######################UNTIED POS: %s\n", untied);
			while(str[++i] && (ft_islnu(str[i])));
			//printf("dollar: %s\n", dollar);
			if (lexer->sflag)
			{
				i++;
				lexer->sflag = 0;
			}
			if (!str[i] && dollar == NULL)
				return (NULL);
		}
		else // Adiciona o normal caracter ao untied
		{
			printf("STRUNTIER REALLOC\n");
			untied = ft_realloc(untied, ft_strlen(untied),
		       		ft_strlen(untied) + 1);
			untied[ft_strlen(untied)] = str[i++];
			printf("UNTIEEEEDDDD: %s\n", untied);
		}
	}
	lexer->start = i;
	lexer->end = ft_strlen(str);
	printf("from str: %s\n", untied);
//	printf("UNTIED: %s\n", untied);
	return (untied);
}

//HANDLES QUOTES OR QUOTES + QUOTES WITHOUT SPACE: "aaa"'bbb'
char *quote_untier(char *str, t_lexer *lexer) // ""ola
{
	int	i;
	char	*untied;

	i = 0;
	untied = "";
	printf("QUOTEUNTIER: %s\n", str);
	while (str[i] && (str[i] == 34 || str[i] == 39))
	{
		while (str[i] && str[i + 1] && (str[i + 1] == str[i])) // o próximo é " '
		{
			printf("CHTO SUKA?\n");
			/*if (!str[i + 2])
				return (ft_strdup("\0"));*/
			i += 2;
		}
		printf("str: %s\n", str);
		printf("str: %s | %d\n", &str[i], i);
		if (str[i] && str[i] == 34) //""
		{
			printf("WEAKKKYY\n: %s\n", untied);
			if (*untied)
				untied = ft_neostrjoin(untied, weak_quote(&str[i], lexer), 3);
			else
				untied = weak_quote(&str[i], lexer);
			while (str[++i] && str[i] != 34);
			i++;
		}
		else if (str[i] && str[i] == 39) // ''
		{
			if (*untied)
				untied = ft_neostrjoin(untied, stro_quote(&str[i]), 3);
			else
				untied = stro_quote(&str[i]);
			while (str[++i] && str[i] != 39);
			i++;
		}
		//else
		//	return (NULL);
	}
	//printf("QUOTEUNTIER1: %s\n", untied);
	return (untied);
}

