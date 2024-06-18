/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_machine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 09:22:05 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/06/18 21:50:10 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

void	print_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		print_error("Error. Fork\n");
	return (pid);
}
int	is_executable(t_tree *tree)
{
	fprintf(stderr, "Is executable? = %s\n", tree->content);
	if (tree == NULL || tree->left == NULL)
		return (0);
	if (tree->left->id == NAME && tree->pipe_flag == 0 && (tree->id == JOB || tree->id == CMD))
	{
		fprintf(stderr, "yes\n");
		return (1);
	}
	else
	{
		fprintf(stderr, "no\n");
		return (0);
	}
}

void	run_tree(t_tree *tree, t_lexer *lexer)
{
	fprintf(stderr, "Tree Running\n");
	fprintf(stderr, "ID =%i\n%s\nPIPE_FLAG =%i\n", tree->id, tree->content, tree->pipe_flag);
	if ((tree->id == JOB || tree->id == CMD) && tree->pipe_flag == 1)
		set_pipe(tree, lexer);
	else if (tree->id == REDIR)
		set_redirection(tree, lexer);
	else if (is_executable(tree))
		exec_cmd(tree, lexer);
	else if (!is_executable(tree))
	{
		if (tree->left)
		{
			fprintf(stderr, "ESQUERDA\n");
			run_tree(tree->left, lexer);
		}
		if (tree->right)
		{
			fprintf(stderr, "DIREITA\n");
			run_tree(tree->right, lexer);
		}
	}
	return ;
}

void	the_machine(t_tree *tree, t_lexer *lexer)
{
	if (fork1() == 0)
	{
		run_tree(tree, lexer);
		exit(EXIT_SUCCESS);
	}
	waitpid(0, &lexer->status, 0);
}