/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/04 14:20:04 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

#include <math.h>
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

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
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
	player->cx = 4.5;
	player->cy = 12.5;
	player->radius = 0.25;
	player->angle_deg = 90;
	player->angle_rad = player->angle_deg * (M_PI / 180);
	player->dx = -cos(player->angle_rad);
	player->dy = -sin(player->angle_rad);
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

static int	draw_line(t_image *img, t_line *line, int color)
{
	float	dx = line->x1 - line->x0;
	float	dy = line->y1 - line->y0;
	int		steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);

	float	x_inc = dx / steps;
	float	y_inc = dy / steps;

	float	x = line->x0;
	float	y = line->y0;

	for (int i = 0; i <= steps; i++)
	{
		put_pixel(img, roundf(x), roundf(y), color);
		x += x_inc;
		y += y_inc;
	}
	return (0);
}

static int	fill_rect(t_image *img, t_rect *rect, int color)
{
	for (int y = rect->y; y < rect->y + rect->h; y++)
	{
		for (int x = rect->x; x < rect->x + rect->w; x++)
		{
			put_pixel(img, x, y, color);
		}
	}
	return (0);
}

static int	draw_map(t_image *frame, t_map *map)
{
	t_rect	cell;

	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; map->grid[y][x] && x < map->width; x++)
		{
			cell.x = x * CELL_WIDTH;
			cell.y = y * CELL_WIDTH;
			cell.w = CELL_WIDTH;
			cell.h = CELL_WIDTH;
			if (map->grid[y][x] == '1')
			{
				fill_rect(frame, &cell, 0xFFFFFF);
			}
			else if (map->grid[y][x] == ' ')
			{
				fill_rect(frame, &cell, 0x808080);
			}
			else
			{
				fill_rect(frame, &cell, 0x000000);
			}
		}
	}
	return (0);
}

static int	draw_player(t_image *frame, t_player *player)
{
	t_rect	bound_box;
	t_line	dir_vec;

	bound_box.x = (player->cx - player->radius) * CELL_WIDTH;
	bound_box.y = (player->cy - player->radius) * CELL_WIDTH;
	bound_box.w = (player->radius * 2) * CELL_WIDTH;
	bound_box.h = (player->radius * 2) * CELL_WIDTH;
	dir_vec.x0 = player->cx * CELL_WIDTH;
	dir_vec.y0 = player->cy * CELL_WIDTH;
	dir_vec.x1 = dir_vec.x0 + player->dx * (1.5 * CELL_WIDTH);
	dir_vec.y1 = dir_vec.y0 + player->dy * (1.5 * CELL_WIDTH);
	fill_rect(frame, &bound_box, 0xFF00FF);
	draw_line(frame, &dir_vec, 0xFFFF00);
	return (0);
}

static int	redraw_frame(t_game *game)
{
	draw_map(&game->win.frame, &game->map);
	draw_player(&game->win.frame, &game->player);
	mlx_put_image_to_window(game->mlx_ctx, game->win.win_ctx, game->win.frame.img_ctx, 0, 0);
	return (0);
}

static int	key_press_hook(int keysym, void *param)
{
	t_game	*game;
	float	move_speed;

	game = (t_game *)param;
	move_speed = 0.25;
	if (keysym == XK_Up)
	{
		game->player.cx += game->player.dx * move_speed;
		game->player.cy += game->player.dy * move_speed;
	}
	else if (keysym == XK_Down)
	{
		game->player.cx -= game->player.dx * move_speed;
		game->player.cy -= game->player.dy * move_speed;
	}
	else if (keysym == XK_Left)
	{
		game->player.angle_deg = (game->player.angle_deg - 15) % 360;
		game->player.angle_rad = game->player.angle_deg * (M_PI / 180);
		game->player.dx = -cos(game->player.angle_rad);
		game->player.dy = -sin(game->player.angle_rad);
	}
	else if (keysym == XK_Right)
	{
		game->player.angle_deg = (game->player.angle_deg + 15) % 360;
		game->player.angle_rad = game->player.angle_deg * (M_PI / 180);
		game->player.dx = -cos(game->player.angle_rad);
		game->player.dy = -sin(game->player.angle_rad);
	}
	printf("X  : %f\n", game->player.cx);
	printf("Y  : %f\n", game->player.cy);
	printf("Dx : %f\n", game->player.dx);
	printf("Dy : %f\n", game->player.dy);
	printf("Deg: %d\n", game->player.angle_deg);
	printf("Rad: %f\n", game->player.angle_rad);
	printf("------------------\n");
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
