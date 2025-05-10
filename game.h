/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:19:47 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/10 10:15:33 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# define WIN_WIDTH 960
# define WIN_HEIGHT 600
# define TILE_SIZE 16

// 255*255 is approx. 64 kB
# define MAX_GRID_ROW 255
# define MAX_GRID_COL 255

// sentinel value for floor_color and ceil_color
# define COLOR_UNSET 0xFFFFFFFF

#define WHITESPACE " "
#define TABS "\t\v\f\r"

# include "graphics.h"
# include "vec2.h"

# include <stddef.h>

typedef enum e_orientation
{
	ORIENT_UNKNOWN = -1,
	ORIENT_EAST = 0,
	ORIENT_NORTH = 90,
	ORIENT_WEST = 180,
	ORIENT_SOUTH = 270,
}	t_orientation;

typedef struct s_ray
{
	t_vec2d			pos;
	t_vec2d			dir;
	float			len;
	t_orientation	side;
}	t_ray;

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
	int				start_x;
	int				start_y;
	t_orientation	spawn_orientation;
	float			width;
	t_camera		cam;
}	t_player;

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
	uint32_t	north_color;	// temporary colors for NO,WE,SO,EA
	uint32_t	south_color;
	uint32_t	east_color;
	uint32_t	west_color;
	uint32_t 	floor_color;	// can store colors inside a single 32 bit
	uint32_t     ceil_color;		// integer with using rgb_to_hex()
	int			done;
}	t_config;

// Main game structure
typedef struct s_game
{
	void		*mlx_ctx;
	t_window	main;
	t_window	debug;
	t_map		map;
	t_map		tmp_map;			//used by flood_fill algo
	t_player	player;
	t_config	cfg;
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
char	*get_element(char *line, char **element);
char	*get_rgb_code(char *line, char **element);
int		get_scene(t_game *game, char *scene_path);
int		load_all_textures(t_game *game);
int		load_image(t_game *game, t_image *texture_img, char *path);
int		read_scene(t_game *game, char *scene_path);
void	reset_config(t_config *config);
int		reset_map(t_map *map);
int		validate_map(t_game *game);
void	clone_map(t_map *src, t_map *dest);
void	print_error(int default_prompt, char *error_msg);
void	print_config(t_config config);
void	print_map(t_map map);
void	set_player_pos(t_game *game, char orientation, int y, int x);

#endif // GAME_H
