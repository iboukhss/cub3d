/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:27:45 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 13:28:23 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <math.h>

static void	add_loop_hook(t_window *win)
{
	if (win->loop_hook)
	{
		mlx_loop_hook(win->mlx_ctx, win->loop_hook, win->param);
	}
}

static void	add_key_press_hook(t_window *win)
{
	if (win->key_press_hook)
	{
		mlx_hook(win->win_ctx, KeyPress, KeyPressMask, win->key_press_hook, win->param);
	}
}

static void	add_key_release_hook(t_window *win)
{
	if (win->key_release_hook)
	{
		mlx_hook(win->win_ctx, KeyRelease, KeyReleaseMask, win->key_release_hook, win->param);
	}
}

static void	add_button_press_hook(t_window *win)
{
	if (win->button_press_hook)
	{
		mlx_hook(win->win_ctx, ButtonPress, ButtonPressMask, win->button_press_hook, win->param);
	}
}

static void	add_button_release_hook(t_window *win)
{
	if (win->button_release_hook)
	{
		mlx_hook(win->win_ctx, ButtonRelease, ButtonReleaseMask, win->button_release_hook, win->param);
	}
}

static void	add_motion_notify_hook(t_window *win)
{
	if (win->motion_notify_hook)
	{
		mlx_hook(win->win_ctx, MotionNotify, PointerMotionMask, win->motion_notify_hook, win->param);
	}
}

static void	add_expose_hook(t_window *win)
{
	if (win->expose_hook)
	{
		mlx_hook(win->win_ctx, Expose, ExposureMask, win->expose_hook, win->param);
	}
}

static void	add_destroy_notify_hook(t_window *win)
{
	if (win->destroy_notify_hook)
	{
		mlx_hook(win->win_ctx, DestroyNotify, StructureNotifyMask, win->destroy_notify_hook, win->param);
	}
}

int	create_image(t_image *img, int width, int height)
{
	img->img_ctx = mlx_new_image(img->mlx_ctx, width, height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel, &img->bytes_per_line, &img->endian);
	return (0);
}

int	create_window(t_window *win)
{
	if (win->win_ctx)
	{
		return (1);
	}
	win->win_ctx = mlx_new_window(win->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	create_image(&win->frame, win->frame.width, win->frame.height);
	add_loop_hook(win);
	add_key_press_hook(win);
	add_key_release_hook(win);
	add_button_press_hook(win);
	add_button_release_hook(win);
	add_motion_notify_hook(win);
	add_expose_hook(win);
	add_destroy_notify_hook(win);
	return (0);
}

int	destroy_image(t_image *img)
{
	if (!img->img_ctx)
	{
		return (1);
	}
	mlx_destroy_image(img->mlx_ctx, img->img_ctx);
	img->img_ctx = NULL;
	return (0);
}

int	destroy_window(t_window *win)
{
	if (!win->win_ctx)
	{
		return (1);
	}
	mlx_destroy_window(win->mlx_ctx, win->win_ctx);
	win->win_ctx = NULL;
	destroy_image(&win->frame);
	return (0);
}

uint32_t	rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red << 16) | (green << 8) | blue);
}

int	load_xpm_data(t_image *img, char **xpm_data, void *mlx_ctx)
{
	img->img_ctx = mlx_xpm_to_image(mlx_ctx, xpm_data, &img->width, &img->height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel, &img->bytes_per_line, &img->endian);
	return (0);
}

int	load_xpm_file(t_image *img, char *filename)
{
	img->img_ctx = mlx_xpm_file_to_image(img->mlx_ctx, filename, &img->width, &img->height);
	if (!img->img_ctx)
	{
		return (1);
	}
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel, &img->bytes_per_line, &img->endian);
	return (0);
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
int	draw_line(t_image *img, t_line line, uint32_t color)
{
	float	dx = line.x1 - line.x0;
	float	dy = line.y1 - line.y0;
	int		steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);

	float	x_inc = dx / (float)steps;
	float	y_inc = dy / (float)steps;

	float	x = line.x0;
	float	y = line.y0;

	for (int i = 0; i <= steps; i++)
	{
		put_pixel(img, (int)(x + 0.5f), (int)(y + 0.5f), color);
		x += x_inc;
		y += y_inc;
	}
	return (0);
}
