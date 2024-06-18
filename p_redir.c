#include "./minishell.h"

char	*isname(char *str, t_lexer *lexer)
{
	int	i;
	char	*name;

	i = 0;
	name = "";
	while(str[i] && str[i] == ' ')
		i++;
	while(str[i] && str[i] != ' ')
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (*name)
				name = ft_neostrjoin(name, str_untier(&str[i], lexer), 3);
			else
				name = str_untier(&str[i], lexer);
			i += str_skipper(&str[i]);
		}
		else
		{
			name = ft_realloc(name, ft_strlen(name),
				ft_strlen(name) + 1);
			name[ft_strlen(name)] = str[i++];
		}
	}
	lexer->start = i;
	lexer->end = ft_strlen(str);
	lexer->pipe_flag = 0;
	return(cmd_path(name, lexer));
}

//is letter number underscore dot slash or quote
int	ft_islnudsq(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c == '_') || (c == '.')
		|| (c == '/') ||(c == 39) || (c == 34));
}


int	set_redir(char *str, t_lexer *lexer)
{
	int	i;

	i = 0;
	lexer->in_redir = 0;
	while (str[i] && str[i] != '<' && str[i] != '>')
		i++;
	if (str[i] == '<' || str[i] == '>')
	{
		lexer->start = i;
		if (str[i] == '<')
			lexer->in_redir = 1;
		else if (str[i] == '>')
			lexer->in_redir = 2;
		i++;
	}
	if (str[i] == '<' || str[i] == '>')
	{
		if (str[i] == '<' && lexer->in_redir == 1)
			lexer->in_redir = 3;
		else if (str[i] == '>' && lexer->in_redir == 2)
			lexer->in_redir = 4;
		else if (str[i] == '>' || str[i] == '<')
			lexer->in_redir = 0;
		i++;
	}
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

//> "fi le" WILL CREATE 'fi le'
//CONSIDERING A NAME THAT ISALNUDSQ, OTHERWISE CHANGE FUNCTION
//THIS WILL SET IN_REDIT THAT NEED TO BE CLEAN AFTERWARDS
char	*isredir(char *str, t_lexer *lexer)
{
	int	i;
	char	*file;

	file = "";
	i = set_redir(str, lexer);
	if (lexer->in_redir == 0 || (!str[i]) || ft_islnudsq(str[i]) == 0)
		return (NULL);
	while(str[i] && ft_islnudsq(str[i]))
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (*file)
				file = ft_neostrjoin(file, str_untier(&str[i], lexer), 3);
			else
				file = str_untier(&str[i], lexer);
			i += str_skipper(&str[i]);
		}
		else
		{
			file = ft_realloc(file, ft_strlen(file),
				ft_strlen(file) + 1);
			file[ft_strlen(file)] = str[i++];
		}
		lexer->end = i - 1;
	}
// 	NOT SURE ABOUT CHECKING FOR FULL PATH: which file1
	return (file);
}
