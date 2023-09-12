/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ookamonu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 01:24:19 by ookamonu          #+#    #+#             */
/*   Updated: 2023/09/12 06:59:53 by ookamonu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	end(int sig)
{
	if (sig == SIGUSR2)
		ft_putstr("\nMESSAGE RECIEVED CORRECTLY!\n\n", STDOUT_FILENO);
	return ;
}

void	send_char(char c, int server_pid)
{
	int	bit;

	bit = 0;
	while (bit <= 7)
	{
		if (c & (0b10000000 >> bit))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		++bit;
		usleep(300);
	}
}

int	main(int ac, char **av)
{
	int	i;
	int	server_pid;

	signal(SIGUSR2, end);
	i = 0;
	server_pid = ft_atoi(av[1]);
	if (ac != 3)
	{
		ft_putstr("\nPlease Enter The Correct Input: "
			"./client <Server PID> <\"Your Message\">\n\n", STDOUT_FILENO);
	}
	while (av[2][i])
		send_char(av[2][i++], server_pid);
	send_char('\0', server_pid);
}
