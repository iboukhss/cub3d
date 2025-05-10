/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/10 17:37:51 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

static int	draw_tile(t_image *frame, int pos_x, int pos_y, uint32_t color)
{
	t_rect	tile;

	tile.x = pos_x * TILE_SIZE;
	tile.y = pos_y * TILE_SIZE;
	tile.w = TILE_SIZE;
	tile.h = TILE_SIZE;
	fill_rect(frame, tile, color);
	return (0);
}

// Draws the tile map
static int	draw_map(t_image *frame, t_map map)
{
	for (int y = 0; y < map.height; y++)
	{
		for (int x = 0; x < map.width; x++)
		{
			if (map.grid[y][x] == '1')
			{
				draw_tile(frame, x, y, COLOR_WHITE);
			}
			else if (map.grid[y][x] == '0')
			{
				draw_tile(frame, x, y, COLOR_BLACK);
			}
			else
			{
				draw_tile(frame, x, y, COLOR_GRAY);
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
	fill_rect(frame, hitbox, COLOR_RED);
	return (0);
}

static int	draw_floor_and_ceiling(t_image *frame, const t_config *conf)
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
	fill_rect(frame, ceil, conf->ceil_color);
	fill_rect(frame, floor, conf->floor_color);
	return (0);
}

// See: https://lodev.org/cgtutor/raycasting.html#Untextured_Raycaster_
static void	cast_ray(t_ray *ray, t_map map)
{
	int		map_x = (int)ray->pos.x;
	int		map_y = (int)ray->pos.y;

	float	delta_dist_x = (ray->dir.x == 0) ? INFINITY : fabsf(1.0f / ray->dir.x);
	float	delta_dist_y = (ray->dir.y == 0) ? INFINITY : fabsf(1.0f / ray->dir.y);

	float	side_dist_x;
	float	side_dist_y;

	int		step_x;
	int		step_y;

	if (ray->dir.x < 0)
	{
		step_x = -1;
		side_dist_x = (ray->pos.x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0f - ray->pos.x) * delta_dist_x;
	}
	if (ray->dir.y < 0)
	{
		step_y = -1;
		side_dist_y = (ray->pos.y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0f - ray->pos.y) * delta_dist_y;
	}

	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			ray->side = (step_x > 0) ? ORIENT_EAST : ORIENT_WEST;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			ray->side = (step_y > 0) ? ORIENT_SOUTH : ORIENT_NORTH;
		}
		if (map_x < 0 || map_x >= map.width || map_y < 0 || map_y >= map.height || map.grid[map_y][map_x] == '1')
		{
			break ;
		}
	}
	if (ray->side == ORIENT_EAST || ray->side == ORIENT_WEST)
	{
		ray->len = side_dist_x - delta_dist_x;
	}
	else if (ray->side == ORIENT_NORTH || ray->side == ORIENT_SOUTH)
	{
		ray->len = side_dist_y - delta_dist_y;
	}
}

static int	draw_ray_on_minimap(t_game *game, t_ray ray)
{
	t_vec2d	p1;
	t_vec2d	p2;
	t_line	line;

	p1 = ray.pos;
	p2 = vec2d_add(ray.pos, vec2d_mul(ray.dir, ray.len));
	p1 = vec2d_mul(p1, TILE_SIZE);
	p2 = vec2d_mul(p2, TILE_SIZE);
	line.x0 = (int)p1.x;
	line.y0 = (int)p1.y;
	line.x1 = (int)p2.x;
	line.y1 = (int)p2.y;
	draw_line(&game->debug.frame, line, 0xFFFF00);
	return (0);
}

static int	draw_wall_column(t_game *game, int col_x, t_ray ray)
{
	int			column_height;
	int			draw_start;
	int			draw_end;

	column_height = (int)(WIN_HEIGHT / ray.len);
	draw_start = -column_height / 2 + WIN_HEIGHT / 2;
	draw_end = column_height / 2 + WIN_HEIGHT / 2;
	if (draw_start < 0)
	{
		draw_start = 0;
	}
	if (draw_end >= WIN_HEIGHT)
	{
		draw_end = WIN_HEIGHT - 1;
	}

	float	wall_x;
	int		tex_x;
	int		tex_width;
	int		tex_height;

	if (ray.side == ORIENT_EAST || ray.side == ORIENT_WEST)
	{
		wall_x = ray.pos.y + ray.len * ray.dir.y;
	}
	else
	{
		wall_x = ray.pos.x + ray.len * ray.dir.x;
	}
	wall_x -= floorf(wall_x);
	tex_width = game->cfg.texture_EA.width;
	tex_height = game->cfg.texture_EA.height;
	tex_x = (int)(wall_x * tex_width);
	float step = 1.0f * tex_height / column_height;
	float tex_pos = (draw_start - WIN_HEIGHT / 2.0f + column_height / 2.0f) * step;
	for (int y = draw_start; y < draw_end; y++)
	{
		int tex_y = (int)tex_pos;
		if (tex_y < 0) tex_y = 0;
		else if (tex_y >= tex_height) tex_y = tex_height - 1;
		t_image *tex_img;
		if (ray.side == ORIENT_EAST)
		{
			tex_img = &game->cfg.texture_EA;
		}
		else if (ray.side == ORIENT_WEST)
		{
			tex_img = &game->cfg.texture_WE;
		}
		else if (ray.side == ORIENT_NORTH)
		{
			tex_img = &game->cfg.texture_NO;
		}
		else if (ray.side == ORIENT_SOUTH)
		{
			tex_img = &game->cfg.texture_SO;
		}
		else
		{
			tex_img = NULL;
		}
		uint32_t color = get_pixel(tex_img, tex_x, tex_y);
		put_pixel(&game->main.frame, col_x, y, color);
		tex_pos += step;
	}
	return (0);
}

static int	do_raycasting(t_game *game)
{
	t_camera	cam;
	float		camera_x;
	t_ray		ray;

	cam = game->player.cam;
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos = cam.pos;
		ray.dir = vec2d_add(cam.dir, vec2d_mul(cam.plane, camera_x));
		ray.side = ORIENT_UNKNOWN;
		cast_ray(&ray, game->map);
		draw_ray_on_minimap(game, ray);
		draw_wall_column(game, x, ray);
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

int	render_scene(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	// Draw background
	draw_map(&game->debug.frame, game->map);
	draw_player(&game->debug.frame, game->player);
	draw_floor_and_ceiling(&game->main.frame, &game->cfg);
	// Draw foreground
	do_raycasting(game);
	// Refresh window frames
	//draw_image(&game->main, &game->cfg.texture_EA, 0, 0, game->mlx_ctx);
	refresh_frame(&game->main, game->mlx_ctx);
	refresh_frame(&game->debug, game->mlx_ctx);
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
	print_error(0, "map ok\n");
	init_game(&game);
	create_window(&game.main);
	create_window(&game.debug);	// this is the minimap
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
