NAME = cub3d
BUILD ?= debug

LIBFTDIR = libft
GNLDIR = gnl
MLXDIR = minilibx-linux
LIBMLX = $(MLXDIR)/libmlx.a
GNL = $(GNLDIR)/gnl.a
LIBFT = $(LIBFTDIR)/libft.a

SRCS = \
	debug_print.c \
	scene_utils.c \
	element_parsing.c \
	map_parsing.c \
	game.c \
	game_events.c \
	game_init.c \
	graphics_utils.c \
	map_utils.c \
	map_reading.c \
	vec2_utils.c

HDRS = game.h graphics.h vec2.h libft/libft.h gnl/get_next_line.h

SRCS_GNL = $(addprefix $(GNLDIR)/, get_next_line.c)
SRCS_LIBFT = $(addprefix $(LIBFTDIR)/, ft_strncmp.c)

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
CC = cc
RM = rm -f
CPPFLAGS = -I$(MLXDIR) -I$(LIBFTDIR) -I$(GNLDIR)
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

$(NAME): $(LIBMLX) $(GNL) $(LIBFT) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(GNL) $(LIBFT) $(LDLIBS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(GNL):
	$(MAKE) -C $(GNL_DIR)

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

map1: all
	./$(MAKE) maps/map1.cub

identifier: all
	./$(MAKE) maps/map1
	
.PHONY: all clean fclean re norm
