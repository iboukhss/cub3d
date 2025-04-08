/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 21:13:52 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/08 19:13:02 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800
# define CELL_WIDTH 32

# include <stdbool.h>
# include <X11/keysym.h>
# include <X11/X.h>

enum e_orientation
{
	EAST = 0,
	NORTH = 90,
	WEST = 180,
	SOUTH = 270,
};

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

// Math types
typedef struct s_vec2d
{
	float	x;
	float	y;
}	t_vec2d;

typedef struct s_ray2d
{
	t_vec2d	o;
	t_vec2d	dir;
}	t_ray2d;

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

// NOTE(ismail): Make grid a null-terminated array of null-terminated strings.
// This will help us avoid segfaults and out-of-bounds array access.
// Width and height might not even be necessary but I added them just in case we
// might need them later.
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct s_camera
{
	int		angle_deg;
	float	angle_rad;
	t_vec2d	pos;
	t_vec2d	dir;
	t_vec2d	plane;
}	t_camera;

typedef struct s_player
{
	int					start_x;
	int					start_y;
	enum e_orientation	orientation;
	float				width;
	t_camera			cam;
}	t_player;

// This should hold the texture configuration data. Didn't look at this yet.
typedef struct s_config
{
	// Add configuration data here
}	t_config;

// Main game structure
// win0: Main game window
// win1: Debug view
typedef struct s_game
{
	void		*mlx_ctx;
	t_window	win0;
	t_window	win1;
	t_map		map;
	t_player	player;
	t_config	conf;
}	t_game;

#endif
