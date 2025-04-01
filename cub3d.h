/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 21:13:52 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/01 18:00:02 by iboukhss         ###   ########.fr       */
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

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		cell_width;
	int		stroke_width;
}	t_map;

typedef struct s_player
{
	float	x_pos;
	float	y_pos;
	int		fov;
	int		width;
}	t_player;

typedef struct s_config
{
	// Add configuration data here
}	t_config;

typedef struct s_ray
{
	// Add raycasting data here
}	t_ray;

typedef struct s_game
{
	void		*mlx_ctx;
	t_window	win;
	t_map		map;
	t_player	player;
	t_config	conf;
}	t_game;

#endif
