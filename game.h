/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:19:47 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/18 17:30:37 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# define WIN_WIDTH 1280
# define WIN_HEIGHT 800
# define TILE_SIZE 24

// 255*255 is approx. 64 kB
# define MAX_GRID_ROW 255
# define MAX_GRID_COL 255

#define WHITESPACE " "
#define IDENTIFIER "NOEAWESOFC"

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

typedef struct s_map
{
	char	*scene_path;
	char	grid[MAX_GRID_ROW][MAX_GRID_COL];
	int		width;
	int		height;
	char	spawn_orientation;		// not needed anymore, inside t_player
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

typedef struct s_rgb
{
	int	red;
	int	green;
	int	blue;
}	t_rgb;

// This should hold the texture configuration data. Didn't look at this yet.
typedef struct s_config
{
	char		*NO;
	char		*SO;
	char		*WE;
	char		*EA;
	t_image		texture_NO;      // store in void *img_ctx
	t_image		texture_SO;
	t_image		texture_WE;
	t_image		texture_EA;
	t_rgb		*floor;
	t_rgb		*ceiling;
	uint32_t	floor_color;	// can store colors inside a single 32 bit
	uint32_t	ceil_color;		// integer with using rgb_to_hex()
	int			done;
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
	t_config	config;
}	t_game;

int	init_game(t_game *game);
int	destroy_game(t_game *game);

// Hooks
int	close_main_window(void *param);
int	close_debug_window(void *param);

int	render_scene(void *param);

int	keyrelease_main(int keysym, void *param);
int	keyrelease_debug(int keysym, void *param);
int	keypress_main(int keysym, void *param);

// Newly added
char	*get_next_line(int fd);
char	*skip_whitespace(char *line);
int		extract_param(t_config *config, char *line);
int		get_scene(t_game *game, char *scene_path);
int		read_scene(t_game *game, char *scene_path);
void	print_error(int default_prompt, char *error_msg);
void	print_config(t_config config);
void	print_map(t_map map);

#endif // GAME_H
