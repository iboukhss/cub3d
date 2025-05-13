/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_image.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:38:26 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/13 18:38:36 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

#include <stdio.h>

int	create_image(t_image *img, int width, int height)
{
	img->width = width;
	img->height = height;
	img->img_ctx = mlx_new_image(img->mlx_ctx, width, height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
	return (0);
}

int	destroy_image(t_image *img)
{
	if (!img->mlx_ctx || !img->img_ctx)
	{
		return (1);
	}
	mlx_destroy_image(img->mlx_ctx, img->img_ctx);
	img->img_ctx = NULL;
	return (0);
}

int	load_xpm_data(t_image *img, char **xpm_data, void *mlx_ctx)
{
	img->img_ctx = mlx_xpm_to_image(mlx_ctx, xpm_data, &img->width,
			&img->height);
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
	return (0);
}

int	load_xpm_file(t_image *img, char *filename)
{
	img->img_ctx = mlx_xpm_file_to_image(img->mlx_ctx, filename, &img->width,
			&img->height);
	if (!img->img_ctx)
	{
		printf("Error\nFailed to load XPM image '%s'\n", filename);
		return (1);
	}
	img->addr = mlx_get_data_addr(img->img_ctx, &img->bits_per_pixel,
			&img->bytes_per_line, &img->endian);
	return (0);
}
