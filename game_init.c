/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 23:17:42 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/10 17:56:59 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>
#include <stdlib.h>

/*
// Default hardcoded map
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

static int	init_map(t_map *map)
{
	map->grid = (char **)g_testmap;
	map->width = 33;
	map->height = 14;
	return (0);
}*/

static int	init_player(t_player *player)
{
	player->start_x = 4;
	player->start_y = 12;
	player->spawn_orientation = ORIENT_NORTH;
	player->width = 0.5;
	return (0);
}

/* XPM */
static const char *const g_sample_xpm[] = {
"32 32 10 1",
" 	c None",
".	c #2020FF",
"+	c #8080FF",
"@	c #C0C0FF",
"#	c #FFFF00",
"$	c #FFFF80",
"%	c #FF2020",
"&	c #FF4848",
"*	c #FFFFDF",
"=	c #FFC0C0",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"        ..............          ",
"        .++++++++++++.          ",
"        .++++++++++++.          ",
"        .+@@+++++++++.          ",
"        .+@@+++++++++.          ",
"        .+@@+++++++++.          ",
"        .+@@++++++##############",
"        .+@@++++++#$$$$$$$$$$$$#",
"%%%%%%%%.+@@++++++#$$$$$$$$$$$$#",
"%&&&&&&&.+@@++++++#$**$$$$$$$$$#",
"%&&&&&&&.+@@++++++#$**$$$$$$$$$#",
"%&==&&&&.+++++++++#$**$$$$$$$$$#",
"%&==&&&&.+++++++++#$**$$$$$$$$$#",
"%&==&&&&..........#$**$$$$$$$$$#",
"%&==&&&&&&&&&%    #$**$$$$$$$$$#",
"%&==&&&&&&&&&%    #$**$$$$$$$$$#",
"%&==&&&&&&&&&%    #$**$$$$$$$$$#",
"%&==&&&&&&&&&%    #$$$$$$$$$$$$#",
"%&==&&&&&&&&&%    #$$$$$$$$$$$$#",
"%&&&&&&&&&&&&%    ##############",
"%&&&&&&&&&&&&%                  ",
"%%%%%%%%%%%%%%                  ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                "
};

static int	init_config(t_config *cfg, void *mlx_ctx)
{
	cfg->texture_EA.mlx_ctx = mlx_ctx;
	cfg->texture_WE.mlx_ctx = mlx_ctx;
	cfg->texture_NO.mlx_ctx = mlx_ctx;
	cfg->texture_SO.mlx_ctx = mlx_ctx;
	load_xpm_file(&cfg->texture_EA, cfg->EA);
	load_xpm_file(&cfg->texture_WE, cfg->WE);
	load_xpm_file(&cfg->texture_NO, cfg->NO);
	load_xpm_file(&cfg->texture_SO, cfg->SO);
	return (0);
}

// NOTE(ismail): Y-axis is inverted compared to the regular unit circle.
// Be mindful when dealing with rotations.
static int	init_camera(t_camera *cam, t_player player)
{
	cam->angle_deg = player.spawn_orientation;
	cam->angle_rad = rad(cam->angle_deg);
	cam->pos = vec2d_init(player.start_x + 0.5f, player.start_y + 0.5f);
	cam->dir = vec2d_init(cosf(cam->angle_rad), -sinf(cam->angle_rad));
	cam->plane = vec2d_init(sinf(cam->angle_rad), cosf(cam->angle_rad));
	return (0);
}

static int	init_main_window(t_window *win, t_game *game)
{
	win->mlx_ctx = game->mlx_ctx;
	win->win_ctx = NULL;
	win->frame.mlx_ctx = game->mlx_ctx;
	win->frame.img_ctx = NULL;
	win->width = WIN_WIDTH;
	win->height = WIN_HEIGHT;
	win->title = "cub3D";
	win->loop_hook = render_scene;
	win->key_press_hook = keypress_main;
	win->key_release_hook = keyrelease_main;
	win->button_press_hook = NULL;
	win->button_release_hook = NULL;
	win->motion_notify_hook = NULL;
	win->expose_hook = NULL;
	win->destroy_notify_hook = close_main_window;
	win->param = game;
	return (0);
}

static int	init_debug_window(t_window *win, t_game *game)
{
	win->mlx_ctx = game->mlx_ctx;
	win->win_ctx = NULL;
	win->frame.mlx_ctx = game->mlx_ctx;
	win->frame.img_ctx = NULL;
	win->width = game->map.width * TILE_SIZE;
	win->height = game->map.height * TILE_SIZE;
	win->title = "Map";
	win->loop_hook = NULL;
	win->key_press_hook = NULL;
	win->key_release_hook = keyrelease_debug;
	win->button_press_hook = NULL;
	win->button_release_hook = NULL;
	win->motion_notify_hook = NULL;
	win->expose_hook = NULL;
	win->destroy_notify_hook = close_debug_window;
	win->param = game;
	return (0);
}

int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	//init_map(&game->map);
	init_player(&game->player);
	init_camera(&game->player.cam, game->player);
	init_config(&game->cfg, game->mlx_ctx);
	init_main_window(&game->main, game);
	init_debug_window(&game->debug, game);
	return (0);
}

int	destroy_game(t_game *game)
{
	destroy_window(&game->main);
	destroy_window(&game->debug);
	destroy_image(&game->cfg.texture_EA);
	destroy_image(&game->cfg.texture_WE);
	destroy_image(&game->cfg.texture_NO);
	destroy_image(&game->cfg.texture_SO);
	free(game->cfg.EA);
	free(game->cfg.WE);
	free(game->cfg.NO);
	free(game->cfg.SO);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
	return (0);
}
