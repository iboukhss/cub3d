/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:19:47 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/10 00:00:59 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800
# define TILE_SIZE 32

# include "graphics.h"
# include "vec2.h"

# include <stddef.h>

enum e_orientation
{
	EAST = 0,
	NORTH = 90,
	WEST = 180,
	SOUTH = 270,
};

// NOTE(ismail): Make grid a null-terminated array of null-terminated strings.
// This will help us avoid segfaults and out-of-bounds array access.
// If we could make the map a rectangle of dimensions width x height that would
// also be great also. Holes in the map could be padded with spaces for example.
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

int	init_game(t_game *game);
int	init_hooks(t_game *game);
int	render_scene(t_game *game);
int	destroy_game(t_game *game);

#endif // GAME_H
