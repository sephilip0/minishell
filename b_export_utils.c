/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_export_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:33:32 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/08/07 10:19:43 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

int	free_and_error(char ***envp, char *error)
{
	free_mat(*envp);
	perror(error);
	return (0);
}

int	is_in_env(char *key, char **envp)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (!ft_strncmp(key, envp[i], key_len) && envp[i][key_len] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_key_value(char *arg)
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
				return (0);  //INVALIDO
		i++;
	}
	if (!equal) // VALIDO MAS NÃO TEM =
		return (1);
	return (2); // VALIDO
}