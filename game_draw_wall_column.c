/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_draw_wall_column.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:00:36 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:02:09 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

static int	init_slice(struct s_col_ctx *c, t_ray ray)
{
	c->column_height = (int)(WIN_HEIGHT / ray.len);
	c->draw_start = -c->column_height / 2 + WIN_HEIGHT / 2;
	c->draw_end = c->column_height / 2 + WIN_HEIGHT / 2;
	if (c->draw_start < 0)
		c->draw_start = 0;
	if (c->draw_end >= WIN_HEIGHT)
		c->draw_end = WIN_HEIGHT - 1;
	if (ray.side == ORIENT_EAST || ray.side == ORIENT_WEST)
		c->wall_x = ray.pos.y + ray.len * ray.dir.y;
	else
		c->wall_x = ray.pos.x + ray.len * ray.dir.x;
	c->wall_x -= floorf(c->wall_x);
	c->tex_x = (int)(c->wall_x * c->tex->width);
	c->step = 1.0f * c->tex->height / c->column_height;
	c->tex_pos = (c->draw_start - WIN_HEIGHT / 2.0f + c->column_height / 2.0f)
		* c->step;
	return (0);
}

static int	draw_slice(t_image *img, int col_x, struct s_col_ctx c)
{
	int			tex_y;
	uint32_t	color;
	int			y;

	y = c.draw_start;
	while (y < c.draw_end)
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
		y++;
	}
	return (0);
}

int	draw_wall_column(t_game *game, int col_x, t_ray ray)
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
