#include "game.h"
#include "get_next_line.h"
#include "libft.h"


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	fd = open(scene_path, O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	if (line == NULL)
	{
		print_error(1, "INVALID Map. Map is empty.");
		return (1);
	}
	while (line != NULL && game->cfg.done == 0) //looping over scene to retrieve 6 elements
	{	
		tmp = skip_whitespace(line);
		if (*tmp == '\n')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if(extract_param(&game->cfg, line) != 0)
			return (free(line), 1);
		free(line);	
		line = get_next_line(fd);
	}

	int rows = 0;
	int len = 0;
	int maxlen = 0;	// variable to keep track of the longest map line

	reset_map(&game->map);

	// NOTE(ismail): get_next_line returns a line with \n included
	// so the length is + 1 byte except when we are on the very last line.
	// Therefore, my current fix is wrong because maxlen will possibly
	// be 1 more than the actual width.
	// Also need to fix get_next_line memory leaks.

	char *pos_nl;
	while (line != NULL)
	{
		tmp = skip_whitespace(line);
		if (*tmp == '\n')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		pos_nl = ft_strchr(line, '\n');
		if (pos_nl != NULL)
			line[pos_nl - line] = '\0';
		len = strnlen(line, MAX_GRID_COL - 1);
		if (len > maxlen)
		{
			maxlen = len;
		}
		ft_memcpy(game->map.grid[rows], line, len + 1);
		free(line);
		/*game->map.grid[i] = ft_substr(line, 0, ft_strlen(line));
		if (game->map.grid[i] == NULL)
			return (1); 					//free here the grid already allocated*/
		line = get_next_line(fd);
		rows++;
	}
	game->map.width = maxlen;
	game->map.height = rows;
	printf("maxlen: %d\n", maxlen);
	close(fd);
	return (0);
}

void	reset_config(t_config *config)
{
	config->NO = NULL;
	config->SO = NULL;
	config->WE = NULL;
	config->EA = NULL;
	config->ceiling.blue = -1;
	config->ceiling.green = -1;
	config->ceiling.red = -1;
	config->floor.blue = -1;
	config->floor.green = -1;
	config->floor.red = -1;
	config->done = 0;
}

int	get_scene(t_game *game, char *scene_path)
{
	if (check_cub_format(scene_path) != 0)
		return (1);
	reset_config(&game->cfg);
	if (read_scene(game, scene_path) != 0)
		return (1);
	//print_config(game->cfg);
	//print_map(game->map);
	if (validate_map(game) != 0)
		return (1);
	return (0);
}
