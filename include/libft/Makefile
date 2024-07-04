#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/04 16:22:14 by bazaluga          #+#    #+#              #
#    Updated: 2024/07/04 19:36:54 by bazaluga         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME	:=	libft.a

SRC		:=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
			ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
			ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
			ft_atoi.c ft_calloc.c ft_strdup.c\
			\
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
			ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
			ft_putendl_fd.c ft_putnbr_fd.c\
			\
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			ft_lstmap.c \
			\
			ft_isspace.c ft_strndup.c ft_atol.c ft_utohex.c ft_max.c \
			ft_max_arr.c ft_min.c ft_min_arr.c ft_abs.c ft_lstremove.c \
			\
			ft_printf.c ft_printf_buffer.c ft_printf_buffer2.c \
			ft_printf_char_str_handlers.c ft_printf_convert_buffer.c \
			ft_printf_flags_parsing.c ft_printf_float_handlers.c \
			ft_printf_helpers.c ft_printf_hex_handlers.c ft_printf_int_handlers.c \
			ft_printf_node.c ft_printf_uint_handlers.c ft_printf_utils.c \
			ft_ftoa.c ft_ftoa_printf.c ft_strdup_free.c ft_strjoin_free.c\
			free_split.c ft_strcmp.c \
			\
			get_next_line.c get_next_line_utils.c

OBJ		:=	$(SRC:.c=.o)

CC		:=	cc

CFLAGS	:=	-Wall -Wextra -Werror

########### COLORS ##########

RED		:=	"\033[31m"
GREEN	:=	"\033[32m"
RESET	:=	"\033[0m"

all:		$(NAME)

.c.o:
			@echo $(GREEN)"Compiling "$<$(RESET)
			@$(CC) $(CFLAGS) -o $(<:.c=.o) -c $<

$(NAME):	$(OBJ)
			@echo $(GREEN)"\nAdding obj files to "$(NAME)$(RESET)
			@ar -rcs $(NAME) $(OBJ)

bonus:		$(NAME)

ifneq ($(shell uname), Darwin)
breaker:
			$(CC) -nostartfiles -shared -fPIC -ldl $(CFLAGS) -o libft.so $(SRC) $(SRCB)
else
breaker:
			$(CC) -dynamiclib $(CFLAGS) -o libft.so $(SRC) $(SRCB) -L../obj -lmalloc
endif

ifneq ($(shell uname), Darwin)
so:
			$(CC) -nostartfiles -shared -fPIC -ldl $(CFLAGS) -o libft.so $(SRC) $(SRCB)
else
so:
			$(CC) -dynamiclib $(CFLAGS) -o libft.so $(SRC) $(SRCB) -L../obj -lmalloc
endif

clean:
			@echo $(RED)"CLEANING OBJS"
			@rm -f $(OBJ)
			@echo $(RESET)

fclean:		clean
			@echo $(RED)"CLEANING ALL"
			@rm -f $(NAME)
			@rm -f libft.so
			@rm -f *.out
			@rm -rf *.dSYM
			@echo $(RESET)

re:			fclean
			make -s all

.PHONY:		all clean fclean re
