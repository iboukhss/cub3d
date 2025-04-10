/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/10 14:00:02 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

static int	draw_map(t_image *frame, t_map map)
{
	t_rect	tile;

	for (int y = 0; map.grid[y] != NULL && y < map.height; y++)
	{
		for (int x = 0; map.grid[y][x] != '\0' && x < map.width; x++)
		{
			tile.x = x * TILE_SIZE;
			tile.y = y * TILE_SIZE;
			tile.w = TILE_SIZE;
			tile.h = TILE_SIZE;
			if (map.grid[y][x] == '1')
			{
				fill_rect(frame, tile, 0xFFFFFF);
			}
			else if (map.grid[y][x] == ' ')
			{
				fill_rect(frame, tile, 0x808080);
			}
			else
			{
				fill_rect(frame, tile, 0x000000);
			}
		}
	}
	return (0);
}

static t_rect	player_to_rect(t_player *player, float raster_factor)
{
	t_rect	rect;

	rect.x = (player->cam.pos.x - player->width / 2) * raster_factor;
	rect.y = (player->cam.pos.y - player->width / 2) * raster_factor;;
	rect.w = player->width * raster_factor;
	rect.h = player->width * raster_factor;
	return (rect);
}

// Draws the player's hitbox
static int	draw_player(t_image *frame, t_player *player)
{
	t_rect	hitbox;

	hitbox = player_to_rect(player, TILE_SIZE);
	fill_rect(frame, hitbox, 0xFF0000);
	return (0);
}

// Returns the perpendicular distance from the ray intersection to the camera
// plane
// Negative distance means Y side ray hit and positive means X side ray hit to
// help with shading.
static float	cast_ray(t_ray2d ray, t_map map)
{
	int		map_x = (int)ray.pos.x;
	int		map_y = (int)ray.pos.y;

	float	delta_dist_x = (ray.dir.x == 0) ? 1e30f : fabsf(1.0f / ray.dir.x);
	float	delta_dist_y = (ray.dir.y == 0) ? 1e30f : fabsf(1.0f / ray.dir.y);

	float	side_dist_x;
	float	side_dist_y;

	int		step_x;
	int		step_y;

	int		side;
	float	perp_wall_dist;

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
	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (map_x < 0 || map_x >= map.width || map_y < 0 || map_y >= map.height || map.grid[map_y][map_x] == '1')
		{
			break ;
		}
	}
	if (side == 0)
	{
		perp_wall_dist = side_dist_x - delta_dist_x;
	}
	else
	{
		perp_wall_dist = -(side_dist_y - delta_dist_y);
	}
	return (perp_wall_dist);
}

int	draw_line_vertical(t_image *img, t_line line, uint32_t color)
{
	for (int y = line.y0; y <= line.y1; y++)
	{
		put_pixel(img, line.x0, y, color);
	}
	return (0);
}

static int	draw_floor_and_ceiling(t_image *img)
{
	t_rect	floor;
	t_rect	ceil;

	ceil.x = 0;
	ceil.y = 0;
	ceil.w = img->width;
	ceil.h = img->height / 2;
	floor.x = 0;
	floor.y = ceil.h;
	floor.w = img->width;
	floor.h = img->height - ceil.h;
	fill_rect(img, ceil, 0xA9A9A9 * 2);
	fill_rect(img, floor, 0xA9A9A9);
	return (0);
}

// NOTE(ismail): Work in progress. Heavy refactoring incoming.
int	render_scene(t_game *game)
{
	float	camera_x;
	t_ray2d	ray;
	float	perp_wall_dist;
	t_line	line;
	t_line	vert_line;

	draw_map(&game->win1.frame, game->map);
	draw_player(&game->win1.frame, &game->player);
	draw_floor_and_ceiling(&game->win0.frame);
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		// I think we could control the FOV here
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos.x = game->player.cam.pos.x;
		ray.pos.y = game->player.cam.pos.y;
		ray.dir.x = game->player.cam.dir.x + game->player.cam.plane.x * camera_x;
		ray.dir.y = game->player.cam.dir.y + game->player.cam.plane.y * camera_x;
		perp_wall_dist = cast_ray(ray, game->map);
		// Trace ray on the minimap
		line = ray2d_to_line(ray, fabsf(perp_wall_dist), TILE_SIZE);
		draw_line(&game->win1.frame, line, 0xFFFF00);
		// Very convoluted logic to shade walls
		int column_height = (int)(WIN_HEIGHT / fabsf(perp_wall_dist));
		int draw_start = -column_height / 2 + WIN_HEIGHT / 2;
		int draw_end = column_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		if (draw_end >= WIN_HEIGHT) draw_end = WIN_HEIGHT - 1;
		vert_line.x0 = x;
		vert_line.y0 = draw_start;
		vert_line.x1 = x;
		vert_line.y1 = draw_end;
		if (perp_wall_dist > 0)
		{
			draw_line_vertical(&game->win0.frame, vert_line, 0x0000FF);
		}
		else
		{
			draw_line_vertical(&game->win0.frame, vert_line, 0x0000FF / 2);
		}
	}
	mlx_put_image_to_window(game->mlx_ctx, game->win1.win_ctx, game->win1.frame.img_ctx, 0, 0);
	mlx_put_image_to_window(game->mlx_ctx, game->win0.win_ctx, game->win0.frame.img_ctx, 0, 0);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	init_hooks(&game);
	render_scene(&game);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
