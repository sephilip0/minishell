/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 08:38:28 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/08/12 15:15:28 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

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
	*envp = new_envp;
	return (1);
}

int	no_equal(char *key_value)
{
	int	i;

	i = 0;
	while(key_value[i])
	{
		if (key_value[i] =='=')
			return (0);
		i++;
	}
	return (1);
}

int	export(char *key_value, char ***envp)
{
	char	**temp;
	int		cheker;

	cheker = is_valid_key_value(key_value);
	if (cheker == 0)
	{
		print_error(" not a valid identifier");
		return (EXIT_FAILURE);
	}
	if (cheker == 1)
		return (0);
	temp = ft_split(key_value, '=');
	if (!temp)
		return (1);
	if (is_in_env(temp[0], *envp))
		unset_env(temp[0], envp);
	if (!append_env(key_value, envp))
	{
		free_mat(temp);
		return (1);
	}
	free_mat(temp);
	return (0);
}

void	bubble_sort(char **mat)
{
	char	*place_holder;
	int		i;
	int		j;
	int		k;

	i = 0;
	while(mat[i + 1])
	{
		j = i;
		while(mat[j + 1])
		{
			k = 0;
			while (mat[j][k] &&  mat[j + 1][k] && mat[j][k] == mat[j + 1][k])
				k++;
			if (mat[j][k] > mat[j + 1][k])
			{
				place_holder = mat[j];
				mat[j] = mat[j + 1];
				mat[j + 1] = place_holder;
			}
			j++;
		}
		i++;
	}
	return;
}

void	print_export(char **mat)
{
	char	**sorted_mat;
	int		i;

	sorted_mat = matcpy(mat);
	bubble_sort(sorted_mat);
	i = 0;
	while (sorted_mat[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		printf("%s\n", sorted_mat[i]);
		i++;
	}
	free_mat(sorted_mat);
}

int	exports_loop(char	**mat, t_lexer *lexer)
{
	int	i;

	i = 1;
	while (mat[i])
	{
		if (export(mat[i], &lexer->env))
		{
			status = 1;
			return (1);
		}
		i++;
	}
	if (i == 1)
		print_export(lexer->env);
	return (0);
}
