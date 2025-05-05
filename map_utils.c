#include "game.h"

void	set_player_pos(t_game *game, char orientation, int y, int x)
{
	game->map.spawn_orientation = orientation;
	game->player.spawn_orientation = orientation;
	game->player.start_x = x;
	game->player.start_y = y;
}

// NOTE(ismail): Make sure the grid is filled with zeroes before processing.
int	reset_map(t_map *map)
{
	for (int y = 0; y < MAX_GRID_ROW; y++)
	{
		for (int x = 0; x < MAX_GRID_COL; x++)
		{
			map->grid[y][x] = '\0';
		}
	}
	map->spawn_orientation = '\0';
	return (0);
}

void clone_map(t_map *src, t_map *dest)
{
	int height = src->height;
	int width = src->width;
	int i = 0;

	while (i < height)
	{
		int j = 0;
		while (j < width)
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
	while (i < MAX_GRID_ROW)
	{
		int j = 0;
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
