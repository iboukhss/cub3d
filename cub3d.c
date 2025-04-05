/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/05 16:35:19 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double	rad(double deg)
{
	return (deg * (M_PI / 180.0));
}

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
	return (0);
}

static int	init_player(t_player *player)
{
	player->cx = 4.5;
	player->cy = 12.5;
	player->angle_deg = 90;
	player->fov_deg = 60;
	player->radius = 0.25;
	player->angle_rad = rad(player->angle_deg);
	player->dx = cos(player->angle_rad);
	player->dy = -sin(player->angle_rad);
	player->ldx = cos(rad(player->angle_deg + player->fov_deg / 2.0));
	player->ldy = -sin(rad(player->angle_deg + player->fov_deg / 2.0));
	player->rdx = cos(rad(player->angle_deg - player->fov_deg / 2.0));
	player->rdy = -sin(rad(player->angle_deg - player->fov_deg / 2.0));
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
	win->width = game->map.width * CELL_WIDTH;
	win->height = game->map.height * CELL_WIDTH;
	win->title = "Map view";
	win->win_ctx = mlx_new_window(game->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(game->mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx, &win->frame.bits_per_pixel, &win->frame.bytes_per_line, &win->frame.endian);
	return (0);
}

static int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	init_map(&game->map);
	init_player(&game->player);
	create_main_window(&game->win0, game);
	create_debug_window(&game->win1, game);
	return (0);
}

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win0.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win0.win_ctx);
	mlx_destroy_image(game->mlx_ctx, game->win1.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win1.win_ctx);
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

// Using DDA to draw lines.
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

// Draws the player's square position and 3 direction vectors
// representing the field of view.
static int	draw_player(t_image *frame, t_player *player)
{
	t_rect	bound_box;
	t_line	mid_vec;
	t_line	min_vec;
	t_line	max_vec;

	int		center_x = player->cx * CELL_WIDTH;
	int		center_y = player->cy * CELL_WIDTH;

	bound_box.x = (player->cx - player->radius) * CELL_WIDTH;
	bound_box.y = (player->cy - player->radius) * CELL_WIDTH;
	bound_box.w = (player->radius * 2) * CELL_WIDTH;
	bound_box.h = (player->radius * 2) * CELL_WIDTH;
	mid_vec.x0 = center_x;
	mid_vec.y0 = center_y;
	mid_vec.x1 = center_x + player->dx * (4 * CELL_WIDTH);
	mid_vec.y1 = center_y + player->dy * (4 * CELL_WIDTH);
	min_vec.x0 = center_x;
	min_vec.y0 = center_y;
	min_vec.x1 = center_x + player->ldx * (4 * CELL_WIDTH);
	min_vec.y1 = center_y + player->ldy * (4 * CELL_WIDTH);
	max_vec.x0 = center_x;
	max_vec.y0 = center_y;
	max_vec.x1 = center_x + player->rdx * (4 * CELL_WIDTH);
	max_vec.y1 = center_y + player->rdy * (4 * CELL_WIDTH);
	fill_rect(frame, &bound_box, 0xFF00FF);
	draw_line(frame, &mid_vec, 0xFFFF00);
	draw_line(frame, &min_vec, 0x00FFFF);
	draw_line(frame, &max_vec, 0x00FFFF);
	return (0);
}

static int	render_debug_window(t_window *win, t_game *game)
{
	draw_map(&win->frame, &game->map);
	draw_player(&win->frame, &game->player);
	mlx_put_image_to_window(game->mlx_ctx, win->win_ctx, win->frame.img_ctx, 0, 0);
	return (0);
}

static int	rotate_player(t_player *player, int deg)
{
	player->angle_deg = (player->angle_deg + deg) % 360;
	player->angle_rad = rad(player->angle_deg);
	player->dx = cos(player->angle_rad);
	player->dy = -sin(player->angle_rad);
	player->ldx = cos(rad(player->angle_deg + player->fov_deg / 2.0));
	player->ldy = -sin(rad(player->angle_deg + player->fov_deg / 2.0));
	player->rdx = cos(rad(player->angle_deg - player->fov_deg / 2.0));
	player->rdy = -sin(rad(player->angle_deg - player->fov_deg / 2.0));
	return (0);
}

// NOTE(ismail): Very messy right now. Needs better refactoring.
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
		rotate_player(&game->player, 15);
	}
	else if (keysym == XK_Right)
	{
		rotate_player(&game->player, -15);
	}
	printf("X  : %f\n", game->player.cx);
	printf("Y  : %f\n", game->player.cy);
	printf("Dx : %f\n", game->player.dx);
	printf("Dy : %f\n", game->player.dy);
	printf("Deg: %d\n", game->player.angle_deg);
	printf("Rad: %f\n", game->player.angle_rad);
	printf("------------------\n");
	render_debug_window(&game->win1, game);
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
	render_debug_window(&game->win1, game);
	return (0);
}

static int	init_hooks(t_game *game)
{
	mlx_hook(game->win1.win_ctx, KeyPress, KeyPressMask, key_press_hook, game);
	mlx_key_hook(game->win1.win_ctx, key_release_hook, game->mlx_ctx);
	mlx_hook(game->win1.win_ctx, DestroyNotify, 0, mlx_loop_end, game->mlx_ctx);
	mlx_expose_hook(game->win1.win_ctx, expose_hook, game);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	init_hooks(&game);
	render_debug_window(&game.win1, &game);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
