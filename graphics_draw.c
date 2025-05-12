/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_draw.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:37:24 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:44:16 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <math.h>

int	fill_rect(t_image *img, t_rect rect, uint32_t color)
{
	int	x;
	int	y;

	y = rect.y;
	while (y < rect.y + rect.h)
	{
		x = rect.x;
		while (x < rect.x + rect.w)
		{
			put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
	return (0);
}

static int	init_dda(struct s_dda *d, t_line line)
{
	float	dx;
	float	dy;

	dx = line.x1 - line.x0;
	dy = line.y1 - line.y0;
	if (fabsf(dx) > fabsf(dy))
	{
		d->steps = fabsf(dx);
	}
	else
	{
		d->steps = fabsf(dy);
	}
	d->x_inc = dx / (float)d->steps;
	d->y_inc = dy / (float)d->steps;
	return (0);
}

// Using DDA to draw lines
int	draw_line(t_image *img, t_line line, uint32_t color)
{
	struct s_dda	dda;
	float			x;
	float			y;
	int				i;

	init_dda(&dda, line);
	x = line.x0;
	y = line.y0;
	i = 0;
	while (i <= dda.steps)
	{
		put_pixel(img, (int)(x + 0.5f), (int)(y + 0.5f), color);
		x += dda.x_inc;
		y += dda.y_inc;
		i++;
	}
	return (0);
}
