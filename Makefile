# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 18:41:09 by bazaluga          #+#    #+#              #
#    Updated: 2024/07/17 17:30:07 by jsommet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	    :=	minishell

SRCDIR	    :=	src

OBJDIR	    :=	obj

INCDIR	    :=	include

LIBFTDIR    :=	$(INCDIR)/libft

LIBFT	    :=	$(LIBFTDIR)/libft.a

SRC	    :=	main.c ft_readline.c minishell_utils.c sighandlers.c variables.c variables2.c variables_utils.c token_split.c token_split_utils.c tokenize.c

OBJ	    :=  $(SRC:.c=.o)

SRC	    :=  $(addprefix $(SRCDIR)/, $(SRC))

OBJ	    :=  $(addprefix $(OBJDIR)/, $(OBJ))

CC	    :=  cc

CFLAGS	    :=  -Wall -Wextra -Werror -I$(INCDIR) -g3

########### COLORS ##########

RED	:=  "\033[1;31m"
GREEN	:=  "\033[1;32m"
RESET	:=  "\033[0m"

all:		$(NAME)

bonus:		$(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c Makefile | $(OBJDIR)
	@printf $(GREEN)
	$(CC) $(CFLAGS) -MMD -c $< -o $@
	@printf $(RESET)

$(LIBFT):
	@echo $(GREEN)"\n\tCOMPILING LIBFT"$(RESET)
	@make -sC $(LIBFTDIR)
	@echo $(GREEN)"\n\tLIBFT COMPILED"$(RESET)

$(NAME):	$(OBJ) $(LIBFT)
	@echo $(GREEN)"LINKING mandatory objects to create $(NAME)"
	$(CC) $(OBJ) $(CFLAGS) -L$(LIBFTDIR) -lft -lreadline -o $(NAME)
	@printf $(RESET)

libft:		$(LIBFT)
	@make -sC $(LIBFTDIR)

clean:
	@echo $(RED)"CLEANING OBJS"
	rm -rf $(OBJDIR)
	@make -sC $(LIBFTDIR) clean
	@echo $(RESET)

fclean:		clean
	@echo $(RED)"CLEANING ALL"
	rm -f $(NAME)
	rm -rf *.dSYM
	@make -sC $(LIBFTDIR) fclean
	@echo $(RESET)

re:		fclean
	@make -s all

-include	$(OBJ:.o=.d)

.PHONY:		all clean fclean re bonus libft
