/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:32:25 by dennis            #+#    #+#             */
/*   Updated: 2025/05/12 21:59:42 by dennis           ###   ########.fr       */
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
