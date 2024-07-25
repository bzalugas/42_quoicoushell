# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 18:41:09 by bazaluga          #+#    #+#              #
#    Updated: 2024/07/25 18:36:47 by jsommet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	    :=	minishell

SRCDIR	    :=	src

OBJDIR	    :=	obj

INCDIR	    :=	include

LIBFTDIR    :=	$(INCDIR)/libft

LIBFT	    :=	$(LIBFTDIR)/libft.a

SRC			:=	builtins.c exec_end_child.c exec_handle_streams.c exec_heredoc.c \
				exec_main.c free_cmds.c ft_export.c minishell_utils.c sighandlers.c \
				token_split.c token_split_utils.c tokenize.c variables.c variables2.c \
				variables_utils.c

MAIN		:=	main.c

OBJ			:=  $(SRC:.c=.o)

OBJMAIN		:= $(MAIN:.c=.o)

SRC	    	:=  $(addprefix $(SRCDIR)/, $(SRC))

OBJ	    	:=  $(addprefix $(OBJDIR)/, $(OBJ))

OBJMAIN    	:=  $(addprefix $(OBJDIR)/, $(OBJMAIN))

CC	    	:=  cc

CFLAGS	    :=  -Wall -Wextra -Werror -I$(INCDIR) -g3

########### COLORS ##########

RED		:=  "\033[1;31m"
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

$(NAME):	$(OBJ) $(OBJMAIN) $(LIBFT)
	@echo $(GREEN)"LINKING mandatory objects to create $(NAME)"
	$(CC) $(OBJ) $(OBJMAIN) -lreadline -L$(LIBFTDIR) -lft -o $(NAME)
	@printf $(RESET)

libft:		$(LIBFT)
	@make -sC $(LIBFTDIR)

test_exec:	$(LIBFT) $(OBJ) | $(OBJDIR)
			@cc src/testing/*.c $(OBJ) -L$(LIBFTDIR) -lft -lreadline -o $(OBJDIR)/test_exec
			./$(OBJDIR)/test_exec

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

.PHONY:		all clean fclean re bonus libft test_exec
