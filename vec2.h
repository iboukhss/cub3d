/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:07:22 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:26:37 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC2_H
# define VEC2_H

# include "graphics.h"

// Vector types
typedef struct s_vec2d
{
	float	x;
	float	y;
}	t_vec2d;

typedef struct s_ray2d
{
	t_vec2d	pos;
	t_vec2d	dir;
}	t_ray2d;

t_vec2d	vec2d_init(float x, float y);
t_vec2d	vec2d_mul(t_vec2d v, float s);
t_vec2d	vec2d_add(t_vec2d a, t_vec2d b);
t_ray2d	ray2d_init(t_vec2d pos, t_vec2d dir);

double	rad(double deg);

#endif // VEC2_H
