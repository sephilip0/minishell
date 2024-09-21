/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_sigaction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetrukh <dpetrukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:39:27 by dpetrukh          #+#    #+#             */
/*   Updated: 2024/09/21 11:40:47 by dpetrukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


// Manipulador para SIGINT (Ctrl+C)
void handle_sigint(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void handle_heredoc(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;

	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		// rl_on_new_line();
		// rl_replace_line("", 0);
		status = 130;
		close(STDIN_FILENO);
	}
}

void	sig_ignore(struct sigaction *sa, int signal)
{
	struct sigaction	sa_origin;

	sa->sa_handler = SIG_IGN;
	sa->sa_flags = 0;
	sigemptyset(&sa->sa_mask);
	sigaction(signal, sa, &sa_origin);
}

void	signal_main(struct sigaction sa)
{
	sa.sa_sigaction = handle_sigint;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction (SIGINT, &sa, NULL);
	sig_ignore(&sa, SIGQUIT);
}

void	signal_child(struct sigaction sa)
{
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask) != 0)
		return ;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	signal_heredoc(struct sigaction sa)
{
	sa.sa_sigaction = handle_heredoc;
	sa.sa_flags = 0;
	if (sigemptyset(&sa.sa_mask) != 0)
		return ;
	sigaction(SIGINT, &sa, NULL);
	sig_ignore(&sa, SIGQUIT);
}

void	setup_signal_handlers(t_signals sig)
{
	static struct sigaction sa;

	if (sig == MAIN)
		signal_main(sa);
	if (sig == CHILD)
		signal_child(sa);
	else if (sig == HEREDOC)
		signal_heredoc(sa);
	else if (sig == IGNORE)
	{
		sig_ignore(&sa, SIGINT);
		sig_ignore(&sa, SIGQUIT);
	}
}
