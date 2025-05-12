/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 14:35:27 by iboukhss         ###   ########.fr       */
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

static float	get_delta_dist_x(t_ray ray)
{
	if (ray.dir.x == 0)
	{
		return (INFINITY);
	}
	else
	{
		return (fabsf(1.0f / ray.dir.x));
	}
}

static float	get_delta_dist_y(t_ray ray)
{
	if (ray.dir.y == 0)
	{
		return (INFINITY);
	}
	else
	{
		return (fabsf(1.0f / ray.dir.y));
	}
}

// See: https://lodev.org/cgtutor/raycasting.html#Untextured_Raycaster_
static int	init_dda(struct s_dda_ctx *d, t_ray ray)
{
	d->map_x = (int)ray.pos.x;
	d->map_y = (int)ray.pos.y;
	d->delta_dist_x = get_delta_dist_x(ray);
	d->delta_dist_y = get_delta_dist_y(ray);
	if (ray.dir.x < 0)
	{
		d->step_x = -1;
		d->side_dist_x = (ray.pos.x - d->map_x) * d->delta_dist_x;
	}
	else
	{
		d->step_x = 1;
		d->side_dist_x = (d->map_x + 1.0f - ray.pos.x) * d->delta_dist_x;
	}
	if (ray.dir.y < 0)
	{
		d->step_y = -1;
		d->side_dist_y = (ray.pos.y - d->map_y) * d->delta_dist_y;
	}
	else
	{
		d->step_y = 1;
		d->side_dist_y = (d->map_y + 1.0f - ray.pos.y) * d->delta_dist_y;
	}
	return (0);
}

static bool	dda_done(struct s_dda_ctx d, t_map map)
{
	return (d.map_x < 0 || d.map_y >= map.width
		|| d.map_y < 0 || d.map_y >= map.height
		|| map.grid[d.map_y][d.map_x] == '1');
}

static t_orientation	get_ray_orient_x(int step_x)
{
	if (step_x > 0)
	{
		return (ORIENT_EAST);
	}
	else
	{
		return (ORIENT_WEST);
	}
}

static t_orientation	get_ray_orient_y(int step_y)
{
	if (step_y > 0)
	{
		return (ORIENT_SOUTH);
	}
	else
	{
		return (ORIENT_NORTH);
	}
}

static int	run_dda(t_ray *ray, struct s_dda_ctx d, t_map map)
{
	while (1)
	{
		if (d.side_dist_x < d.side_dist_y)
		{
			d.side_dist_x += d.delta_dist_x;
			d.map_x += d.step_x;
			ray->side = get_ray_orient_x(d.step_x);
		}
		else
		{
			d.side_dist_y += d.delta_dist_y;
			d.map_y += d.step_y;
			ray->side = get_ray_orient_y(d.step_y);
		}
		if (dda_done(d, map))
		{
			break ;
		}
	}
	if (ray->side == ORIENT_EAST || ray->side == ORIENT_WEST)
		ray->len = d.side_dist_x - d.delta_dist_x;
	else if (ray->side == ORIENT_NORTH || ray->side == ORIENT_SOUTH)
		ray->len = d.side_dist_y - d.delta_dist_y;
	return (0);
}

static void	cast_ray(t_ray *ray, t_map map)
{
	struct s_dda_ctx	dda_ctx;

	init_dda(&dda_ctx, *ray);
	run_dda(ray, dda_ctx, map);
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

static int	init_slice(struct s_col_ctx *c, t_ray ray)
{
	c->column_height = (int)(WIN_HEIGHT / ray.len);
	c->draw_start = -c->column_height / 2 + WIN_HEIGHT / 2;
	c->draw_end = c->column_height / 2 + WIN_HEIGHT / 2;
	if (c->draw_start < 0)
	{
		c->draw_start = 0;
	}
	if (c->draw_end >= WIN_HEIGHT)
	{
		c->draw_end = WIN_HEIGHT - 1;
	}
	if (ray.side == ORIENT_EAST || ray.side == ORIENT_WEST)
	{
		c->wall_x = ray.pos.y + ray.len * ray.dir.y;
	}
	else
	{
		c->wall_x = ray.pos.x + ray.len * ray.dir.x;
	}
	c->wall_x -= floorf(c->wall_x);
	c->tex_x = (int)(c->wall_x * c->tex->width);
	c->step = 1.0f * c->tex->height / c->column_height;
	c->tex_pos = (c->draw_start - WIN_HEIGHT / 2.0f + c->column_height / 2.0f) * c->step;
	return (0);
}

static int	draw_slice(t_image *img, int col_x, struct s_col_ctx c)
{
	int			tex_y;
	uint32_t	color;

	for (int y = c.draw_start; y < c.draw_end; y++)
	{
		tex_y = (int)c.tex_pos;
		if (tex_y < 0)
		{
			tex_y = 0;
		}
		else if (tex_y >= c.tex->height)
		{
			tex_y = c.tex->height - 1;
		}
		color = get_pixel(c.tex, c.tex_x, tex_y);
		put_pixel(img, col_x, y, color);
		c.tex_pos += c.step;
	}
	return (0);
}

static int	draw_wall_column(t_game *game, int col_x, t_ray ray)
{
	struct s_col_ctx	col_ctx;

	if (ray.side == ORIENT_EAST)
	{
		col_ctx.tex = &game->cfg.east_texture;
	}
	else if (ray.side == ORIENT_WEST)
	{
		col_ctx.tex = &game->cfg.west_texture;
	}
	else if (ray.side == ORIENT_NORTH)
	{
		col_ctx.tex = &game->cfg.north_texture;
	}
	else
	{
		col_ctx.tex = &game->cfg.south_texture;
	}
	init_slice(&col_ctx, ray);
	draw_slice(&game->main.frame, col_x, col_ctx);
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
	draw_map(&game->debug.frame, game->map);
	draw_player(&game->debug.frame, game->player);
	draw_floor_and_ceiling(&game->main.frame, game->cfg);
	do_raycasting(game);
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
		return (1);
	print_error(0, "map ok\n");
	init_game(&game);
	create_window(&game.main);
	create_window(&game.debug);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
