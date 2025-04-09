/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/09 23:56:08 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

static int	draw_map(t_image *frame, t_map *map)
{
	t_rect	tile;

	for (int y = 0; map->grid[y] != NULL && y < map->height; y++)
	{
		for (int x = 0; map->grid[y][x] != '\0' && x < map->width; x++)
		{
			tile.x = x * TILE_SIZE;
			tile.y = y * TILE_SIZE;
			tile.w = TILE_SIZE;
			tile.h = TILE_SIZE;
			if (map->grid[y][x] == '1')
			{
				fill_rect(frame, &tile, 0xFFFFFF);
			}
			else if (map->grid[y][x] == ' ')
			{
				fill_rect(frame, &tile, 0x808080);
			}
			else
			{
				fill_rect(frame, &tile, 0x000000);
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
	fill_rect(frame, &hitbox, 0xFF0000);
	return (0);
}

// Returns the distance travelled by the ray
static float	cast_ray(t_ray2d ray, t_map *map)
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
	float	ray_length;

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
		if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height || map->grid[map_y][map_x] == '1')
		{
			break ;
		}
	}
	if (side == 0)
	{
		ray_length = side_dist_x - delta_dist_x;
	}
	else
	{
		ray_length = side_dist_y - delta_dist_y;
	}
	return (ray_length);
}

int	render_scene(t_game *game)
{
	float	camera_x;
	t_ray2d	ray;
	float	ray_length;
	t_line	line;

	draw_map(&game->win1.frame, &game->map);
	draw_player(&game->win1.frame, &game->player);
	// This is where we cast a ray for each screen column
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos.x = game->player.cam.pos.x;
		ray.pos.y = game->player.cam.pos.y;
		ray.dir.x = game->player.cam.dir.x + game->player.cam.plane.x * camera_x;
		ray.dir.y = game->player.cam.dir.y + game->player.cam.plane.y * camera_x;
		ray_length = cast_ray(ray, &game->map);
		line = ray2d_to_line(ray, ray_length, TILE_SIZE);
		draw_line(&game->win1.frame, &line, 0xFFFF00);
	}
	mlx_put_image_to_window(game->mlx_ctx, game->win1.win_ctx, game->win1.frame.img_ctx, 0, 0);
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
