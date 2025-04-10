/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:27:45 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/10 12:39:54 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <math.h>

uint32_t	rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red << 16) | (green << 8) | blue);
}

// Assuming ARGB32 (little endian) which means BGRA byte order
// See: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
int	put_pixel(t_image *img, int pos_x, int pos_y, uint32_t color)
{
	char	*pixel;
	int		bytes_per_line;
	int		bytes_per_pixel;

	if (pos_x < 0 || pos_x >= img->width || pos_y < 0 || pos_y >= img->height)
	{
		return (-1);
	}
	bytes_per_line = img->bytes_per_line;
	bytes_per_pixel = img->bits_per_pixel / 8;
	pixel = img->addr + pos_y * bytes_per_line + pos_x * bytes_per_pixel;
	if (bytes_per_pixel == sizeof(color))
	{
		*(uint32_t *)pixel = color;
	}
	return (0);
}

int	fill_rect(t_image *img, t_rect rect, uint32_t color)
{
	for (int y = rect.y; y < rect.y + rect.h; y++)
	{
		for (int x = rect.x; x < rect.x + rect.w; x++)
		{
			put_pixel(img, x, y, color);
		}
	}
	return (0);
}

// Using DDA to draw lines
//
// NOTE(ismail): It seems like sometimes lines are not rasterized properly,
// esp. vertical lines. Minor issue.
int	draw_line(t_image *img, t_line line, uint32_t color)
{
	float	dx = line.x1 - line.x0;
	float	dy = line.y1 - line.y0;
	int		steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);

	float	x_inc = dx / steps;
	float	y_inc = dy / steps;

	float	x = line.x0;
	float	y = line.y0;

	for (int i = 0; i <= steps; i++)
	{
		put_pixel(img, roundf(x), roundf(y), color);
		x += x_inc;
		y += y_inc;
	}
	return (0);
}
