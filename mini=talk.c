
///////////////////////////////// SERVER FUNCTIONS ///////////////////////////////////

#include "minitalk.h" // Include the necessary header file for the Minitalk project

// Function: bit_handler
// ---------------------
// This function handles the reception of individual bits sent as signals and reconstructs the character.
// Parameters:
// - sig: The received signal (either SIGUSR1 or SIGUSR2)
// - info: A pointer to information about the signal (unused in this context)
// - more_info: Additional information about the signal (unused in this context)
void	bit_handler(int sig, siginfo_t *info, void *more_info)
{
	// Define static variables to keep track of the received bits and the character being built
	static char	c = 0b00000000; // Initialize a character with all bits set to 0
	static int	bit = 0; // Initialize a bit counter to 0

	(void)more_info; // Avoid unused parameter warning

	// Check if the received signal is SIGUSR1 (represents a '1' bit)
	if (SIGUSR1 == sig)
		c |= (0b10000000 >> bit); // Set the corresponding bit in the character
	++bit; // Move to the next bit

	// If 8 bits (1 byte) have been received
	if (8 == bit)
	{
		// If the character is null (end of message)
		if (c == '\0')
		{
			write(STDOUT_FILENO, "\n", 1); // Print a newline character to the standard output
			kill(info->si_pid, SIGUSR2); // Notify the client that the message has been received
		}
		else
			write(STDOUT_FILENO, &c, 1); // Write the received character to the standard output

		c = 0b00000000; // Reset the character to 0 for the next character reception
		bit = 0; // Reset the bit counter to 0
	}
}

int	main(int ac, char **av)
{
	struct sigaction	sa; // Define a struct to configure signal handling

	// Check if the correct number of arguments is provided (server should have no arguments)
	if (ac != 1)
	{
		ft_putstr("\nEnter Process Name Only: ./server\n", 2); // Print an error message
		return (1); // Exit the program with an error code
	}
	(void)av; // Avoid unused variable warning

	// Print a welcome message and the server's PID (Process ID) to the standard output
	ft_putstr("\nWelcome To BAD-HUMAN's Server!\n Server PID = ", STDOUT_FILENO);
	ft_putnbr(getpid()); // Use getpid() to obtain the server's PID
	write(STDOUT_FILENO, "\n\n", 3); // Print two newline characters for formatting

	sigemptyset(&sa.sa_mask); // Initialize the signal mask (used for blocking signals during signal handling)
	sigaddset(&sa.sa_mask, SIGUSR1); // Add SIGUSR1 to the signal mask (block SIGUSR1 during signal handling)
	sigaddset(&sa.sa_mask, SIGUSR2); // Add SIGUSR2 to the signal mask (block SIGUSR2 during signal handling)
	sa.sa_flags = SA_SIGINFO; // Set the SA_SIGINFO flag to use sa_sigaction instead of sa_handler
	sa.sa_sigaction = bit_handler; // Set the signal handler function to bit_handler

	// Register the signal handlers for SIGUSR1 and SIGUSR2 using sigaction
	if ((sigaction(SIGUSR1, &sa, NULL) == -1) || (sigaction(SIGUSR2, &sa, NULL) == -1))
	{
		ft_putstr("\nServer Error\n\n", 2); // Print an error message if sigaction fails
		return (1); // Exit the program with an error code
	}

	while (1)
		pause(); // Wait indefinitely for signals
}




/////////////////////////////////// CLIENT FUNCTIONS ///////////////////////////////////

#include "minitalk.h" // Include the necessary header file for the Minitalk project

// Function: end
// -------------
// This function is called when SIGUSR2 is received, indicating successful message transmission.
// It prints a success message.
// Parameters:
// - sig: The received signal (SIGUSR2)
void	end(int sig)
{
	if (sig == SIGUSR2)
		ft_putstr("\nMESSAGE RECEIVED CORRECTLY!\n\n", 1); // Print a success message to the standard output
	return ;
}

// Function: send_char
// -------------------
// This function sends each character in the message as a series of bits using signals.
// Parameters:
// - c: The character to send
// - server_pid: The PID (Process ID) of the server process
void	send_char(char c, int server_pid)
{
	int	bit; // Declare a variable to represent the current bit being sent

	bit = 0; // Initialize the bit counter to 0
	while (bit <= 7) // Loop through all 8 bits of the character
	{
		// If the current bit is 1, send SIGUSR1; otherwise, send SIGUSR2
		if (c & (0b10000000 >> bit)) // Use bitwise AND and shifting to check each bit
			kill(server_pid, SIGUSR1); // Send SIGUSR1 to the server
		else
			kill(server_pid, SIGUSR2); // Send SIGUSR2 to the server
		++bit; // Move to the next bit
		usleep(500); // Add a small delay between sending each bit
	}
}

int	main(int ac, char **av)
{
	int	i; // Declare a variable for iterating through the message characters
	int	server_pid; // Declare a variable to store the server's PID

	// Register the end function to handle SIGUSR2 (successful message reception)
	signal(SIGUSR2, end);

	i = 0; // Initialize the character index to 0
	server_pid = ft_atoi(av[1]); // Convert the server's PID from a string to an integer

	// Check if the correct number of arguments is provided (client should have 3 arguments)
	if (3 == ac)
	{
		while (av[2][i]) // Loop through each character in the message string
			send_char(av[2][i++], server_pid); // Send each character to the server
		send_char('\0', server_pid); // Send a null character to indicate the end of the message
	}
	else
	{
		// Print an error message for incorrect arguments to the standard error
		ft_putstr("Please enter the correct input"
			"./client <PID> \"Message\"\n", 2);
	}
}
