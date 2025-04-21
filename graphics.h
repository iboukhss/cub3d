/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:53:21 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/11 16:52:58 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains useful type definitions and functions for interacting
// with MinilibX

#ifndef GRAPHICS_H
# define GRAPHICS_H

// This is ARGB32 format
# define COLOR_RED			0x00FF0000
# define COLOR_GREEN		0x0000FF00
# define COLOR_BLUE			0x000000FF
# define COLOR_YELLOW		0x00FFFF00
# define COLOR_WHITE		0x00FFFFFF
# define COLOR_BLACK		0x00000000
# define COLOR_GRAY			0x00808080
# define COLOR_LIGHT_GRAY	0x00D3D3D3
# define COLOR_DARK_GRAY	0x00A9A9A9

# include <mlx.h>
# include <stddef.h>
# include <stdint.h>
# include <X11/keysym.h>
# include <X11/X.h>

// Convenient MLX data structures
typedef struct s_image
{
	void	*img_ctx;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		bytes_per_line;
	int		endian;
}	t_image;

typedef struct s_window
{
	void		*win_ctx;
	t_image		frame;
	int			width;
	int			height;
	const char	*title;
	int			(*loop_hook)(void *);
	int			(*key_press_hook)(int, void *);
	int			(*key_release_hook)(int, void *);
	int			(*button_press_hook)(int, int, int, void *);
	int			(*button_release_hook)(int, int, int, void *);
	int			(*motion_notify_hook)(int, int, int, void *);
	int			(*expose_hook)(void *);
	int			(*destroy_notify_hook)(void *);
	void		*param;
}	t_window;

// Raster types
typedef struct s_line
{
	int	x0;
	int	y0;
	int	x1;
	int	y1;
}	t_line;

typedef struct s_circle
{
	int	x;
	int	y;
	int	r;
}	t_circle;

typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_rect;

int			create_window(t_window *win, void *mlx_ctx);
int			destroy_window(t_window *win, void *mlx_ctx);

uint32_t	rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue);

int			put_pixel(t_image *img, int pos_x, int pos_y, uint32_t color);
int			fill_rect(t_image *img, t_rect rect, uint32_t color);
int			draw_line(t_image *img, t_line line, uint32_t color);

#endif // GRAPHICS_H
