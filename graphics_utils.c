/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:27:45 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/11 16:53:41 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <math.h>

int	create_window(t_window *win, void *mlx_ctx)
{
	if (win->win_ctx)
	{
		return (1);
	}
	win->win_ctx = mlx_new_window(mlx_ctx, win->width, win->height, (char *)win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx,
										&win->frame.bits_per_pixel,
										&win->frame.bytes_per_line,
										&win->frame.endian);
	if (win->loop_hook)
		mlx_loop_hook(mlx_ctx, win->loop_hook, win->param);
	if (win->key_press_hook)
		mlx_hook(win->win_ctx, KeyPress, KeyPressMask, win->key_press_hook, win->param);
	if (win->key_release_hook)
		mlx_hook(win->win_ctx, KeyRelease, KeyReleaseMask, win->key_release_hook, win->param);
	if (win->button_press_hook)
		mlx_hook(win->win_ctx, ButtonPress, ButtonPressMask, win->button_press_hook, win->param);
	if (win->button_release_hook)
		mlx_hook(win->win_ctx, ButtonRelease, ButtonReleaseMask, win->button_release_hook, win->param);
	if (win->motion_notify_hook)
		mlx_hook(win->win_ctx, MotionNotify, PointerMotionMask, win->motion_notify_hook, win->param);
	if (win->expose_hook)
		mlx_hook(win->win_ctx, Expose, ExposureMask, win->expose_hook, win->param);
	if (win->destroy_notify_hook)
		mlx_hook(win->win_ctx, DestroyNotify, StructureNotifyMask, win->destroy_notify_hook, win->param);
	return (0);
}

int	destroy_window(t_window *win, void *mlx_ctx)
{
	if (!win->win_ctx)
	{
		return (1);
	}
	mlx_destroy_image(mlx_ctx, win->frame.img_ctx);
	win->frame.img_ctx = NULL;
	mlx_destroy_window(mlx_ctx, win->win_ctx);
	win->win_ctx = NULL;
	return (0);
}

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
