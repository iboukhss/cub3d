/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:32:25 by dennis            #+#    #+#             */
/*   Updated: 2025/05/13 16:19:59 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#include <stdlib.h>

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_image_path(t_config *cfg)
{
	if (!cfg)
		return ;
	if (cfg->no)
		free(cfg->no);
	if (cfg->ea)
		free(cfg->ea);
	if (cfg->so)
		free(cfg->so);
	if (cfg->we)
		free(cfg->we);
}

int	destroy_game(t_game *game)
{
	destroy_window(&game->main);
	destroy_window(&game->debug);
	destroy_image(&game->cfg.east_texture);
	destroy_image(&game->cfg.west_texture);
	destroy_image(&game->cfg.north_texture);
	destroy_image(&game->cfg.south_texture);
	free(game->cfg.ea);
	free(game->cfg.we);
	free(game->cfg.no);
	free(game->cfg.so);
	mlx_destroy_display(game->mlx_ctx);
	free(game->mlx_ctx);
	return (0);
}
