/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 21:13:52 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/04 16:51:53 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800
# define CELL_WIDTH 32

#define WHITESPACE " "
#define IDENTIFIER "NOEAWESOFC"

# include <stdbool.h>
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

// Utility classes for math calculations
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
	char	*scene_path;
	char	**grid;
	int		width;
	int		height;
	char	spawn_orientation;
}	t_map;

// NOTE(ismail): This is a mess right now. To summarize:
// cx: Player center x coordinate (add 0.5 to center on the pixel grid)
// cy: Player center y coordinate (add 0.5 to center on the pixel grid)
// angle_deg: Starting angle direction, as in: NORTH = 90, WEST = 180,
// SOUTH = 270 and EAST = 0 or 360
// fov_deg: Player field of view (anywhere between 60 and 90 is good)
// Everything else is temporary and subject to change.
typedef struct s_player
{
	float	cx;
	float	cy;
	float	radius;
	float	dx;
	float	dy;
	int		angle_deg;
	float	angle_rad;
	int		fov_deg;
	float	ldx;
	float	ldy;
	float	rdx;
	float	rdy;
}	t_player;

typedef struct s_rgb
{
	int	red;
	int	green;
	int	blue;
}	t_rgb;


// This should hold the texture configuration data. Didn't look at this yet.
typedef struct s_config
{
	char	*NO;
	char	*SO;
	char	*WE;
	char	*EA;
	t_rgb	*floor;
	t_rgb	*ceiling;
}	t_config;

// Main game structure
typedef struct s_game
{
	void		*mlx_ctx;
	t_window	win;
	t_map		map;
	t_player	player;
	t_config	config;
}	t_game;

char	*get_next_line(int fd);
void	print_error(int default_prompt, char *error_msg);
int		get_scene(t_game *game, char *scene_path);
int		read_scene(t_config *config, char *scene_path);

#endif
