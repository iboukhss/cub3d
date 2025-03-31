NAME = cub3d

CC = cc
RM = rm -f

MAKEFLAGS += --no-print-directory

MLX_DIR = minilibx-linux

SRCS = cub3d.c
HDRS = cub3d.h

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

CFLAGS += -Wall -Wextra -Werror
CPPFLAGS = -I$(MLX_DIR)
LDFLAGS = -L$(MLX_DIR)
LDLIBS = -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

norm:
	-norminette $(SRCS) $(HDRS)

vg: $(NAME)
	valgrind --leak-check=full --track-origins=yes ./$<

.PHONY: all clean fclean re norm
