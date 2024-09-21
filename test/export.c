#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../libft/libft.h"

static char     *ftsubstr(char const *s, int start, int len)
{
	int		i;
	char	*substr;

	i = 0;
	substr = malloc((len + 1) * sizeof(char));
	if (substr == NULL)
			return (NULL);
	while (i < len && s[start])
			substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}

static void     ft_free_mem(char **ptr, int len)
{
	int     i;

	i = 0;
	while (i < len)
	{
			if (ptr[i])
					free(ptr[i]);
			i++;
	}
	free(ptr);
}

static void     ft_concat(char **ptr, int words, char const *s, char c)
{
	int     i;
	int     j;
	int     k;

	i = 0;
	k = 0;
	while (k < words)
	{
			while (s[i] == c && s[i])
					i++;
			j = i;
			while (s[j] != c && s[j])
					j++;
			ptr[k] = ftsubstr(s, i, j - i);
			if (ptr[k] == NULL)
			{
					ft_free_mem(ptr, words);
					return ;
			}
			ptr[k][j - i] = '\0';
			k++;
			i = j;
	}
	ptr[k] = NULL;
	return ;
}

static int      count_words(char const *s, char c)
{
	int     count;
	int     i;

	count = 0;
	i = 0;
	while (s[i])
	{
			if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
					count++;
			i++;
	}
	return (count);
}

size_t	ft_strlen(const char *s)
{
	unsigned int	size;

	size = 0;
	//printf("s: %p\n", s);
	if (!(*s))
	{
//		printf("ft_strlen retornara 0\n");
		return (0);
	}
	while (s[size])
		size++;
	return (size);
}

char    **ft_split(char const *s, char c)
{
	char    **ptr;
	int             words;

	words = count_words(s, c);
	ptr = malloc((words + 1) * sizeof(char *));
	if (ptr == NULL)
			return (NULL);
	ft_concat(ptr, words, s, c);
	return (ptr);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const char	*p2;
	char		*p;
	size_t		i;

	i = 0;
	if (!dest && !src)
		return (dest);
	p = (char *)dest;
	p2 = (const char *)src;
	while (i < n)
	{
		p[i] = p2[i];
		i++;
	}
	return (dest);
}

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Função para contar o número de elementos em um array de strings
int count_envp(char **envp) {
    int count = 0;
    while (envp[count]) {
        count++;
    }
    return count;
}

// Função para alocar dinamicamente uma cópia do array envp
char **duplicate_envp(char **envp) {
    int envp_count = count_envp(envp);
    char **new_envp = (char **)malloc((envp_count + 1) * sizeof(char *));
    if (!new_envp) {
        perror("malloc");
        return NULL;
    }

    for (int i = 0; i < envp_count; i++) {
        new_envp[i] = strdup(envp[i]);
        if (!new_envp[i]) {
            perror("strdup");
            // Libera a memória já alocada em caso de falha
            for (int j = 0; j < i; j++) {
                free(new_envp[j]);
            }
            free(new_envp);
            return NULL;
        }
    }

    new_envp[envp_count] = NULL;  // Adiciona o terminador NULL
    return new_envp;
}

int	ft_isalnum(int c)
{
	return ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;
	size_t			i;

	i = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while ((p1[i] || p2[i]) && i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}

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

char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	len;

	len = ft_strlen(src) + 1;
	dest = (char *)malloc(len * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_memcpy(dest, src, len);
	return (dest);
}

//################################# FUNCTIONS #################################

int	is_valid(char *arg)
{
	int	i;
	int	equal;

	if (!ft_isalpha(arg[0]))
		return (0);
	i = 0;
	equal = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
			equal = 1;
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			if (equal == 0)
				return (0);
		i++;
	}
	return (equal);
}
// Check if exists
int	is_in_env(char *key)
{
	char *str;

	str = getenv(key);
	if (!str)
		return (0);
	free(str);
	return (1);
}

// Unset the actual KEY from the environment variables
void	unset_env(char *key, char ***envp)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	while ((*envp)[i])
	{
		j = 0;
		while ((*envp)[i][j] && (*envp)[i][j] != '=')
			j++;
		if (!ft_strncmp((*envp)[i], key, j) && (*envp)[i][j] == '=' && key[j] == '\0')
		{
			free((*envp)[i]);
			k = i;
			while ((*envp)[k])
			{
				(*envp)[k] = (*envp)[k + 1];
				k++;
			}
			break ;
		}
		i++;
	}
}

int	free_and_error(char ***envp, char *error)
{
	free_mat(*envp);
	perror(error);
	return (0);
}

// Append a new KEY=VALUE to the environment variables
int	append_env(char *key_value, char ***envp)
{
	int		envp_count;
	int		i;
	char	**new_envp;

	envp_count = 0;
	while ((*envp)[envp_count])
		envp_count++;
	new_envp = (char **)malloc(sizeof(char *) * (envp_count + 2));
	if (!new_envp)
		return (0);
	i = 0;
	while (i < envp_count)
	{
		new_envp[i] = ft_strdup((*envp)[i]);
		if (new_envp[i] == NULL)
			return (free_and_error(&new_envp, "Error strdup()\n"));
		i++;
	}
	new_envp[i] = ft_strdup(key_value);
	if (!new_envp[i])
		return (free_and_error(&new_envp, "Error strdup()\n"));
	new_envp[i + 1] = NULL;
	free_mat(*envp);
	printf("passed\n");
	*envp = new_envp;
	return (1);
}

int	export(char *key_value, char ***envp)
{
	char	**temp;
	char	**new_env;
	int		cur;


	if (!is_valid(key_value))
	{
		print_error("export: not a valid identifier");
		return (1);
	}
	temp = ft_split(key_value, '=');
	if (!temp)
		return (1);
	if (is_in_env(temp[0]))
		unset_env(temp[0], envp);
	if (!append_env(key_value, envp))
	{
		free_mat(temp);
		return (1);
	}
	free_mat(temp);
	return (0);
}

int	exports_loop(char	**mat, char ***envp)
{
	int	i;

	i = 1;
	while (mat[i])
	{
		if (export(mat[i], envp))
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	**mat;
	char	**lexer_env;
	int		i;

	mat = (char **)malloc(sizeof(char *) * 3);
	mat[0] = ft_strdup("export");
	mat[1] = ft_strdup("Eu=Eu_sou");
	mat[2] = NULL;
	lexer_env = duplicate_envp(envp);
    if (!lexer_env) {
        return 1;
    }
	if (exports_loop(mat, &lexer_env) == 1)
	{
		free(mat[0]);
		free(mat[1]);
		free(mat);
		for (int i = 0; lexer_env[i] != NULL; i++)
		{
			free(lexer_env[i]);
		}
    	free(lexer_env);
	}
}