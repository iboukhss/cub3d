/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/09 16:05:49 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static t_vec2d	vec2d_init(float x, float y)
{
	return ((t_vec2d){x, y});
}

static t_vec2d	vec2d_scale(t_vec2d v, float s)
{
	return ((t_vec2d){v.x * s, v.y * s});
}

static t_vec2d	vec2d_add(t_vec2d a, t_vec2d b)
{
	return ((t_vec2d){a.x + b.x, a.y + b.y});
}

static t_ray2d	ray2d_init(t_vec2d pos, t_vec2d dir)
{
	return ((t_ray2d){pos, dir});
}

// We use this to rasterize the ray after finding its length
static t_line	ray2d_to_line(t_ray2d ray, float len, float raster_factor)
{
	t_vec2d	p1;
	t_vec2d	p2;
	t_vec2d	dir;
	t_line	line;

	dir = vec2d_scale(ray.dir, len);
	p1 = ray.pos;
	p2 = vec2d_add(p1, dir);
	p1 = vec2d_scale(p1, raster_factor);
	p2 = vec2d_scale(p2, raster_factor);
	line.x0 = (int)p1.x;
	line.y0 = (int)p1.y;
	line.x1 = (int)p2.x;
	line.y1 = (int)p2.y;
	return (line);
}

static double	rad(double deg)
{
	return (deg * (M_PI / 180.0));
}

static const char	*g_testmap[] = {
	"        1111111111111111111111111",
	"        1000000000110000000000001",
	"        1011000001110000000000001",
	"        1001000000000000000000001",
	"111111111011000001110000000000001",
	"100000000011000001110111111111111",
	"11110111111111011100000010001    ",
	"11110111111111011101010010001    ",
	"11000000110101011100000010001    ",
	"10000000000000001100000010001    ",
	"10000000000000001101010010001    ",
	"1100000111010101111101111000111  ",
	"1111N111 1110101 101111010001    ",
	"11111111 1111111 111111111111    ",
	NULL,
};

static void	print_position(t_player *player)
{
	printf("Px : %f\n", player->cam.pos.x);
	printf("Py : %f\n", player->cam.pos.y);
	printf("Dx : %f\n", player->cam.dir.x);
	printf("Dy : %f\n", player->cam.dir.y);
	printf("Cx : %f\n", player->cam.plane.x);
	printf("Cy : %f\n", player->cam.plane.y);
	printf("Deg: %d\n", player->cam.angle_deg);
	printf("Rad: %f\n", player->cam.angle_rad);
	printf("---------------------\n");
}

static int	init_map(t_map *map)
{
	map->grid = (char **)g_testmap;
	map->width = 33;
	map->height = 14;
	return (0);
}

// NOTE(ismail): Y-axis is inverted compared to the regular unit circle.
// Be mindful when dealing with rotations.
static int	init_camera(t_camera *cam, t_player *player)
{
	cam->angle_deg = player->orientation;
	cam->angle_rad = rad(cam->angle_deg);
	cam->pos = vec2d_init(player->start_x + 0.5f, player->start_y + 0.5f);
	cam->dir = vec2d_init(cosf(cam->angle_rad), -sinf(cam->angle_rad));
	cam->plane = vec2d_init(sinf(cam->angle_rad), cosf(cam->angle_rad));
	return (0);
}

static int	init_player(t_player *player)
{
	player->start_x = 4;
	player->start_y = 12;
	player->orientation = NORTH;
	player->width = 0.5;
	init_camera(&player->cam, player);
	return (0);
}

static int	create_main_window(t_window *win, t_game *game)
{
	win->width = WIN_WIDTH;
	win->height = WIN_HEIGHT;
	win->title = "cub3D";
	win->win_ctx = mlx_new_window(game->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(game->mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx, &win->frame.bits_per_pixel, &win->frame.bytes_per_line, &win->frame.endian);
	return (0);
}

static int	create_debug_window(t_window *win, t_game *game)
{
	win->width = game->map.width * TILE_SIZE;
	win->height = game->map.height * TILE_SIZE;
	win->title = "Map view";
	win->win_ctx = mlx_new_window(game->mlx_ctx, win->width, win->height, win->title);
	win->frame.width = win->width;
	win->frame.height = win->height;
	win->frame.img_ctx = mlx_new_image(game->mlx_ctx, win->frame.width, win->frame.height);
	win->frame.addr = mlx_get_data_addr(win->frame.img_ctx, &win->frame.bits_per_pixel, &win->frame.bytes_per_line, &win->frame.endian);
	return (0);
}

static int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	init_map(&game->map);
	init_player(&game->player);
	create_main_window(&game->win0, game);
	create_debug_window(&game->win1, game);
	return (0);
}

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win0.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win0.win_ctx);
	mlx_destroy_image(game->mlx_ctx, game->win1.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win1.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
	return (0);
}

// Assuming ARGB32 (little endian) which means BGRA byte order
// See: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
static int	put_pixel(t_image *img, int x_pos, int y_pos, int color)
{
	char	*pixel;
	int		bytes_per_line;
	int		bytes_per_pixel;

	if (x_pos < 0 || x_pos >= img->width || y_pos < 0 || y_pos >= img->height)
	{
		return (-1);
	}
	bytes_per_line = img->bytes_per_line;
	bytes_per_pixel = img->bits_per_pixel / 8;
	pixel = img->addr + y_pos * bytes_per_line + x_pos * bytes_per_pixel;
	if (bytes_per_pixel == sizeof(unsigned int))
	{
		*(unsigned int *)pixel = color;
	}
	return (0);
}

// Using DDA to draw lines
//
// NOTE(ismail): It seems like sometimes lines are not rasterized properly,
// esp. vertical lines. Minor issue.
static int	draw_line(t_image *img, t_line *line, int color)
{
	float	dx = line->x1 - line->x0;
	float	dy = line->y1 - line->y0;
	int		steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);

	float	x_inc = dx / steps;
	float	y_inc = dy / steps;

	float	x = line->x0;
	float	y = line->y0;

	for (int i = 0; i <= steps; i++)
	{
		put_pixel(img, roundf(x), roundf(y), color);
		x += x_inc;
		y += y_inc;
	}
	return (0);
}

static int	fill_rect(t_image *img, t_rect *rect, int color)
{
	for (int y = rect->y; y < rect->y + rect->h; y++)
	{
		for (int x = rect->x; x < rect->x + rect->w; x++)
		{
			put_pixel(img, x, y, color);
		}
	}
	return (0);
}

static int	draw_map(t_image *frame, t_map *map)
{
	t_rect	tile;

	for (int y = 0; map->grid[y] != NULL && y < map->height; y++)
	{
		for (int x = 0; map->grid[y][x] != '\0' && x < map->width; x++)
		{
			tile.x = x * TILE_SIZE;
			tile.y = y * TILE_SIZE;
			tile.w = TILE_SIZE;
			tile.h = TILE_SIZE;
			if (map->grid[y][x] == '1')
			{
				fill_rect(frame, &tile, 0xFFFFFF);
			}
			else if (map->grid[y][x] == ' ')
			{
				fill_rect(frame, &tile, 0x808080);
			}
			else
			{
				fill_rect(frame, &tile, 0x000000);
			}
		}
	}
	return (0);
}

static t_rect	player_to_rect(t_player *player, float raster_factor)
{
	t_rect	rect;

	rect.x = (player->cam.pos.x - player->width / 2) * raster_factor;
	rect.y = (player->cam.pos.y - player->width / 2) * raster_factor;;
	rect.w = player->width * raster_factor;
	rect.h = player->width * raster_factor;
	return (rect);
}

// Draws the player's hitbox
static int	draw_player(t_image *frame, t_player *player)
{
	t_rect	hitbox;

	hitbox = player_to_rect(player, TILE_SIZE);
	fill_rect(frame, &hitbox, 0xFF0000);
	return (0);
}

// Returns the distance travelled by the ray
static float	cast_ray(t_ray2d ray, t_map *map)
{
	int		map_x = (int)ray.pos.x;
	int		map_y = (int)ray.pos.y;

	float	delta_dist_x = (ray.dir.x == 0) ? 1e30f : fabsf(1.0f / ray.dir.x);
	float	delta_dist_y = (ray.dir.y == 0) ? 1e30f : fabsf(1.0f / ray.dir.y);

	float	side_dist_x;
	float	side_dist_y;

	int		step_x;
	int		step_y;

	int		side;
	float	ray_length;

	if (ray.dir.x < 0)
	{
		step_x = -1;
		side_dist_x = (ray.pos.x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0f - ray.pos.x) * delta_dist_x;
	}
	if (ray.dir.y < 0)
	{
		step_y = -1;
		side_dist_y = (ray.pos.y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0f - ray.pos.y) * delta_dist_y;
	}
	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (map_x < 0 || map_x >= map->width || map_y < 0 || map_y >= map->height || map->grid[map_y][map_x] == '1')
		{
			break ;
		}
	}
	if (side == 0)
	{
		ray_length = side_dist_x - delta_dist_x;
	}
	else
	{
		ray_length = side_dist_y - delta_dist_y;
	}
	return (ray_length);
}

static int	render_scene(t_game *game)
{
	float	camera_x;
	t_ray2d	ray;
	float	ray_length;
	t_line	line;

	draw_map(&game->win1.frame, &game->map);
	draw_player(&game->win1.frame, &game->player);
	// This is where we cast a ray for each screen column
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos.x = game->player.cam.pos.x;
		ray.pos.y = game->player.cam.pos.y;
		ray.dir.x = game->player.cam.dir.x + game->player.cam.plane.x * camera_x;
		ray.dir.y = game->player.cam.dir.y + game->player.cam.plane.y * camera_x;
		ray_length = cast_ray(ray, &game->map);
		line = ray2d_to_line(ray, ray_length, TILE_SIZE);
		draw_line(&game->win1.frame, &line, 0xFFFF00);
	}
	mlx_put_image_to_window(game->mlx_ctx, game->win1.win_ctx, game->win1.frame.img_ctx, 0, 0);
	return (0);
}

static int	move_camera(t_camera *cam, float move_speed)
{
	t_vec2d	move;
	t_vec2d	dest;

	move = vec2d_scale(cam->dir, move_speed);
	dest = vec2d_add(cam->pos, move);
	cam->pos = dest;
	return (0);
}

static int	rotate_camera(t_camera *cam, int angle_deg)
{
	cam->angle_deg = (cam->angle_deg + angle_deg + 360) % 360;
	cam->angle_rad = rad(cam->angle_deg);
	cam->dir = vec2d_init(cosf(cam->angle_rad), -sinf(cam->angle_rad));
	cam->plane = vec2d_init(sinf(cam->angle_rad), cos(cam->angle_rad));
	return (0);
}

static int	key_press_hook(int keysym, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keysym == XK_Up)
	{
		move_camera(&game->player.cam, 0.25f);
	}
	if (keysym == XK_Down)
	{
		move_camera(&game->player.cam, -0.25f);
	}
	if (keysym == XK_Left)
	{
		rotate_camera(&game->player.cam, 15);
	}
	if (keysym == XK_Right)
	{
		rotate_camera(&game->player.cam, -15);
	}
	render_scene(game);
	print_position(&game->player);
	return (0);
}

static int	key_release_hook(int keysym, void *mlx_ctx)
{
	if (keysym == XK_Escape)
	{
		mlx_loop_end(mlx_ctx);
	}
	return (0);
}

static int	expose_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	render_scene(game);
	return (0);
}

static int	init_hooks(t_game *game)
{
	mlx_hook(game->win1.win_ctx, KeyPress, KeyPressMask, key_press_hook, game);
	mlx_key_hook(game->win1.win_ctx, key_release_hook, game->mlx_ctx);
	mlx_hook(game->win1.win_ctx, DestroyNotify, 0, mlx_loop_end, game->mlx_ctx);
	mlx_expose_hook(game->win1.win_ctx, expose_hook, game);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	init_hooks(&game);
	render_scene(&game);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
