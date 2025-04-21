/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/18 17:11:08 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

// Draws the tile map
static int	draw_map(t_image *frame, t_map map)
{
	t_rect	tile;

	for (int y = 0; y < map.height; y++)
	{
		for (int x = 0; x < map.width; x++)
		{
			tile.x = x * TILE_SIZE;
			tile.y = y * TILE_SIZE;
			tile.w = TILE_SIZE;
			tile.h = TILE_SIZE;
			if (map.grid[y][x] == '1')
			{
				fill_rect(frame, tile, 0xFFFFFF);
			}
			else if (map.grid[y][x] == '0')
			{
				fill_rect(frame, tile, 0x000000);
			}
			else
			{
				fill_rect(frame, tile, 0x808080);
			}
		}
	}
	return (0);
}

// Draws the player's position
static int	draw_player(t_image *frame, t_player player)
{
	t_rect	hitbox;

	hitbox.x = (player.cam.pos.x - (player.width / 2)) * TILE_SIZE;
	hitbox.y = (player.cam.pos.y - (player.width / 2)) * TILE_SIZE;
	hitbox.w = player.width * TILE_SIZE;
	hitbox.h = player.width * TILE_SIZE;
	fill_rect(frame, hitbox, 0xFF0000);
	return (0);
}

static int	draw_floor_and_ceiling(t_image *frame, t_config conf)
{
	t_rect	floor;
	t_rect	ceil;

	ceil.x = 0;
	ceil.y = 0;
	ceil.w = frame->width;
	ceil.h = frame->height / 2;
	floor.x = 0;
	floor.y = ceil.h;
	floor.w = frame->width;
	floor.h = frame->height - ceil.h;
	fill_rect(frame, ceil, conf.ceil_color);
	fill_rect(frame, floor, conf.floor_color);
	return (0);
}

// See: https://lodev.org/cgtutor/raycasting.html#Untextured_Raycaster_
static int	cast_ray(float *perp_wall_dist, t_ray2d ray, t_map map)
{
	int		map_x = (int)ray.pos.x;
	int		map_y = (int)ray.pos.y;

	float	delta_dist_x = (ray.dir.x == 0) ? 1e30f : fabsf(1.0f / ray.dir.x);
	float	delta_dist_y = (ray.dir.y == 0) ? 1e30f : fabsf(1.0f / ray.dir.y);

	float	side_dist_x;
	float	side_dist_y;

	int		step_x;
	int		step_y;

	if (ray.dir.x < 0)
	{
		step_x = -1;
		side_dist_x = (ray.pos.x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0f - ray.pos.x) * delta_dist_x;
	}
	if (ray.dir.y < 0)
	{
		step_y = -1;
		side_dist_y = (ray.pos.y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0f - ray.pos.y) * delta_dist_y;
	}

	enum e_orientation side = -1;

	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = (step_x > 0) ? EAST : WEST;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = (step_y > 0) ? SOUTH : NORTH;
		}
		if (map_x < 0 || map_x >= map.width || map_y < 0 || map_y >= map.height || map.grid[map_y][map_x] == '1')
		{
			break ;
		}
	}
	if (side == EAST || side == WEST)
	{
		*perp_wall_dist = side_dist_x - delta_dist_x;
	}
	else if (side == NORTH || side == SOUTH)
	{
		*perp_wall_dist = side_dist_y - delta_dist_y;
	}
	return (side);
}

int	draw_line_vertical(t_image *img, t_line line, uint32_t color)
{
	for (int y = line.y0; y <= line.y1; y++)
	{
		put_pixel(img, line.x0, y, color);
	}
	return (0);
}

static int	do_raycasting(t_game *game)
{
	t_camera	cam;
	t_config	conf;
	float		camera_x;
	t_ray2d		ray;
	t_line		line;
	t_line		vert_line;

	cam = game->player.cam;
	conf = game->config;
	for (int x = 0; x < WIN_WIDTH; x += 1)
	{
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos.x = cam.pos.x;
		ray.pos.y = cam.pos.y;
		ray.dir.x = cam.dir.x + cam.plane.x * camera_x;
		ray.dir.y = cam.dir.y + cam.plane.y * camera_x;
		float perp_wall_dist = 0;
		enum e_orientation side = 0;
		side = cast_ray(&perp_wall_dist, ray, game->map);
		// Trace ray on the minimap
		line = ray2d_to_line(ray, perp_wall_dist, TILE_SIZE);
		draw_line(&game->win1.frame, line, 0xFFFF00);
		// Very convoluted logic to shade walls
		int column_height = (int)(WIN_HEIGHT / perp_wall_dist);
		int draw_start = -column_height / 2 + WIN_HEIGHT / 2;
		int draw_end = column_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		if (draw_end >= WIN_HEIGHT) draw_end = WIN_HEIGHT - 1;
		vert_line.x0 = x;
		vert_line.y0 = draw_start;
		vert_line.x1 = x;
		vert_line.y1 = draw_end;
		if (side == NORTH)
			draw_line_vertical(&game->win0.frame, vert_line, conf.north_color);
		else if (side == SOUTH)
			draw_line_vertical(&game->win0.frame, vert_line, conf.south_color);
		else if (side == EAST)
			draw_line_vertical(&game->win0.frame, vert_line, conf.east_color);
		else if (side == WEST)
			draw_line_vertical(&game->win0.frame, vert_line, conf.west_color);
	}
	return (0);
}

static int	refresh_frame(t_window *win, void *mlx_ctx)
{
	if (!win->win_ctx)
	{
		return (1);
	}
	mlx_put_image_to_window(mlx_ctx, win->win_ctx, win->frame.img_ctx, 0, 0);
	return (0);
}

// NOTE(ismail): Work in progress. Heavy refactoring incoming.
int	render_scene(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	// Draw background
	draw_map(&game->win1.frame, game->map);
	draw_player(&game->win1.frame, game->player);
	draw_floor_and_ceiling(&game->win0.frame, game->config);
	// Draw foreground
	do_raycasting(game);
	// Refresh window frames
	refresh_frame(&game->win0, game->mlx_ctx);
	refresh_frame(&game->win1, game->mlx_ctx);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	if (argc != 2)
	{
		print_error(1, "Incorrect number of parameters");
		return (1);
	}
	if (get_scene(&game, argv[1]) != 0)
		return(1);
	init_game(&game);
	create_window(&game.win0, game.mlx_ctx);
	create_window(&game.win1, game.mlx_ctx);	// this is the minimap
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
