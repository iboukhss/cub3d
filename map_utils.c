#include "cub3d.h"
#include "get_next_line.h"
#include "libft.h"


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void print_config(t_config config)
{
	printf("NO -> %s\n", config.NO);
	printf("SO -> %s\n", config.SO);
	printf("WE -> %s\n", config.WE);
	printf("EA -> %s\n", config.EA);
	//printf("F -> %s\n", config.NO);
	//printf("C -> %s\n", config.NO);
}

int	check_cub_format(char *scene_path)
{
	size_t len;

	len = ft_strlen(scene_path);
	if (len >= 4 && ft_strncmp(".cub",
			(const char *)&scene_path[len - 4], 4) == 0)
	{
		printf("correct file format !\n");
		return (0);
	}
	else
	{
		print_error(1, "Scene should be in .cub format");
		return (1);
	}
	return (1);
}

int	get_scene(t_game *game, char *scene_path)
{
	if (check_cub_format(scene_path) != 0)
		return (1);
	if (read_scene(&game->config, scene_path) != 0)
		return (1);
	print_config(game->config);
	return (0);
}


