/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 23:24:15 by iboukhss          #+#    #+#             */
/*   Updated: 2025/04/10 13:15:56 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>
#include <stdio.h>

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

static int	move_camera(t_camera *cam, float move_speed)
{
	t_vec2d	move;
	t_vec2d	dest;

	move = vec2d_scale(cam->dir, move_speed);
	dest = vec2d_add(cam->pos, move);
	cam->pos = dest;
	return (0);
}

static int	rotate_camera(t_camera *cam, int rot_speed)
{
	cam->angle_deg = (cam->angle_deg + rot_speed + 360) % 360;
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
		move_camera(&game->player.cam, 0.33f);
	}
	if (keysym == XK_Down)
	{
		move_camera(&game->player.cam, -0.33f);
	}
	if (keysym == XK_Left)
	{
		rotate_camera(&game->player.cam, 10);
	}
	if (keysym == XK_Right)
	{
		rotate_camera(&game->player.cam, -10);
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

int	init_hooks(t_game *game)
{
	mlx_hook(game->win1.win_ctx, KeyPress, KeyPressMask, key_press_hook, game);
	mlx_key_hook(game->win1.win_ctx, key_release_hook, game->mlx_ctx);
	mlx_hook(game->win1.win_ctx, DestroyNotify, 0, mlx_loop_end, game->mlx_ctx);
	mlx_expose_hook(game->win1.win_ctx, expose_hook, game);
	return (0);
}
