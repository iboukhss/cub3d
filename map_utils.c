/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:54:27 by dennis            #+#    #+#             */
/*   Updated: 2025/05/13 16:11:42 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	set_player_pos(t_game *game, char orientation, int y, int x)
{
	game->map.spawn_orientation = orientation;
	game->player.spawn_orientation = orientation;
	game->player.start_x = x;
	game->player.start_y = y;
	if (orientation == 'N')
		game->player.spawn_orientation = ORIENT_NORTH;
	else if (orientation == 'S')
		game->player.spawn_orientation = ORIENT_SOUTH;
	else if (orientation == 'W')
		game->player.spawn_orientation = ORIENT_WEST;
	else if (orientation == 'E')
		game->player.spawn_orientation = ORIENT_EAST;
	else
		game->player.spawn_orientation = ORIENT_UNKNOWN;
}

void	reset_config(t_config *config)
{
	config->no = NULL;
	config->so = NULL;
	config->we = NULL;
	config->ea = NULL;
	config->ceil_color = COLOR_UNSET;
	config->floor_color = COLOR_UNSET;
	config->done = 0;
}

// NOTE(ismail): Make sure the grid is filled with zeroes before processing.
int	reset_map(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAX_GRID_ROW)
	{
		j = 0;
		while (j < MAX_GRID_COL)
		{
			map->grid[i][j] = '\0';
			j++;
		}
		i++;
	}
	map->spawn_orientation = '\0';
	map->height = 0;
	map->width = 0;
	return (0);
}

void	clone_map(t_map *src, t_map *dest)
{
	int	i;
	int	j;

	i = 0;
	while (i < src->height)
	{
		j = 0;
		while (j < src->width)
		{
			dest->grid[i][j] = src->grid[i][j];
			j++;
		}
		while (j < MAX_GRID_COL)
		{
			dest->grid[i][j] = '\0';
			j++;
		}
		i++;
	}
	dest->height = src->height;
	dest->width = src->width;
	dest->spawn_orientation = src->spawn_orientation;
}
