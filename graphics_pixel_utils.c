/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_pixel_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:35:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:36:21 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

uint32_t	rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red << 16) | (green << 8) | blue);
}

// Assuming ARGB32 (little endian) which means BGRA byte order
// See: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
int	put_pixel(t_image *img, int pos_x, int pos_y, uint32_t color)
{
	int		bytes_per_line;
	int		bytes_per_pixel;
	char	*pixel;

	if (!img->img_ctx)
	{
		return (1);
	}
	if (pos_x < 0 || pos_x >= img->width || pos_y < 0 || pos_y >= img->height)
	{
		return (2);
	}
	bytes_per_line = img->bytes_per_line;
	bytes_per_pixel = img->bits_per_pixel / 8;
	pixel = img->addr + pos_y * bytes_per_line + pos_x * bytes_per_pixel;
	*(uint32_t *)pixel = color;
	return (0);
}

uint32_t	get_pixel(t_image *img, int pos_x, int pos_y)
{
	int		bytes_per_line;
	int		bytes_per_pixel;
	char	*pixel;

	if (!img->img_ctx)
	{
		return (1);
	}
	if (pos_x < 0 || pos_x >= img->width || pos_y < 0 || pos_y >= img->height)
	{
		return (2);
	}
	bytes_per_line = img->bytes_per_line;
	bytes_per_pixel = img->bits_per_pixel / 8;
	pixel = img->addr + pos_y * bytes_per_line + pos_x * bytes_per_pixel;
	return (*(uint32_t *)pixel);
}
