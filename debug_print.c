#include "game.h"

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
	while (i < MAX_GRID_ROW && map.grid[i] != NULL)
	{
		printf("%s", map.grid[i]);
		i++;
	}
}

void    print_error(int default_prompt, char* error_msg)
{
    if (default_prompt == 1)
        printf("Error\n");
    if (error_msg != NULL)
        printf("%s\n", error_msg);   
}
