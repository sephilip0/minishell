/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:35:16 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/08/12 15:12:51 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

void print_error(char *message) {
	// Escrever a mensagem de erro para STDERR
	write(STDERR_FILENO, message, strlen(message));
}
/*
int ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
	|| c == '\v' || c == '\f' || c == '\r');
}

int	is_cd(char *line, char **param)
{
	while(*line && ft_isspace(*line))
		line++;
	if (ft_strncmp(line, "cd", 2) != 0 || (!ft_isspace(line[2]) && line[2] && line[2] != '\0')) //Invalid: "da asd"
		return (0);
	line += 2;
	while(*line && ft_isspace(*line))// Espaçamento entre cd e param
		line++;
	if (*line == '\0') // Valid: "cd    "
		return (1);
	*param = line;
	while (*line && !ft_isspace(*line)) // Encontrar o fim do param
		line++;
	while (*line && ft_isspace(*line)) // Espaçamento depois do 1º param
		line++;
	if (*line == '\0') //valid: "   cd     param     "
		return (1);
	print_error("B_CD: TOO MANY ARGUMENTS\n");
	return (0); //invalid: "  cd    param    param"
}

void	built_cd(char *param)
{
	if (param == NULL || !ft_strncmp(param, "~", 2) || !ft_strncmp(param, "~/", 3))
	{
		printf("param: %s\n",param);
		chdir(getenv("HOME"));
	}
	else if (chdir(param) == -1)
		print_error("B_CD: NO SUCH FILE OR DIRECTORY\n");
}*/

int	b_cd(char **mat, int tofree)
{
	if(mat[1] && mat[2])
 	{
		if (tofree)
			free_mat(mat);
 		print_error("B_CD: too many arguments\n");
 		return (1);
 	}
 	if (mat[1] == NULL || !strncmp(mat[1], "~", 2) || !strncmp(mat[1], "~/", 3))
 		chdir(getenv("HOME"));
 	else if (chdir(mat[1]) == -1)
 	{
		if (tofree)
			free_mat(mat);
 		print_error("B_CD: No such file or directory\n");
 		return (1);
 	}
	if (tofree)
		free_mat(mat);
	return (0);
}
