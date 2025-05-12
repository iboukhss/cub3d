/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:12:44 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:26:21 by iboukhss         ###   ########.fr       */
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

double	rad(double deg)
{
	return (deg * (M_PI / 180.0));
}
