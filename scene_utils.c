#include "game.h"
#include "get_next_line.h"
#include "libft.h"


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

int	read_scene(t_game *game, char *scene_path)
{
	int		fd;
	char	*line;
	char	*tmp;
	size_t	i;

	i = 0;
	fd = open(scene_path, O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	if (line == NULL)
	{
		print_error(1, "INVALID Map. Map is empty.");
		return (1);
	}
	while (line != NULL && game->config.done == 0) //looping over scene to retrieve 6 elements
	{	
		tmp = skip_whitespace(line);
		if (*tmp == '\n')
			continue ;
		if(extract_param(&game->config, line) != 0)
			return (1);
		line = get_next_line(fd);
	}

	while (line != NULL)
	{
		tmp = skip_whitespace(line);
		if (*tmp == '\n')
			continue ;
		game->map.grid[i] = ft_substr(line, 0, ft_strlen(line));
		if (game->map.grid[i] == NULL)
			return (1); 					//free here the grid already allocated
		line = get_next_line(fd);
		i++;
	}
	close(fd);
	return (0);
}

void	reset_config_map(t_config *config, t_map *map)
{
	size_t	i;

	i = 0;
	config->NO = NULL;
	config->SO = NULL;
	config->WE = NULL;
	config->EA = NULL;
	config->ceiling = NULL;
	config->floor = NULL;
	config->done = 0;
	while (i < MAX_GRID_SIZE)
	{
		map->grid[i] = NULL;
		i++;
	}
}

int	get_scene(t_game *game, char *scene_path)
{
	if (check_cub_format(scene_path) != 0)
		return (1);
	reset_config_map(&game->config, &game->map);
	if (read_scene(game, scene_path) != 0)
		return (1);
	print_config(game->config);
	print_map(game->map);
	return (0);
}


