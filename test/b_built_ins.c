/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_built_ins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 09:35:08 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/08/31 14:22:11 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

int	b_pwd()
{
	char	*ptr;
	int	size;

	ptr = NULL;
	size = 1024;
	ptr = (char *)ft_calloc(size, sizeof(char));
	if (ptr == NULL)
		return (-1);
	ptr = getcwd(ptr, 1024);
	ft_putstr_fd(ptr, STDOUT_FILENO);
	ft_putchar_fd(10, STDOUT_FILENO);
	free(ptr);
	return (0);
}

int	print_env(char **env)
{
	//int i;

	//i = 0;
	//printf("ENTERED PRINT_ENV\n");
	if (env == NULL)
		return (-1);
	print_mat(env);
/*	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}*/
	return (0);
}

int is_valid_number(char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str) {
		if (!isdigit(*str))
		{
			print_error(" numeric argument required");
			return (2);
		}
		str++;
	}
	return (1);
}

int is_valid_exit(char **mat)
{
	int		i;

	i = 0;
	while (mat[i])
		i++;
	if (i > 2)
	{
		print_error(" too many arguments");
		return (0);
	}
	if (mat[1])
		return (is_valid_number(mat[1]));
	return (0);
}

void b_exit(char **mat, t_lexer *lexer, t_tree *reset)
{
	int status;
	int	valid;

	valid = is_valid_exit(mat);
	if (valid == 1)
	{
		status = atoi(mat[1]);
		if (status < 0 || status > 255)  // Garantir que o status esteja dentro do intervalo 0-255
			status = status % 256;       // Truncar para 8 bits
		free_mat(lexer->env);
		free_mat(mat);
		tree_free(reset);
		exit(status);  // Use o status diretamente
	}
	free_mat(lexer->env);
	free_mat(mat);
	tree_free(reset);
	if (valid == 2)
		exit(2);
	exit(1);  // Se não houver argumento ou argumento inválido, saia com 0
}

void	pipeignore(int fd)
{
	int	a;
	char	*buf;

	buf = ft_calloc(BUFFERSIZE, sizeof(char));
	a = read(fd, buf, BUFFERSIZE);
	if (a <= 0)
	{
		free(buf);
		return ;
	}
	while(a > 0)
	{
		free(buf);
		buf = ft_calloc(BUFFERSIZE, sizeof(char));
		a = read(fd, buf, BUFFERSIZE);
	}
	free(buf);
}


// -1 error
// 0 ok
// 1 not builtin
void	builtins(t_tree *reset, char **mat, t_lexer *lexer, int pipenbr)
{
	int	value;

	value = 1;
//	fprintf(stderr, "tree: %s\n", tree->content);
//	fprintf(stderr, "ENTREIIIIII: %d\n", value);
	if (!ft_strncmp(*mat, "exit", 5))
	{
		if (pipenbr > 0)
			pipeignore(STDIN_FILENO);
		b_exit(mat, lexer, reset);
	}
	if (!ft_strncmp(*mat, "cd", 3))
		value = b_cd(mat, 0);
	else if (!ft_strncmp(*mat, "pwd", 4))
		value = b_pwd();
	else if (!ft_strncmp(*mat, "env", 4))
		value = print_env(lexer->env);
	else if (!ft_strncmp(*mat, "echo", 5))
		value = b_echo(mat);
	else if (!ft_strncmp(*mat, "export", 7))
	{
		value = exports_loop(mat, lexer);
		print_env(lexer->env);
	}
	else if (!ft_strncmp(*mat, "unset", 6))
	{
		value = unset_loop(mat, lexer);
		print_env(lexer->env);
	}
//	fprintf(stderr, "VALOR DESTA POHA: %d\n", value);
	if (value != 1)
	{
		//if its a pipe, otherwise could be from a file
		if (pipenbr > 0)
			pipeignore(STDIN_FILENO);
		//closeall_fd(tree);
		free_mat(lexer->env);
		free_mat(mat);
		tree_free(reset);
		//fprintf(stderr, "BUILTIN WITH EXIT STATUS: %d CONTINUE...\n", value);
//		fprintf(stderr, "BUILTIN VAI SAIR COM %d\n", value);
		exit(value);
	}
	//1 meaning its no builtin so we dont exit
	return;
}
