/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:12:44 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/29 01:37:17 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec2.h"

#include <math.h>

t_vec2d	vec2d_init(float x, float y)
{
	return ((t_vec2d){x, y});
}

t_vec2d	vec2d_mul(t_vec2d v, float s)
{
	return ((t_vec2d){v.x * s, v.y * s});
}

t_vec2d	vec2d_add(t_vec2d a, t_vec2d b)
{
	return ((t_vec2d){a.x + b.x, a.y + b.y});
}

t_ray2d	ray2d_init(t_vec2d pos, t_vec2d dir)
{
	return ((t_ray2d){pos, dir});
}

// We use this to rasterize the ray after finding its length
t_line	ray2d_to_line(t_ray2d ray, float len, float raster_factor)
{
	t_vec2d	p1;
	t_vec2d	p2;
	t_vec2d	dir;
	t_line	line;

	dir = vec2d_mul(ray.dir, len);
	p1 = ray.pos;
	p2 = vec2d_add(p1, dir);
	p1 = vec2d_mul(p1, raster_factor);
	p2 = vec2d_mul(p2, raster_factor);
	line.x0 = (int)p1.x;
	line.y0 = (int)p1.y;
	line.x1 = (int)p2.x;
	line.y1 = (int)p2.y;
	return (line);
}

double	rad(double deg)
{
	return (deg * (M_PI / 180.0));
}
