#include "minishell.h"

//divide or join parts
//expand(substitute)
//put in the tree



char *find_redir()

char *find_name()





//input: < file1 cat -e | grep "il" | echo "hello" "hi" > file2
//cleaning + expanding: <file1 cat -e | grep il | echo hello hi >file2
//
//tree with checking for token?
//
//token analising: <REDIR> <NAME> <TOKEN> <PIPE> <NAME> <TOKEN> <PIPE> <NAME> <TOKEN> <TOKEN> <REDIR>




char *checkpath()
{
	//i already got a function for that
	in_path = false;
}

int	isspecial(int symbol, t_lexer lexer)
{
	if(symbol == 20)
		return (symbol);
	else if(symbol == 34)
		return (symbol);
		//in_weak = true;
	else if(symbol == 36)
		return (symbol);
		//in_path = true;
	else if(job[i] == 39)
		return (symbol);
	else if(job[i] == 60)
		return (symbol);
	else if(job[i] == 62)
		return (symbol);
	else if(job[i] == 124)
		return (symbol);
	else
		return (0);
}

int	is_redir(char *cmd)
{
	//redir type
	//1 = input 2 = heredoc 3 = output 4 = append
	int	redir;
	int	i;

	i = 0;
	redir = 0;
	while (cmd[i] && cmd[i] != ' ')
	{
		if (cmd[i] == "<" && redir == 0)
			redir = 1;
		if (cmd[i] == "<" && redir == 1)
			redir = 2;
		if (cmd[i] == ">" && redir == 0)
			redir = 3;
		if (cmd[i] == ">" && redir == 3)
			redir = 4;
		while (cmd[i] && cmd[i] == ' ')
			i++;
		while() //need to handle 'e''c''h''o'
		{
			if (cmd[i] == 34)
				weak_quote();
			if (cmd[i] == 39)
				stro_quote();
		//	need to check for space
		}
	}
}


char *quote_untier(char *str)
{
	int	i;
	char	*untied;

	i = 0;
	untied = NULL;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] == 34)
		{
			if (*untied)
				untied = ft_neostrjoin(united, weak_quote(str), ?);
			else
				untied = weak_quote(str);
			while (str[i] && str[i] != 34)
				i++;
 		}
		if (str[i] == 39)
		{
			if (*untied)
				untied = ft_neostrjoin(united, stro_quote(str), ?);
			else
				unntied = stro_quote(str);
			while (str[i] && str[i] != 39)
				i++;
		}
	}
	return (untied);
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

char *ft_realloc(char *str, int len, int new_size)
{
	char *ret;

	ret = NULL;

//	printf("CALLED REALLOC: %p, len %d, new_size: %d\n", str, len, new_size);
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
	return (path);
}

void	lexer(char *input)
{
	int	i;
	int	j;
	char	*job;
	int	nargs;

	//job = ft_strdup(input);

	//20 = space
	//34 = weak ""
	//36 = $
	//39 = strong ''
	//60 = <
	//62 = >
	//124 = |
	j = 0
	while(job[j] && isspecial(job[j]) == 0)

//read read read until special symbol.
//everything in the back goes to the new malloc.
//reads the specials (what is need to be done or substituted is) and everything that comes with it. Then neostrjoin everything together
//repeat


//found a special, every special has its own rules so we clean them and open malloc for it.
	

	while(job[j] != 39)
	{
		in_strong = true;
	}
	//$ signs
	//spaces



}



int	main(int argc, char *argv[], char *envp[])
{
	char	*gnl;

	//input = readline("[minishell]");
	//	free(input);
	//
	//temporarily with GNL then readline
	gnl = get_next_line(0);
	if (gnl == NULL)
		return (1);
//	while ((ft_strlen(gnl) != eof_len) || 
//		(ft_strncmp(gnl, eof, eof_len - 1) != 0))
//	{
//		ft_putstr_fd(gnl, fdout);
//		free(gnl);
//		gnl = get_next_line(fdin);
//		if (gnl == NULL)
//			return (1);
//	}
	printf("[MINISHELL]%s", gnl);
	free(gnl);
//wait untill every fork finishes?
//then loop again?	
	return(0);
}
