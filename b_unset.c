/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 18:10:45 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/08/09 09:57:41 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

int	unset(char *key_value, char ***envp)
{
	if (is_in_env(key_value, *envp))
	{
		unset_env(key_value, envp);
		return (1);
	}
	return (0);
}

int	unset_loop(char **mat, t_lexer *lexer)
{
	int	i;

	i = 1;
	while (mat[i])
	{
		if (!unset(mat[i], &lexer->env))
			return (0);
		i++;
	}
	return (0);
}
