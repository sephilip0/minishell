//if no NAME just TOKEN -> execve with tokens or printerror no command found
#include "minishell.h"
/*
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
	char	**argv_tokens;
	int		n_tokens;
	int		i;
	t_tree	*ptr;

	if (tree == NULL || tree->left == NULL)
	{
		printf("Error. No right side tree for cmd\n");
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
}*/

/*
int	possible_node(t_tree *tree)
{
	if (tree->right)
	{
		if (tree->right->id == CMD)
			return(0)


	}
}*/



//cound have been made easier by just going to the right
int	pipecount(t_tree *tree)
{
	int	npipes;

	npipes = 0;
	if (tree->left)
		npipes = pipecount(tree->left);
	if (tree->right)
		npipes = pipecount(tree->right);
	if (tree->pipe_flag == 1)
		npipes++;
	return (npipes);
}

//FAST FIXED, MAYBE REDO (ADDED 2 IFS)
//find working cmd
t_tree	*iscorrect_node(t_tree *tree, t_tree *reset)
{
	int	pipe_on;

	pipe_on = 0;
	if (tree != reset)
		pipe_on = 1;
	if (tree->left->id == CMD)
		tree = tree->left;
	while (tree)
	{
		if (tree->left->id == NAME)
		{
			//printf("CONTENT: %s %d %d\n", tree->left->content, tree->left->pipefd[1], tree->left->pipefd[0]);
			if (tree->left->pipefd[0] != -1 && tree->left->pipefd[1] != -1)
				return (tree->left);
			else
				status = 1;
			if (tree->left->pipefd[0] == -1 &&
				tree->left->pipefd[1] != -1 && tree->left->pipefd[1] > 2)
			{
				if (pipe_on)
					pipeignore(tree->left->pipefd[1]);
				//printf("ISCORRECT_NODE: CMD: %s FECHA: %d\n", tree->left->content, tree->left->pipefd[1]);
				close(tree->left->pipefd[1]);
			}
			if (tree->left->pipefd[1] == -1 &&
				tree->left->pipefd[0] != -1 && tree->left->pipefd[0] > 2)
			{
				//printf("ISCORRECT_NODE: CMD: %s FECHA: %d\n", tree->left->content, tree->left->pipefd[0]);
				close(tree->left->pipefd[0]);
			}
		}
		if (tree->right)
			tree = tree->right;
		else
			return (NULL);
	}
	return (NULL);
}

//find the first node for temporary storage of ints
t_tree	*no_cmd(t_tree *tree)
{
	printf("NO CMD WAS FOUND BUT KEEP GOING...\n");
	if (tree->left->id == CMD)
		tree = tree->left;
	if (tree->left)
		return (tree->left);
	else
		return (NULL);
}

void	closenodefds(t_tree *tree)
{
	if (tree->pipefd[1] > 2)
		close(tree->pipefd[1]);
	if (tree->pipefd[0] > 2)
		close(tree->pipefd[0]);
}

//recebe o topo. esta funcao deve distinguir
t_tree	*find_cmd(t_tree *tree)
{
	//RULES: TOKEN CANNOT EXIST BEFORE COMMAND
	if (tree->left->id == CMD)
		tree = tree->left;
	while (tree)
	{
		//printf("CURRENT BULLSHIT: %s\n", tree->content);
		if (tree->left->id == TOKEN)
		{
			ft_putstr_fd(tree->left->content, 2);
			//printf("ACCESS MALUCO %d\n", access(tree->left->content, F_OK | R_OK | X_OK));
			if ((tree->left->content[0] != '/') && (tree->left->content[ft_strlen(tree->left->content) - 1] != '/') && (ft_strncmp(tree->left->content, "./", 2)))
			{
				ft_putstr_fd(": command not found\n", 2);
				status = 127;
			}
			else if (access(tree->left->content, F_OK) != -1)
			{
				if (access(tree->left->content, R_OK | X_OK) != -1)
					ft_putstr_fd(": Is a directory\n", 2);
				else
					ft_putstr_fd(": Permission denied\n", 2);
				status = 126;	
			}
			else
			{
				ft_putstr_fd(": No such file or directory\n", 2);
				status = 127;
			}
			return(NULL);
		}
		if (tree->left->id == NAME)
			return (tree->left);
		tree = tree->right;
	}
	return (NULL);
}

int	read_heredoc(int piper[2], char *eof, t_lexer *lexer, int dolexpand)
{
	char	*gnl;
	int		pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Error heredoc fork()\n");
		return (-1);
	}
	if (pid == 0)
	{
		status = 0;
		setup_signal_handlers(HEREDOC);
		close(piper[0]);
		while (1)
		{
			gnl = readline(">");
			if (gnl == NULL)
			{
				if (status == 130)
					printf("AFINAL FOI O CTRL C\n");
				break ;
			}
			if (ft_strlen(gnl) == (ft_strlen(eof)) &&
				(ft_strncmp(gnl, eof, ft_strlen(eof)) == 0))
				break;
			if (!dolexpand)
				ft_putstr_fd(gnl, piper[1]);
			else
			{
				gnl = newexpander(gnl, lexer, dolexpand);
				//gnl = expanddollar(gnl, lexer, dolexpand);
				//printf("RLASJDLASKDJALSKDJALKDJA: %s\n", gnl);
				if (*gnl)
				{
					printf("LOOOOOOOOOOOOOOOOOOOOL\n");
					ft_putstr_fd(gnl, piper[1]);
				}
			}
			ft_putstr_fd("\n", piper[1]);
			if (*gnl)
				free(gnl);
		}
		if (status != 130)
			printf("bash: warning: here-document delimited by end-of-file\n");
		close(piper[1]);
		free_mat(lexer->env);
		tree_free(lexer->tree);
		exit(status);
	}
	printf("UPS PARENT1: %d\n", status);
	waitpid(pid, &status, 0);
	get_status(status);
	printf("UPS PARENT: %d\n", status);
	return (pid);
}

int	ft_heredoc(char *eof, t_lexer *lexer, int dolexpand)
{
	int		piper[2];
	int		child;

	fprintf(stderr, "HEREDOC ACTIVATED. EXPAND: %d\n", dolexpand);
	//fprintf(stderr, "STATUS BEFORE HEREDOC: %d\n", status);
	if (pipe(piper) == -1)
	{
		perror("ERROR HEREDOC\n");
		return (-1);
	}
	setup_signal_handlers(IGNORE);
	child = read_heredoc(piper, eof, lexer, dolexpand);
	if (child == -1)
		return (-1);
	printf("STATUS1: %d\n", status);
	//waitpid(child, &status, 0);
	printf("STATUS2: %d\n", status);
	close(piper[1]);
	//setup_signal_handlers(MAIN);
	return (piper[0]);
}

//fd = 1 by default. If no redir found it will write in the stdout
// <file1 >file1 cat faz com que o file fique blank. O OPENOUTFILE TEM DE VIR PRIMEIRO O OPENINFILE PORQUE ELE TRUNCA E APAGA AS CENAS DE LA?

int	openoutfile(t_tree *tree)
{
	int	fd;

	fd = 1;
	if (tree->left->redir_type == 2)
	{
		fd = open(tree->left->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
		//printf("ABERTO:%s com fd: %d\n", tree->left->content, fd);
	}
	if (tree->left->redir_type == 4)
	{
		fd = open(tree->left->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		//printf("ABERTO:%s com fd: %d\n", tree->left->content, fd);
	}
	if (fd == -1)
	{
		perror(tree->left->content);
		//printf("%s: No such file or directory\n", tree->left->content);
		return(-1);
	}
	return(fd);
}

//int	redir_intree(t_tree *tree, int redir_type)

// -1 error
//fd = 0 by default. If no redir found it will read from stdin
int	openinfile(t_tree *tree, t_lexer *lexer)
{
	int	fd;

	fd = 0;
	if (tree->left->redir_type == 1)
	{
		fd = open(tree->left->content, O_RDONLY);
		//printf("ABERTO:%s com fd: %d\n", tree->left->content, fd);
	}
	if (tree->left->redir_type == 3)
		fd = ft_heredoc(tree->left->content, lexer, tree->left->herexpand);
	if (fd == -1)
	{
		perror(tree->left->content);
		//printf("%s: No such file or directory\n", tree->left->content);
		return(-1);
	}
	return(fd);
}

//we rewrite cmd->pipe but infile and outfile stay for close
int	openfiles(t_tree *tree, t_tree *cmd, t_lexer *lexer)
{
	int	infile;
	int	outfile;
	int	fdstatus;

	infile = 0;
	outfile = 1;
	if (tree->left->id == CMD)
		tree = tree->left;
	if (tree->left->redir_type == 2 || tree->left->redir_type == 4)
	{
		outfile = openoutfile(tree);
		//printf("OPENOUTFILE: %d\n", outfile);
		cmd->pipefd[0] = outfile;
	}
	if (tree->left->redir_type == 1 || tree->left->redir_type == 3)
	{
		infile = openinfile(tree, lexer);
		//printf("OPENINFILE: %d\n", infile);
		cmd->pipefd[1] = infile;
	}
	if (infile == -1 || outfile == -1)
		return (-1);
	//printf("oFILE: %d CMD: %d\n", outfile, cmd->pipefd[0]);
	//printf("iFILE: %d CMD: %d\n", infile, cmd->pipefd[1]);
	if (tree->right && tree->right->left->id == REDIR)
	{
		fdstatus = openfiles(tree->right, cmd, lexer);
		if (infile != cmd->pipefd[1])
		{
			//printf("CLOSECLEAN INNFILE: %d\n", infile);
			closeclean(infile, -1);
		}
		if (outfile != cmd->pipefd[0])
		{
			//printf("CLOSECLEAN OUTFILE: %d\n", outfile);
			closeclean(outfile, -1);
		}
		if (fdstatus == -1)
		{
			//printf("CLOSECLEAN PIPEFD[1]: %d\n", cmd->pipefd[1]);
			//printf("CLOSECLEAN PIPEFD[0]: %d\n", cmd->pipefd[0]);
			closeclean(cmd->pipefd[1], -1);
			closeclean(cmd->pipefd[0], -1);
			return (-1);
		}
	}
	return (0);
}

int	error_return(char *str, int exit_status)
{
	ft_putstr_fd(str, 2);
	return (exit_status);
}

int	closeclean(int fd, int force)
{
	if (fd > 2)
		close(fd);
	return (force);
}

//in case of problem we cannot exit, just hold the value.
int	setfds(t_tree *tree, int npipes, t_lexer *lexer)
{
	t_tree	*cmd;
	t_tree	*prev;
	int	piper[2];

	//printf("SETFDS SETFDS SETFDS SETFDS SETFD SSETFDS SETFDS SETFDS SETFDS\n");
	prev = NULL;
	while (tree)
	{
		cmd = find_cmd(tree);
		if (!cmd)
			cmd = no_cmd(tree);
		if (cmd)
		{
			openfiles(tree, cmd, lexer);
			//printf("FINAL IN: %d\n", cmd->pipefd[1]);
			//printf("FINAL OUT: %d\n", cmd->pipefd[0]);
			//LADO DIREITO DO CMD = LADO ESQUERDO DO PIPE
			//cmd->pipefd[1] = openinfile(tree);
			//ECHO HI | CAT FILE
			//cmd->pipefd[0] = openoutfile(tree);
			// se nao tiver redir e nao for o primeiro
			if (cmd->pipefd[1] == 0 && (pipecount(tree) != npipes))
			{
				//4 [4 3] 5
				if(piper[1] != -1)
				{
					cmd->pipefd[1] = piper[0];
		//			printf("TUDO CORRETO\n");
				}
			}
			else if (cmd->pipefd[1] != 0 && (pipecount(tree) != npipes))
			{
		//		printf("BROKEN PIPILINHAS IN\n");
				//5 [-1 3] 5
				if(piper[1] != -1)
					prev->pipefd[0] = -1;
		//		printf("SETFDS: CLOSE0: %d\n", piper[0]);
				piper[0] = closeclean(piper[0], -1);
		//		printf("SETFDS: CLOSE1: %d\n", piper[1]);
				piper[1] = closeclean(piper[1], -1);

			}
	//		printf("CONTENT->%s\n", cmd->content);
	//		printf("[IN: %d]\n", cmd->pipefd[1]);
	//		printf("[OUT: %d]\n", cmd->pipefd[0]);
			// if (cmd->pipefd[1] == -1 || cmd->pipefd[0] == -1)
			// 	return (-1);
			if (!(tree->pipe_flag))
			{
				// printf("BREAK FREE\n");
				break;
			}
		}
		//ATTENTION WHAT IF IT DOESNT FIND CMD?
		if (cmd && pipe(piper) == -1)
			return(error_return("PIPE ERROR\n", -1));
		//printf("PIPEZITA: %d %d\n", piper[1], piper[0]);
		if (cmd && cmd->pipefd[0] == 1)
			cmd->pipefd[0] = piper[1];
		else if (cmd)
		{
	//		printf("BROKEN PIPILINHAS OUT\n");
	//		printf("SETFDS: CLOSE0: %d\n", piper[0]);
			piper[0] = closeclean(piper[0], -1);
	//		printf("SETFDS: CLOSE1: %d\n", piper[0]);
			piper[1] = closeclean(piper[1], -1);
		}
	//	printf("FOI DADO SER AO CMD: %s:\n", cmd->content);
	//	printf("[IN: %d]\n", cmd->pipefd[1]);
	//	printf("[OUT: %d]\n", cmd->pipefd[0]);
		prev = cmd;
		if (tree->pipe_flag)
			tree = tree->right;
		else
			break ;
	}
	//printf("FINALLL\n");
	//printf("[IN: %d]\n", cmd->pipefd[1]);
	//printf("[OUT: %d]\n", cmd->pipefd[0]);
	return (0);
}

void	closeall_fd(t_tree *tree)
{
	//fprintf(stderr, "CLOSE CLOSE CLOSE CLOSE CLOSE CLOSE CLOSE CLOSE CLOSE\n");
	if (tree->id == NAME)
	{
	//	fprintf(stderr, "NAMEZITO: %d %d\n", tree->pipefd[0], tree->pipefd[1]);
		if (tree->pipefd[1] > 2)
		{
	//		fprintf(stderr, "CLOSEALL_FD: CMD: %s CLOSE: %d\n", tree->content, tree->pipefd[1]);
			close(tree->pipefd[1]);
		}
		if (tree->pipefd[0] > 2)
		{
	//		fprintf(stderr, "CLOSEALL_FD: CMD: %s CLOSE: %d\n", tree->content, tree->pipefd[0]);
			close(tree->pipefd[0]);
		}
	}
	if (tree->left)
		closeall_fd(tree->left);
	if (tree->right)
		closeall_fd(tree->right);
}

//not sure about this one
// void	waiter(t_tree *tree, t_lexer *lexer)
// {
// 	t_tree *cmd;
// 	int	status;
// 	int	exit_status;

// 	status = 0;
// 	//printf("LEXER STATUS: %d\n", lexer->status);
// 	while (tree)
// 	{
// 		cmd = iscorrect_node(tree, tree, lexer);
// 	/*	if (cmd)
// 			printf("CMD TODO CAGADO: %s\n", cmd->content);
// 		else
// 			printf("NULLO, NADA AQUI\n");*/
// 		if (cmd != NULL)
// 		{
// 	//		printf("WAITER CMD: %s CMD->FID: %d\n", cmd->content, cmd->fid);
// 			waitpid(cmd->fid, &status, 0);
// 			//printf("HUH: %d\n", status);
// 	//		printf("WIFEXITED: %d\n", WIFEXITED(status));
// 	//		printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
// 	//		printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
// 	//		printf("WTERMSIG: %d\n", WTERMSIG(status));
// 	//		printf("WCOREDUMP: %d\n", WCOREDUMP(status));
// 	//		printf("WIFSTOPPED: %d\n", WIFSTOPPED(status));
// 	//		printf("WSTOPSIG: %d\n", WSTOPSIG(status));
// 	//		printf("WIFCONTINUED: %d\n", WIFCONTINUED(status));
// 	//		printf("PRE_STATUS %d\n", lexer->status);
// 			if (WIFEXITED(status))
// 			{
// 				exit_status = WEXITSTATUS(status);
// 				lexer->status = exit_status;
// 			}
// 			else
// 				lexer->status = status;
// 	//		printf("POS_STATUS %d\n", lexer->status);
// 		}
// 		if (tree->pipe_flag == 1)
// 			tree = tree->right;
// 		else
// 			break;
// 	}
// }

void	get_status(int	sts)
{
	int	sig;

	if (WIFSIGNALED(sts))
	{
		sig = WTERMSIG(sts);
		if (sig == SIGINT)
		{
			status = 130;
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		else if (sig == SIGQUIT)
		{
			status = 131;
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		}
	}
	else if (WIFEXITED(sts))
		status = WEXITSTATUS(sts);
	else
		status = sts;
}

//not sure about this one
void	waiter(t_tree *tree)
{
	t_tree *cmd;

	//printf("LEXER STATUS: %d\n", lexer->status);
	while (tree)
	{
		cmd = iscorrect_node(tree, tree);
		if (cmd != NULL)
		{
			waitpid(cmd->fid, &status, 0);
			get_status(status);
		}
		if (tree->pipe_flag == 1)
			tree = tree->right;
		else
			break;
	}
}

//was outputting to 2
void	print_mat(char **mat)
{
	int	i;

	i = 0;
	//printf("========================\n");
	while (mat[i])
	{
		ft_putstr_fd(mat[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	//printf("========================\n");
}
/*
char	**assembly_tokens(t_tree *tree)
{
	t_tree *cur;
	char  **mat;
	int	n_tokens;

	n_tokens = 0;
	if (tree->left->id == CMD)
		tree = tree->left;
	cur = tree;
	while (cur)
	{
		if (cur->left->id == TOKEN || cur->left->id == NAME)



	}
	print_mat(mat);
	return (mat);
}
*/
int	find_nid(t_tree *tree, int id)
{
	int i;

	i = 0;
	if (tree->left)
		i += find_nid(tree->left, id);
	if (tree->right)
		i += find_nid(tree->right, id);
	if (tree->id == id)
		i++;
	return(i);

}

int	executive(t_tree *tree, t_tree *reset, t_lexer *lexer)
{
	t_tree *cmd;
	char	**full;

	if (setfds(tree, pipecount(tree), lexer) == -1)
		return (-1);
	//printf("ELEXER STATUS: %d\n", lexer->status);
	while (tree)
	{
		cmd = iscorrect_node(tree, reset);
		if (!cmd)
			closenodefds(no_cmd(tree));
		//printf("ELEXER STATUS: %d\n", lexer->status);
		if (cmd != NULL)
		{
			full = join_tokens(tree);
			setup_signal_handlers(IGNORE);
			cmd->fid = fork();
	//		printf("--------------CMD: %s CMD->FID: %d---------------------\n", cmd->content, cmd->fid);
			if (cmd->fid < 0)
				return(error_return("NO FORK\n", -1));
			if (cmd->fid == 0)
			{
	//			printf("FINAL IN: %d OUT: %d\n", cmd->pipefd[1], cmd->pipefd[0]);
				setup_signal_handlers(CHILD);
				dup2(cmd->pipefd[1], STDIN_FILENO);
				dup2(cmd->pipefd[0], STDOUT_FILENO);
				closeall_fd(reset);
				builtins(reset, full, lexer, cmd->pipefd[1]);
	//			fprintf(stderr, "VOU COMECAR A METER MERDAS\n");
				if (execve(cmd->content, full, lexer->env) == -1)
				{
					//closeall_fd(tree);
					free_mat(lexer->env);
					free_mat(full);
					tree_free(reset);
				}
			}
			if (cmd->pipefd[1] > 2)
			{
	//			printf("EXECUTIVE: CMD: %s CLOSE: %d\n", cmd->content, cmd->pipefd[1]);
				close(cmd->pipefd[1]);
				//cmd->pipefd[1] = 0;
			}
			if (cmd->pipefd[0] > 2)
			{
	//			printf("EXECUTIVE: CMD: %s CLOSE: %d\n", cmd->content, cmd->pipefd[0]);
				close(cmd->pipefd[0]);
				//cmd->pipefd[0] = 0;
			}
			free_mat(full);
			//wait(&(cmd->fid));
		}
		if (tree->pipe_flag == 1)
			tree = tree->right;
		else
			break;
	}
	//printf("ELEXER STATUS: %d\n", lexer->status);
	waiter(reset);
	return (0);
}
