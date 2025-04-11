NAME = cub3d
BUILD ?= debug

MLXDIR = minilibx-linux
LIBMLX = $(MLXDIR)/libmlx.a

SRCS = \
	game.c \
	game_events.c \
	game_init.c \
	graphics_utils.c \
	vec2_utils.c

HDRS = game.h graphics.h vec2.h

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
CC = cc
RM = rm -f
CPPFLAGS = -I$(MLXDIR)
CFLAGS = -Wall -Wextra
LDFLAGS = -L$(MLXDIR)
LDLIBS = -lmlx -lXext -lX11 -lm

ifeq ($(BUILD),release)
	CFLAGS += -Werror
else ifeq ($(BUILD),debug)
	CFLAGS += -g3
else ifeq ($(BUILD),asan)
	CFLAGS += -ggdb3 -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif

all: $(NAME)

$(NAME): $(LIBMLX) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(LIBMLX): $(MLXDIR)/.git
	$(MAKE) -C $(MLXDIR)

$(MLXDIR)/.git:
	git submodule update --init --recursive

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS)

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
