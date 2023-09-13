/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ookamonu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 01:24:19 by ookamonu          #+#    #+#             */
/*   Updated: 2023/09/14 00:46:34 by ookamonu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*----------------------Message-Receipt-Acknowledgment-----------------------*\
end function handles SIGUSR2 signals, indicating successful message transmission.
\*---------------------------------------------------------------------------*/
void	end(int sig)
{
	if (sig == SIGUSR2)
		ft_putstr("\nMESSAGE SENT & RECIEVED CORRECTLY!\n\n", STDOUT_FILENO);
	return ;
}

/*--------------------------[ 3 ]-Sending-Characters-------------------------*\
send_char function is responsible for sending each character as a series of bits
(0s & 1s) to server using signals (SIGUSR1 and SIGUSR2). It iterates through each
of the character's 8 bits. For each bit, it checks if it's 1 or a 0 using bitwise
operations (c & (0b10000000 >> bit)). If it's a 1, it sends SIGUSR1; if it's a 0,
it sends SIGUSR2. After sending all 8 bits, it adds a small delay (usleep(300);)
before sending the next character.
\*---------------------------------------------------------------------------*/
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
		usleep(500);
	}
}

/*------------------------[ 2 ]-Client-Initialization------------------------*\
Client takes 2 command-line args, Server's PID & Message to be sent. It also
registers "end" function 2 handle SIGUSR2 signals, indicating successful message
transmission. Client enters a loop to iterate through each character in message.
For each character, it calls the send_char function to send it to the server.
\*---------------------------------------------------------------------------*/
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
/*---------------------------[ 5 ]-End-Communication-------------------------*\
After sending all characters of the message, the Client sends a null character
('\0') to the server using the send_char function to signals to the server that
the message is complete. The Client then terminates.
\*---------------------------------------------------------------------------*/