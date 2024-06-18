/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redirection.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:11:29 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/06/18 18:49:13 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./minishell.h"

void	read_here_doc(int write_pipe, char *eof)
{
	char	*str;

	str = NULL;
	while (1)
	{
		str = get_next_line(STDIN_FILENO);
		if (ft_strncmp(str, eof, ft_strlen(eof)) == 0)
		{
			free(str);
			return ;
		}
		write(write_pipe, str, ft_strlen(str));
		free(str);
	}
}

void	open_here_doc(char	*eof, t_lexer *lexer)
{
	int		fd_pipe[2];
	pid_t	child_pid;

	if (pipe(fd_pipe) == -1)
		print_error("Error creating pipe. open_here_doc()\n");
	child_pid = fork1();
	if (child_pid == 0)
	{
		close(fd_pipe[0]);
		read_here_doc(fd_pipe[1], eof);
		close(STDIN_FILENO);
		close(fd_pipe[1]);
	}
	waitpid(child_pid, &lexer->status, 0);
	close(fd_pipe[1]);
	dup2(fd_pipe[0], STDIN_FILENO);
	close(fd_pipe[0]);
}

void	set_redirection(t_tree *tree, t_lexer *lexer)
{
	int	fd;

	fprintf(stderr, "Set Redirection Running\n");
	if (tree == NULL || tree->redir_type == 0 || tree->content == NULL)
		print_error("Error. Unable to set redirection\n");
	if (tree->redir_type == 1) // < QUEREMOS QUE FICHEIRO SEJA STDIN, LER DESTE FICHEIRO
	{
		fd = open(tree->content, O_RDONLY);
		if (fd < 0)
			print_error("Error. Unable to open the < file\n");
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			print_error("Error. Enable to change the STDIN\n");
		}
		close(fd);
	}
	else if (tree->redir_type == 2) // > QUEREMOS QUE FICHEIRO SEJA STOUT, PRINTAR PARA ESTE FICHEIRO, OVERWRITE
	{
		fd = open(tree->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			print_error("Error. Unable to open the > file\n");
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			print_error("Error. Enable to change the STDOUT\n");
		}
		close(fd);
	}
	else if (tree->redir_type == 3) // << FUNÇÃO HERE_DOC
		open_here_doc(tree->content, lexer);
	else if (tree->redir_type == 4) // >> QUEREMOS QUE FICHEIRO SEJA STOUT, PRINTAR PARA ESTE FICHEIRO, APPEND
	{
		fd = open(tree->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			print_error("Error. Unable to write to the >> file\n");
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			print_error("Error. Enable to change the STDOUT\n");
		}
		close(fd);
	}
	return ;
}
