/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_pipe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:10:53 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/06/18 22:04:00 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

void	set_pipe(t_tree *tree, t_lexer *lexer)
{
	int		pipe_fd[2];
	pid_t	child_pid;

	fprintf(stderr, "set_pipe\n");
	if (pipe(pipe_fd) == -1)
		print_error("Error creating pipe. set_pipe()");
	child_pid = fork1();
	if (child_pid == 0)
	{
		fprintf(stderr, "1st pid\n");
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		if (tree->left)
			run_tree(tree->left, lexer);
		tree_free(tree);
		free_mat(lexer->env);
	}
	waitpid(child_pid, &lexer->status, 0);
	fprintf(stderr, "2st pid\n");
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[1]);
	if (tree->right)
		run_tree(tree->right, lexer);
	close(pipe_fd[0]);
	waitpid(child_pid, &lexer->status, 0);
	tree_free(tree);
	free_mat(lexer->env);
	return ;
}
