#include "game.h"

int	load_image(t_game *game, t_image *texture_img, char *path)
{
	texture_img->img_ctx = mlx_xpm_file_to_image(game->mlx_ctx, path, 
		&texture_img->width, &texture_img->height);
	if (texture_img->img_ctx == NULL)
	{
		print_error(1, "mlx_xpm_file_to_image FAILED.");
		return (1);
	}
	texture_img->addr = mlx_get_data_addr(texture_img->img_ctx,
		&texture_img->bits_per_pixel, &texture_img->bytes_per_line, &texture_img->endian);
	return (0);
}

int	load_all_textures(t_game *game)
{
	if (!game || !game->mlx_ctx)
	{
		print_error(1, "load_image: NULL game or uninitialized mlx_ctx.");
		return (1);
	}
	if (load_image(game, &game->cfg.texture_NO, game->cfg.NO) != 0)
		return (1);
	if (load_image(game, &game->cfg.texture_SO, game->cfg.SO) != 0)
		return (1);
	if (load_image(game, &game->cfg.texture_EA, game->cfg.EA) != 0)
		return (1);
	if (load_image(game, &game->cfg.texture_WE, game->cfg.WE) != 0)
		return (1);
	return (0);
}
