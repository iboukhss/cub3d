/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/02 01:45:51 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>

static int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	game->win.width = WIN_WIDTH;
	game->win.height = WIN_HEIGHT;
	game->win.title = "cub3D";
	game->win.win_ctx = mlx_new_window(game->mlx_ctx, game->win.width, game->win.height, game->win.title);
	game->win.frame.width = game->win.width;
	game->win.frame.height = game->win.height;
	game->win.frame.img_ctx = mlx_new_image(game->mlx_ctx, game->win.frame.width, game->win.frame.height);
	game->win.frame.addr = mlx_get_data_addr(game->win.frame.img_ctx, &game->win.frame.bits_per_pixel, &game->win.frame.bytes_per_line, &game->win.frame.endian);
	return (0);
}

// Hardcoded stubs
static const char	*g_testmap[] = {
	"        1111111111111111111111111",
	"        1000000000110000000000001",
	"        1011000001110000000000001",
	"        1001000000000000000000001",
	"111111111011000001110000000000001",
	"100000000011000001110111111111111",
	"11110111111111011100000010001    ",
	"11110111111111011101010010001    ",
	"11000000110101011100000010001    ",
	"10000000000000001100000010001    ",
	"10000000000000001101010010001    ",
	"1100000111010101111101111000111  ",
	"1111N111 1110101 101111010001    ",
	"11111111 1111111 111111111111    ",
};

static int	init_map(t_map *map)
{
	map->grid = (char **)g_testmap;
	map->width = 33;
	map->height = 14;
	map->cell_width = CELL_WIDTH;
	map->stroke_width = 2;
	return (0);
}

static int	init_player(t_player *player)
{
	player->x_pos = 4 + 0.25;
	player->y_pos = 12 + 0.25;
	player->width = CELL_WIDTH / 2;
	return (0);
}

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
	return (0);
}

// Assuming ARGB32 (little endian) which means BGRA byte order.
// Source: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
static int	put_pixel(t_image *img, int x_pos, int y_pos, int color)
{
	char	*pixel;
	int		bytes_per_line;
	int		bytes_per_pixel;

	if (x_pos < 0 || x_pos >= img->width || y_pos < 0 || y_pos >= img->height)
	{
		return (-1);
	}
	bytes_per_line = img->bytes_per_line;
	bytes_per_pixel = img->bits_per_pixel / 8;
	pixel = img->addr + y_pos * bytes_per_line + x_pos * bytes_per_pixel;
	if (bytes_per_pixel == sizeof(unsigned int))
	{
		*(unsigned int *)pixel = color;
	}
	return (0);
}

static int	draw_rect(t_image *img, int x_pos, int y_pos, int width, int height, int color)
{
	for (int y = y_pos; y < y_pos + height; y++)
	{
		for (int x = x_pos; x < x_pos + width; x++)
		{
			put_pixel(img, x, y, color);
		}
	}
	return (0);
}
static int	draw_map(t_image *frame, t_map *map)
{
	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; map->grid[y][x] && x < map->width; x++)
		{
			if (map->grid[y][x] == '1')
			{
				draw_rect(frame, x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, 0xFFFFFF);
			}
			else if (map->grid[y][x] == ' ')
			{
				draw_rect(frame, x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, 0x808080);
			}
			else
			{
				draw_rect(frame, x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, 0x000000);
			}
		}
	}
	return (0);
}

static int	draw_player(t_image *frame, t_player *player)
{
	draw_rect(frame, player->x_pos * CELL_WIDTH, player->y_pos * CELL_WIDTH, player->width, player->width, 0xFF0000);
	return (0);
}

static int	redraw_frame(t_game *game)
{
	draw_map(&game->win.frame, &game->map);
	draw_player(&game->win.frame, &game->player);
	mlx_put_image_to_window(game->mlx_ctx, game->win.win_ctx, game->win.frame.img_ctx, 0, 0);
	return (0);
}

static bool	is_arrow_key(int keysym)
{
	return (keysym == XK_Up || keysym == XK_Down || keysym == XK_Left || keysym == XK_Right);
}

static bool	is_wasd_key(int keysym)
{
	return (keysym == XK_w || keysym == XK_a || keysym == XK_s || keysym == XK_d);
}

static bool	is_wall(t_map *map, float x, float y)
{
	return (map->grid[(int)y][(int)x] == '1');
}

// This is so hacky
static int	key_press_hook(int keysym, void *param)
{
	t_game	*game;
	float	move_speed;
	float	epsilon;
	float	new_x;
	float	new_y;
	float	box[4][2];

	game = (t_game *)param;
	move_speed = 0.25;
	epsilon = 0.001f;
	new_x = game->player.x_pos;
	new_y = game->player.y_pos;
	if (keysym == XK_Up || keysym == XK_w)
	{
		new_y -= move_speed;
	}
	else if (keysym == XK_Down || keysym == XK_s)
	{
		new_y += move_speed;
	}
	else if (keysym == XK_Left || keysym == XK_a)
	{
		new_x -= move_speed;
	}
	else if (keysym == XK_Right || keysym == XK_d)
	{
		new_x += move_speed;
	}
	else
	{
		return (-1);
	}
	box[0][0] = new_x;
	box[0][1] = new_y;
	box[1][0] = new_x + 0.5f - epsilon;
	box[1][1] = new_y;
	box[2][0] = new_x + 0.5f - epsilon;
	box[2][1] = new_y + 0.5f - epsilon;
	box[3][0] = new_x;
	box[3][1] = new_y + 0.5f - epsilon;
	printf("0: x: %f y: %f\n", box[0][0], box[0][1]);
	printf("1: x: %f y: %f\n", box[1][0], box[1][1]);
	printf("2: x: %f y: %f\n", box[2][0], box[2][1]);
	printf("3: x: %f y: %f\n", box[3][0], box[3][1]);
	for (int i = 0; i < 4; i++)
	{
		if (is_wall(&game->map, box[i][0], box[i][1]))
		{
			return (-1);
		}
	}
	game->player.x_pos = new_x;
	game->player.y_pos = new_y;
	redraw_frame(game);
	return (0);
}

static int	key_release_hook(int keysym, void *mlx_ctx)
{
	if (keysym == XK_Escape)
	{
		mlx_loop_end(mlx_ctx);
	}
	return (0);
}

static int	expose_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	redraw_frame(game);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	init_map(&game.map);
	init_player(&game.player);
	mlx_hook(game.win.win_ctx, DestroyNotify, 0, mlx_loop_end, game.mlx_ctx);
	mlx_hook(game.win.win_ctx, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_key_hook(game.win.win_ctx, key_release_hook, game.mlx_ctx);
	mlx_expose_hook(game.win.win_ctx, expose_hook, &game);
	redraw_frame(&game);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
