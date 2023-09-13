/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ookamonu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 23:15:25 by ookamonu          #+#    #+#             */
/*   Updated: 2023/09/13 22:15:14 by ookamonu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

/*------------------[ 4 ]-Receiving-&-Processing-Characters------------------*\
Server is still waiting in the while (1) loop to receive signals from the client.
When the server receives a SIGUSR1 signal, it interprets it as 1 bit & sets the
corresponding bit in the character being built (c).	Then increments the bit
counter to move to the next bit. This process continues until all 8 bits of the
character are received.
Once a complete character is received, the server checks if it's a null character
('\0'), which marks the end of the message. If it's a null character, the server
prints a newline character to the standard output & sends *SIGUSR2* signal back
to the client to indicate successful message reception. If not a null character,
the server simply writes the received character to the standard output.
After processing a character, the server resets the character and bit counters
for the next character.
\*---------------------------------------------------------------------------*/
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

/*------------------------[ 1 ]-Server Initialization------------------------*\
The server initializes, it prints a welcome message & its Process ID (PID)
to standard output. It sets up signal handling using sigaction for both SIGUSR1 &
SIGUSR2. The server enters an infinite loop "while (1) pause();" wherenit waits
for incoming signals. This loop runs indefinitely, ready to receive messages from
the client.
\*---------------------------------------------------------------------------*/
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

/*
struct sigaction
{
	void (*sa_handler)(int);  =Pointer to signal handling function (deprecated).
	void (*sa_sigaction)(int, siginfo_t *, void *); =signal handl func (modern).
	sigset_t sa_mask;  =Set of signals to be blocked during execution of handler.
	int sa_flags;         =Special flags controlling the behavior of the signal.
	void (*sa_restorer)(void);   =Not used in modern implementations.
};
Tester --> https://github.com/ThibaudM13/minitalk-Tester/blob/main/tester.sh
*/