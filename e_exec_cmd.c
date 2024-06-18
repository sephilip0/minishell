/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_exec_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:04:39 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/06/18 21:48:11 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

static void	printf_argv(char **argv)
{
	int	i;

	i = 0;
	while(argv[i])
	{
		fprintf(stderr, "token[%i]: %s\n",i, argv[i]);
		i++;
	}
	if (argv[i] == NULL)
		fprintf(stderr, "token[%i]: NULL\n", i);
}

//Transforma /usr/bin/ls = ls
static char	*set_command(char *str)
{
	int		len;
	int		len_cmd;
	int		i;
	char	*cmd;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	i = len;
	while (i > 0 && str[i-1] != '/')
		i--;
	len_cmd = len - i;
	cmd = malloc(len_cmd + 1);
	if (cmd == NULL)
		return (NULL);
	cmd[len_cmd] = '\0';
	while (len_cmd > 0)
		cmd[--len_cmd] = str[--len];
	return (cmd);
}
// Percorre a árvore e cria o argv[] para o execve()
static char	**join_tokens(t_tree *tree)
{
	char	**argv_tokens;
	int		n_tokens;
	int		i;
	t_tree	*ptr;

	if (tree == NULL || tree->left == NULL)
	{
		print_error("Error. No right side tree for cmd\n");
		return (NULL);
	}
	n_tokens = 0;
	ptr = tree;
	while (ptr)
	{
		if (ptr->left->id == TOKEN || ptr->left->id == NAME)
			n_tokens++;
		ptr = ptr->right;
	}
	argv_tokens = malloc(sizeof(char *) * (n_tokens + 1));
	if (argv_tokens == NULL)
	{
		printf("Error. Malloc() join_tokens()\n");
		return (NULL);
	}
	i = 1;
	argv_tokens[0] = set_command(tree->left->content);
	tree = tree->right;
	while (i <= n_tokens && tree)
	{
		argv_tokens[i] = tree->left->content;
		tree = tree->right;
		i++;
	}
	argv_tokens[i] = (char *)0;
	return (argv_tokens);
}

void	exec_cmd(t_tree *tree, t_lexer *lexer)
{
	char	**argv_tokens;

	if (tree == NULL || lexer->env == NULL)
		return ;
	fprintf(stderr, "Join_cmd\n");
	argv_tokens = join_tokens(tree);
	fprintf(stderr, "CMD = %s\nTOKENS:\n", tree->left->content);
	printf_argv(argv_tokens);
	if (!tree->left->content || !argv_tokens[0])
	{
		print_error("Invalid command or arguments\n");
		free(argv_tokens[0]);
		free(argv_tokens);
		return ;
	}
	if (builtins(argv_tokens, 0) == 0)
	{
		free(argv_tokens[0]);
		free(argv_tokens);
		exit(EXIT_SUCCESS) ;
	}
	if (execve(tree->left->content, argv_tokens, NULL) == -1)
	{
		lexer->status = -1;
		printf("Lexer Status: %i\n", lexer->status);
		free(argv_tokens[0]);
		free(argv_tokens);
	}
	print_error("Error. Command not found\n");
}