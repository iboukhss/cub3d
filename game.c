/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:29:38 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:01:56 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <math.h>

static void	cast_ray(t_ray *ray, t_map map)
{
	struct s_dda_ctx	dda_ctx;

	init_dda(&dda_ctx, *ray);
	run_dda(ray, dda_ctx, map);
}

static int	do_raycasting(t_game *game)
{
	t_camera	cam;
	float		camera_x;
	t_ray		ray;
	int			x;

	cam = game->player.cam;
	x = 0;
	while (x < WIN_WIDTH)
	{
		camera_x = ((2.0f * x) / (float)WIN_WIDTH) - 1.0f;
		ray.pos = cam.pos;
		ray.dir = vec2d_add(cam.dir, vec2d_mul(cam.plane, camera_x));
		ray.side = ORIENT_UNKNOWN;
		cast_ray(&ray, game->map);
		draw_ray_on_minimap(game, ray);
		draw_wall_column(game, x, ray);
		x++;
	}
	return (0);
}

static int	refresh_frame(t_window *win, void *mlx_ctx)
{
	if (!win->win_ctx)
	{
		return (1);
	}
	mlx_put_image_to_window(mlx_ctx, win->win_ctx, win->frame.img_ctx, 0, 0);
	return (0);
}

int	render_scene(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	draw_map(&game->debug.frame, game->map);
	draw_player(&game->debug.frame, game->player);
	draw_floor_and_ceiling(&game->main.frame, game->cfg);
	do_raycasting(game);
	refresh_frame(&game->main, game->mlx_ctx);
	refresh_frame(&game->debug, game->mlx_ctx);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	if (argc != 2)
	{
		print_error(1, "Incorrect number of parameters");
		return (1);
	}
	if (get_scene(&game, argv[1]) != 0)
		return (1);
	print_error(0, "map ok\n");
	init_game(&game);
	create_window(&game.main);
	create_window(&game.debug);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
