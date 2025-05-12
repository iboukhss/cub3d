/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:27:45 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 17:23:16 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <math.h>

static void	register_hook(t_window *win, struct s_hook h)
{
	if (h.func)
	{
		mlx_hook(win->win_ctx, h.x_event, h.x_mask, h.func, h.param);
	}
}

static void	add_loop_hook(t_window *win)
{
	if (win->loop_hook)
	{
		mlx_loop_hook(win->mlx_ctx, win->loop_hook, win->param);
	}
}

static void	add_key_press_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = KeyPress;
	h.x_mask = KeyPressMask;
	h.func = win->key_press_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_key_release_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = KeyRelease;
	h.x_mask = KeyReleaseMask;
	h.func = win->key_release_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_button_press_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = ButtonPress;
	h.x_mask = ButtonPressMask;
	h.func = win->button_press_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_button_release_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = ButtonRelease;
	h.x_mask = ButtonReleaseMask;
	h.func = win->button_release_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_motion_notify_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = MotionNotify;
	h.x_mask = PointerMotionMask;
	h.func = win->motion_notify_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_expose_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = Expose;
	h.x_mask = ExposureMask;
	h.func = win->expose_hook;
	h.param = win->param;
	register_hook(win, h);
}

static void	add_destroy_notify_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = DestroyNotify;
	h.x_mask = StructureNotifyMask;
	h.func = win->destroy_notify_hook;
	h.param = win->param;
	register_hook(win, h);
}

int	create_image(t_image *img, int width, int height)
{
	img->width = width;
	img->height = height;
	img->img_ctx = mlx_new_image(img->mlx_ctx, width, height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
	return (0);
}

int	create_window(t_window *win)
{
	if (win->win_ctx)
	{
		return (1);
	}
	win->win_ctx = mlx_new_window(win->mlx_ctx, win->width, win->height,
			win->title);
	create_image(&win->frame, win->width, win->height);
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
	img->img_ctx = mlx_xpm_to_image(mlx_ctx, xpm_data, &img->width,
			&img->height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
	return (0);
}

int	load_xpm_file(t_image *img, char *filename)
{
	img->img_ctx = mlx_xpm_file_to_image(img->mlx_ctx, filename, &img->width,
			&img->height);
	if (!img->img_ctx)
	{
		return (1);
	}
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
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
