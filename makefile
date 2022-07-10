# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekraujin <ekraujin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/21 14:41:30 by mchatzip          #+#    #+#              #
#    Updated: 2022/07/10 19:19:34 by ekraujin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= g++
CFLAGS	= -Wall -Wextra -Werror -g -std=c++98 -pedantic

RM		= rm -f

NAME	= webserv

SRC		= main.cpp error_page.cpp utils.cpp handlers.cpp server.cpp config.cpp socket.cpp

OBJ		= ${SRC:.c=.o}

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:		
			$(RM) *.o

fclean:		clean
			$(RM) $(NAME)
			rm -r webserv.dSYM

re:			fclean all

.PHONY: all clean fclean re