/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:54:53 by iboukhss          #+#    #+#             */
/*   Updated: 2025/03/31 01:26:43 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#include <mlx.h>
#include <stdlib.h>

static int	init_game(t_game *game)
{
	game->mlx_ctx = mlx_init();
	game->win.width = WIN_WIDTH;
	game->win.height = WIN_HEIGHT;
	game->win.title = "cub3D";
	game->win.win_ctx = mlx_new_window(game->mlx_ctx, game->win.width, game->win.height, game->win.title);
	game->win.frame.width = game->win.width;
	game->win.frame.height = game->win.height;
	game->win.frame.img_ctx = mlx_new_image(game->mlx_ctx, game->win.frame.width, game->win.frame.height);
	game->win.frame.addr = mlx_get_data_addr(game->win.frame.img_ctx, &game->win.frame.bits_per_pixel, &game->win.frame.bytes_per_line, &game->win.frame.endian);
	return (0);
}

static int	destroy_game(t_game *game)
{
	mlx_destroy_image(game->mlx_ctx, game->win.frame.img_ctx);
	mlx_destroy_window(game->mlx_ctx, game->win.win_ctx);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
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

// Assuming ARGB32 (little endian) which means BGRA byte order.
// Source: https://en.wikipedia.org/wiki/RGBA_color_model#ARGB32
static int	draw_rect(t_image *img, int width, int height)
{
	for (int y = 0; y < height && y < img->height; y++)
	{
		char *row = img->addr + (y * img->bytes_per_line);
		for (int x = 0; x < width && x < img->width; x++)
		{
			char *pixel = row + (x * (img->bits_per_pixel / 8));
			((unsigned char *)pixel)[0] = 255;	// blue pixel
			((unsigned char *)pixel)[1] = 255;	// green pixel
			((unsigned char *)pixel)[2] = 255;	// red pixel
		}
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_game(&game);
	mlx_hook(game.win.win_ctx, DestroyNotify, 0, mlx_loop_end, game.mlx_ctx);
	mlx_key_hook(game.win.win_ctx, key_release_hook, game.mlx_ctx);
	draw_rect(&game.win.frame, 500, 500);
	mlx_put_image_to_window(game.mlx_ctx, game.win.win_ctx, game.win.frame.img_ctx, 0, 0);
	mlx_loop(game.mlx_ctx);
	destroy_game(&game);
	return (0);
}
