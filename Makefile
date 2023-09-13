# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ookamonu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/27 19:22:04 by ookamonu          #+#    #+#              #
#    Updated: 2023/09/14 00:46:56 by ookamonu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

SERVER = server
CLIENT = client

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all : $(SERVER) $(CLIENT)

$(SERVER): server.c libft.o
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT): client.c libft.o 
	$(CC) $(CFLAGS) $^ -o $@

clean : 
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(SERVER) $(CLIENT)

re : fclean all
