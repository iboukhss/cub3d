/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init_dda.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:58:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 17:59:12 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

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
int	init_dda(struct s_dda_ctx *d, t_ray ray)
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
