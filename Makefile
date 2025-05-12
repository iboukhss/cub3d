NAME = cub3d
BUILD ?= debug

LIBFT_DIR = libft
GNL_DIR = gnl
MLX_DIR = minilibx-linux

LIBMLX = $(MLX_DIR)/libmlx.a
LIBGNL = $(GNL_DIR)/libgnl.a
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = \
	debug_print.c \
	scene_utils.c \
	element_parsing.c \
	extract_element.c \
	map_parsing.c \
	map_utils.c \
	game.c \
	game_draw.c \
	game_draw_wall_column.c \
	game_events_main.c \
	game_events_debug.c \
	game_init.c \
	game_init_dda.c \
	game_run_dda.c \
	graphics_draw.c \
	graphics_events.c \
	graphics_image.c \
	graphics_mouse_events.c \
	graphics_pixel_utils.c \
	graphics_window.c \
	vec2_utils.c \
	free.c

HDRS = game.h graphics.h vec2.h libft/libft.h gnl/get_next_line.h

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
CC = cc
RM = rm -f
CPPFLAGS = -I$(MLX_DIR) -I$(LIBFT_DIR) -I$(GNL_DIR)
CFLAGS = -Wall -Wextra
LDFLAGS = -L$(MLX_DIR) -L$(LIBFT_DIR) -L$(GNL_DIR)
LDLIBS = -lmlx -lXext -lX11 -lm -lgnl -lft

ifeq ($(BUILD),release)
	CFLAGS += -Werror
else ifeq ($(BUILD),debug)
	CFLAGS += -g3
else ifeq ($(BUILD),asan)
	CFLAGS += -ggdb3 -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif

all: $(NAME)

$(NAME): $(LIBMLX) $(LIBGNL) $(LIBFT) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(LIBGNL):
	$(MAKE) -C $(GNL_DIR)

$(LIBMLX): $(MLX_DIR)/.git
	$(MAKE) -C $(MLX_DIR)

$(MLX_DIR)/.git:
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
	./$(NAME) maps/valid/map1.cub

map3: all
	./$(NAME) maps/valid/map3.cub

identifier: all
	./$(NAME) maps/map1

.PHONY: all clean fclean re norm
