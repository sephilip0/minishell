#include "./minishell.h"

char	*isname(char *str, t_lexer *lexer)
{
	char	*name;
	char	*return_state;

	name = str_untier(str, lexer);
	/*printf("+++++++ LEXER START: %d\n", lexer->start);
	lexer->start = i;
	printf("------- LEXER START: %d\n", lexer->start);
	printf("========= LEXER END: %d\n", lexer->end);
	lexer->end = ft_strlen(str);
	printf("========= LEXER POSEND: %d\n", lexer->end);*/
	lexer->pipe_flag = 0;
	printf("SAIDA IS NAME: %s\n", name);
	return_state = cmd_path(name, set_pather(lexer->env));
	printf("SAIDA IS NAME RETURN: %s\n", return_state);
	return(return_state);
}

//added dollar signs
//added ? 
//is letter number underscore dot slash or quote
int	ft_islnudsq(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c == '_') || (c == '.') || (c == '$' || (c == '?'))
		|| (c == '/') ||(c == 39) || (c == 34));
}

//USED IN NEWEXPANDER
int	ft_islnudsqi(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c == '_') || (c == '.') || (c == '?'))
		|| (c == '/') ||(c == 39) || (c == 34);
}



int	set_redir(char *str, t_lexer *lexer)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	lexer->in_redir = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if ((str[i] == '<' || str[i] == '>') && !in_single_quotes && !in_double_quotes)
		{
			lexer->start = i;
	//		printf("start: %d\n", lexer->start);
			if (str[i] == '<')
				lexer->in_redir = 1;
			else if (str[i] == '>')
				lexer->in_redir = 2;
			i++;
			if ((str[i] == '<' || str[i] == '>') && !in_single_quotes && !in_double_quotes)
			{
				if (str[i] == '<' && lexer->in_redir == 1)
					lexer->in_redir = 3;
				else if (str[i] == '>' && lexer->in_redir == 2)
					lexer->in_redir = 4;
				else if (str[i] == '>' || str[i] == '<')
					lexer->in_redir = 0;
				i++;
	//			printf("start: %d\n", lexer->start);
			}
			break;
		}
		i++;
	}
	while (str[i] && str[i] == ' ')
		i++;
	//printf("start: %d\n", lexer->start);
	return (i);
}

// int	set_redir(char *str, t_lexer *lexer)
// {
// 	int	i;

// 	i = 0;
// 	lexer->in_redir = 0;
// 	while (str[i] && str[i] != '<' && str[i] != '>')
// 		i++;
// 	if (str[i] == '<' || str[i] == '>')
// 	{
// 		lexer->start = i;
// 		if (str[i] == '<')
// 			lexer->in_redir = 1;
// 		else if (str[i] == '>')
// 			lexer->in_redir = 2;
// 		i++;
// 	}
// 	if (str[i] == '<' || str[i] == '>')
// 	{
// 		if (str[i] == '<' && lexer->in_redir == 1)
// 			lexer->in_redir = 3;
// 		else if (str[i] == '>' && lexer->in_redir == 2)
// 			lexer->in_redir = 4;
// 		else if (str[i] == '>' || str[i] == '<')
// 			lexer->in_redir = 0;
// 		i++;
// 	}
// 	while (str[i] && str[i] == ' ')
// 		i++;
// 	return (i);
// }

//> "fi le" WILL CREATE 'fi le'
//CONSIDERING A NAME THAT ISALNUDSQ, OTHERWISE CHANGE FUNCTION
//THIS WILL SET IN_REDIT THAT NEED TO BE CLEAN AFTERWARDS
char	*isredir(char *str, t_lexer *lexer)
{
	int	i;
	int	tmp_start;
	char	*file;

	printf("---------------ISREDIR: %s\n", str);
	i = set_redir(str, lexer);
	printf("PRE I: %d\n", i);	
	printf("set_redir: %d\n", lexer->in_redir);
	tmp_start = lexer->start;
	if (lexer->in_redir == 0 || (!str[i]) || ft_islnudsq(str[i]) == 0)
		return (NULL);
	file = str_untier(&str[i], lexer);
	printf("UNITERIERJRIJI---------LEXER START: %d\n", lexer->start);	
	printf("UNTIERELRKJLKJ---------LEXER END: %d\n", lexer->end);	
	//printf("FILE: %s\n", file);
	//CAREFULL WITH # THAT MAKE ISLNUDSQ ERROR
	while(str[i] && ft_islnudsq(str[i]))
	{
		if (str[i] == 34 || str[i] == 39)
			lexer->willexpand = 0;
		i++;
	}
	lexer->end = i - 1;
	lexer->start = tmp_start;
	printf("---------LEXER START: %d\n", lexer->start);	
	printf("---------LEXER END: %d\n", lexer->end);	
// 	NOT SURE ABOUT CHECKING FOR FULL PATH: which file1
	printf("ISREDIR RETURN: %s\n", file);
	return (file);
}
