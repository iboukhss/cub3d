#include "cub3d.h"

#include <stdio.h>

void print_config(t_config config)
{
	printf("NO -> %s\n", config.NO);
	printf("SO -> %s\n", config.SO);
	printf("WE -> %s\n", config.WE);
	printf("EA -> %s\n", config.EA);
	printf("F -> RGB(%d,%d,%d)\n", config.floor->red, config.floor->green, config.floor->blue);
	printf("C -> RGB(%d,%d,%d)\n", config.ceiling->red, config.ceiling->green, config.ceiling->blue);
}

void print_map(t_map map)
{
	size_t	i;

	i = 0;
	while (i < MAX_GRID_SIZE && map.grid[i] != NULL)
	{
		printf("%s", map.grid[i]);
		i++;
	}
}
