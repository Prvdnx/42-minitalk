/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ookamonu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 23:15:25 by ookamonu          #+#    #+#             */
/*   Updated: 2023/09/12 07:00:23 by ookamonu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	bit_handler(int sig, siginfo_t *info, void *more_info)
{
	static char	c = 0b00000000;
	static int	bit = 0;

	(void)more_info;
	if (SIGUSR1 == sig)
		c |= (0b10000000 >> bit);
	++bit;
	if (8 == bit)
	{
		if (c == '\0')
		{
			write(STDOUT_FILENO, "\n", 1);
			kill(info->si_pid, SIGUSR2);
		}
		else
			write(STDOUT_FILENO, &c, 1);
		c = 0b00000000;
		bit = 0;
	}
}

int	main(int ac, char **av)
{
	struct sigaction	sa;

	if (ac != 1)
	{
		ft_putstr("\nEnter Process Name Only: ./server\n", STDOUT_FILENO);
		return (1);
	}
	(void)av;
	ft_putstr("\nWelcome To BAD-HUMAN's Server!\n Server PID = ", STDOUT_FILENO);
	ft_putnbr(getpid());
	write(STDOUT_FILENO, "\n\n", 3);
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = bit_handler;
	if ((sigaction(SIGUSR1, &sa, NULL) == -1)
		|| (sigaction(SIGUSR2, &sa, NULL) == -1))
	{
		ft_putstr("\nServer Error\n\n", STDOUT_FILENO);
		return (1);
	}
	while (1)
		pause();
}
