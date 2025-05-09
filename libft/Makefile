NAME = libft.a
SRCS = ft_strncmp.c ft_atoi.c ft_split.c ft_strlcpy.c ft_isdigit.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
