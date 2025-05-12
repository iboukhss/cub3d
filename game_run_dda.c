/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_run_dda.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:56:56 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 17:58:17 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

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

static bool	dda_done(struct s_dda_ctx d, t_map map)
{
	return (d.map_x < 0 || d.map_y >= map.width
		|| d.map_y < 0 || d.map_y >= map.height
		|| map.grid[d.map_y][d.map_x] == '1');
}

int	run_dda(t_ray *ray, struct s_dda_ctx d, t_map map)
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
