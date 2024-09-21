/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_exec_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:04:39 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/09/06 17:11:47 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

//Transforma /usr/bin/ls = ls
char	*set_command(char *str)
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
char	**join_tokens(t_tree *tree)
{
	char	**mat;
	int		n_tokens;
	int		i;

	if (tree->left->id == CMD)
		tree = tree->left;
	while(tree && tree->left->id != NAME)
		tree = tree->right;
	//fprintf(stderr, "entrou: %s\n", tree->left->content);
	n_tokens = 1;
	if (tree->right)
	{
		//fprintf(stderr, "LOLAGEM\n");
		n_tokens += find_nid(tree->right, TOKEN);
	}
	//fprintf(stderr, "N_TOKENS: %d\n", n_tokens);
	mat = ft_calloc(sizeof(char *), (n_tokens + 1));
	if (mat == NULL)
	{
		perror("Error. Malloc() join_tokens()\n");
		return (NULL);
	}
	mat[0] = set_command(tree->left->content);
	if (tree->right)
	{
		i = 1;
		while (i < n_tokens)
		{
			tree = tree->right;
			mat[i] = ft_strdup(tree->left->content);
	//		tree = tree->right;
			i++;
		}
	}
	//fprintf(stderr, "join_tokens[0]: %s\n", mat[0]);
	return (mat);
}


/*

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
		free_argv_tokens(argv_tokens, "Invalid command or arguments\n");
	if (builtins(argv_tokens, 0, lexer) == 0)
	{
		printf("FOI BUILTIN\n");
		free_argv_tokens(argv_tokens, NULL);
		tree_free(tree);
		free_mat(lexer->env);
		return ;
		exit(EXIT_SUCCESS);
	}
	printf("here we go\n");
	if (execve(tree->left->content, argv_tokens, NULL) == -1)
	{
		free_argv_tokens(argv_tokens, "Invalid command or arguments\n");
		tree_free(tree);
		free_mat(lexer->env);
	}
}*/
