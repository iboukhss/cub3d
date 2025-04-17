#include "game.h"

//needed for flood fill
/*int	init_tmp_map(t_data *data)
{
	int	row;
	int	col;

	row = -1;
	data->map->tmp_map = malloc(sizeof(t_map));
	if (data->map->tmp_map == NULL)
		return (0);
	while (row++ < data->map->rows)
	{
		data->map->tmp_map->grid[row] = malloc(
				sizeof(char) * (data->map->cols + 1));
		if (data->map->tmp_map->grid[row] == NULL)
		{
			data->map->tmp_map->rows = row;
			return (free_grid(data->map->tmp_map), 0);
		}
		col = -1;
		while (col++ < data->map->cols)
		{
			data->map->tmp_map->grid[row][col] = '0';
			data->map->tmp_map->cols = col;
		}
	}
	data->map->tmp_map->rows = row;
	return (1);
}*/
/*
int	read_map(t_map *map, char *line)
{
	map->grid[i] = ft_substr(line, 0, ft_strlen(line));

	return (0);
}*/
