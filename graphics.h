/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:53:21 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/10 12:40:14 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This file contains useful type definitions and functions for interacting
// with MinilibX

#ifndef GRAPHICS_H
# define GRAPHICS_H

# include <mlx.h>
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
	void	*win_ctx;
	int		width;
	int		height;
	char	*title;
	t_image	frame;
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

uint32_t	rgb_to_hex(uint8_t red, uint8_t green, uint8_t blue);

int			put_pixel(t_image *img, int pos_x, int pos_y, uint32_t color);
int			fill_rect(t_image *img, t_rect rect, uint32_t color);
int			draw_line(t_image *img, t_line line, uint32_t color);

#endif // GRAPHICS_H
