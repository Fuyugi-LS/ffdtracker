# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/15 17:41:24 by nsongsit          #+#    #+#              #
#    Updated: 2025/11/15 22:10:20 by nsongsit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libfdtracker.a

CC			= cc
FLAGS		= -Wall -Wextra -Werror
AR			= ar rcs

HEADERS		= fdtracker.h \
			fdtracker_utils.h

SRCS		= fd_close_all.c \
			fd_close_partial.c \
			fd_perror.c \
			fd_register.c \
			fd_storage.c

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Building library $(NAME)..."
	@$(AR) $(NAME) $(OBJS)
	@echo "$(NAME) is ready."

%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)

fclean: clean
	@echo "Cleaning library file..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
