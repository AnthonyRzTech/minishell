
CC = gcc

CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline -L./libft -lft

EXEC = minishell

SRC = srcs/main.c srcs/lexer/lexer.c srcs/parser/parser.c \
	  srcs/expander/expander.c srcs/executor/executor.c     \
	  srcs/built-in/env.c srcs/built-in/cd.c srcs/built-in/echo.c \
	  srcs/built-in/exit.c srcs/built-in/export.c srcs/built-in/pwd.c \
	  srcs/built-in/unset.c srcs/utils/utils1.c srcs/built-in/export2.c \
	  srcs/executor/executor2.c srcs/executor/executor3.c srcs/executor/executor4.c \
	  srcs/expander/expander2.c srcs/parser/parser2.c srcs/lexer/lexer2.c \
		srcs/lexer/lexer3.c srcs/executor/executor5.c srcs/utils/utils2.c \
		srcs/expander/expander3.c gc/gc.c srcs/utils/utils3.c srcs/utils/sigaction.c \
		srcs/executor/executor6.c srcs/executor/executor7.c

OBJ = $(SRC:.c=.o)

all: lib
	@if [ ! -f $(EXEC) ]; then $(MAKE) $(EXEC); else echo "$(EXEC) is up to date!"; fi

lib:
	@$(MAKE) -C libft

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I./include -I./libft

clean:
	@$(MAKE) -C libft clean
	rm -f $(OBJ)

fclean: clean
	@$(MAKE) -C libft fclean
	rm -f $(EXEC)

re: fclean all

.PHONY: all clean fclean re lib
