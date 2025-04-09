/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 23:17:42 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/09 23:51:49 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>
#include <stdlib.h>

// Default hardcoded map
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
	NULL,
};

static int	init_map(t_map *map)
{
	map->grid = (char **)g_testmap;
	map->width = 33;
	map->height = 14;
	return (0);
}

// NOTE(ismail): Y-axis is inverted compared to the regular unit circle.
// Be mindful when dealing with rotations.
static int	init_camera(t_camera *cam, t_player *player)
{
	cam->angle_deg = player->orientation;
	cam->angle_rad = rad(cam->angle_deg);
	cam->pos = vec2d_init(player->start_x + 0.5f, player->start_y + 0.5f);
	cam->dir = vec2d_init(cosf(cam->angle_rad), -sinf(cam->angle_rad));
	cam->plane = vec2d_init(sinf(cam->angle_rad), cosf(cam->angle_rad));
	return (0);
}

static int	init_player(t_player *player)
{
	player->start_x = 4;
	player->start_y = 12;
	player->orientation = NORTH;
	player->width = 0.5;
	init_camera(&player->cam, player);
	return (0);
}

static int	create_main_window(t_window *win, t_game *game)
{
	win->width = WIN_WIDTH;
	win->height = WIN_HEIGHT;
	win->title = "cub3D";
	win->win_ctx = mlx_new_window(game->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(game->mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx, &win->frame.bits_per_pixel, &win->frame.bytes_per_line, &win->frame.endian);
	return (0);
}

static int	create_debug_window(t_window *win, t_game *game)
{
	win->width = game->map.width * TILE_SIZE;
	win->height = game->map.height * TILE_SIZE;
	win->title = "Map view";
	win->win_ctx = mlx_new_window(game->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(game->mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx, &win->frame.bits_per_pixel, &win->frame.bytes_per_line, &win->frame.endian);
	return (0);
}

int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	init_map(&game->map);
	init_player(&game->player);
	create_main_window(&game->win0, game);
	create_debug_window(&game->win1, game);
	return (0);
}

int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win0.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win0.win_ctx);
	mlx_destroy_image(game->mlx_ctx, game->win1.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win1.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
	return (0);
}
