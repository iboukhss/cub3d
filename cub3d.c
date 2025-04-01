/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/01 13:42:50 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <mlx.h>
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

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
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

// Assuming ARGB32 (little endian) which means BGRA byte order.
// Source: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
static int	put_pixel(t_image *img, int x_pos, int y_pos, int color)
{
	char	*pixel;
	int		bytes_per_line;
	int		bytes_per_pixel;

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
	"11000001110101011111011110N0111  ",
	"11110111 1110101 101111010001    ",
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
	player->x_pos = 26;
	player->y_pos = 11;
	player->width = 10;
	return (0);
}

static int	draw_map(t_image *frame, t_map *map)
{
	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; x < map->width; x++)
		{
			if (map->grid[y][x] == '1')
			{
				draw_rect(frame, x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, 0xFFFFFF);
			}
			else if (map->grid[y][x] == ' ')
			{
				draw_rect(frame, x * CELL_WIDTH, y * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, 0x808080);
			}
			else if (map->grid[y][x] == '0')
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

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	init_map(&game.map);
	init_player(&game.player);
	mlx_hook(game.win.win_ctx, DestroyNotify, 0, mlx_loop_end, game.mlx_ctx);
	mlx_key_hook(game.win.win_ctx, key_release_hook, game.mlx_ctx);
	draw_map(&game.win.frame, &game.map);
	draw_player(&game.win.frame, &game.player);
	mlx_put_image_to_window(game.mlx_ctx, game.win.win_ctx, game.win.frame.img_ctx, 0, 0);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
