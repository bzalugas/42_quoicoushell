# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/04 18:41:09 by bazaluga          #+#    #+#              #
#    Updated: 2024/10/04 12:54:21 by jsommet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	    :=	minishell

SRCDIR	    :=	src

OBJDIR	    :=	obj

INCDIR	    :=	include

LIBFTDIR    :=	$(INCDIR)/libft

LIBFT	    :=	$(LIBFTDIR)/libft.a

SRC			:=	builtins.c \
				env_utils.c \
				exec_end_child.c \
				exec_handle_streams.c \
				exec_heredoc.c \
				exec_main.c \
				expand_fhd.c \
				expand_utils.c \
				expand_utils2.c \
				expand.c \
				free_cmds.c \
				ft_cd.c \
				ft_echo.c \
				ft_env.c \
				ft_exit.c \
				ft_export.c \
				ft_export_utils.c \
				ft_pwd.c \
				ft_unset.c \
				get_paths.c \
				history.c \
				minishell_utils.c \
				parsing_utils.c \
				parsing_utils2.c \
				parsing.c \
				sighandlers.c \
				syntax_utils.c \
				syntax.c \
				token_split.c \
				token_split_utils.c \
				utils.c \
				variables.c \
				variables2.c

MAIN		:=	main.c

OBJ			:=  $(SRC:.c=.o)

OBJMAIN		:= $(MAIN:.c=.o)

SRC	    	:=  $(addprefix $(SRCDIR)/, $(SRC))

OBJ	    	:=  $(addprefix $(OBJDIR)/, $(OBJ))

OBJMAIN    	:=  $(addprefix $(OBJDIR)/, $(OBJMAIN))

CC	    	:=  cc

CFLAGS	    :=  -Wall -Wextra -Werror -I$(INCDIR) -g3

LIBFLAGS	:=	-L$(LIBFTDIR)

ifeq ($(shell uname), Darwin)
CFLAGS		+=	-I/usr/local/opt/readline/include
LIBFLAGS	+=	-L/usr/local/opt/readline/lib
endif

########### COLORS ##########

RED		:=  "\033[1;31m"
GREEN	:=  "\033[1;32m"
RESET	:=  "\033[0m"

PRINTED_MSG	:=	0

all:		$(NAME)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c Makefile | $(OBJDIR)
	@if [ "$(PRINTED_MSG)" -eq 0 ]; then \
		printf $(GREEN)"\n\tCOMPILING SOURCES\n"; \
		$(eval PRINTED_MSG := 1) \
	fi
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

$(LIBFT):
	@echo $(GREEN)"\n\tCOMPILING LIBFT"$(RESET)
	@make -sC $(LIBFTDIR)
	@echo $(GREEN)"\tLIBFT COMPILED"$(RESET)

$(NAME):	$(LIBFT) $(OBJ) $(OBJMAIN)
	@echo $(GREEN)"LINKING mandatory objects to create <$(NAME)>"
	@$(CC) $(OBJ) $(OBJMAIN) $(LIBFLAGS) -lft -lreadline -o $(NAME)
	@echo "<$(NAME)> Created ✅"$(RESET)

libft:		$(LIBFT)
	@make -sC $(LIBFTDIR)

test_exec:	$(LIBFT) $(OBJ) | $(OBJDIR)
			@cc src/testing/*.c $(OBJ) -L$(LIBFTDIR) -lft -lreadline -o $(OBJDIR)/test_exec
			./$(OBJDIR)/test_exec

clean:
	@echo $(RED)"CLEANING OBJS"$(RESET)
	@rm -rf $(OBJDIR)

fclean:		clean
	@echo $(RED)"CLEANING ALL"$(RESET)
	@rm -f $(NAME)
	@rm -rf *.dSYM
	@make -sC $(LIBFTDIR) fclean

re:		fclean
	@make -s all

-include	$(OBJ:.o=.d)

.PHONY:		all clean fclean re libft
