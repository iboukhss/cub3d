/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:49:08 by dennis            #+#    #+#             */
/*   Updated: 2025/05/09 15:51:10 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	size_t	len;

	len = ft_strlen(scene_path);
	if (len >= 4 && ft_strncmp(".cub",
			(const char *)&scene_path[len - 4], 4) == 0)
	{
		return (0);
	}
	else
	{
		print_error(1, "Invalid format.Scene not in .cub format");
		return (1);
	}
	return (1);
}

int	handle_nl(char *line)
{
	char	*tmp;

	tmp = skip_whitespace(line);
	if (*tmp == '\n')
	{
		return (1);
	}
	return (0);
}

int	update_width(t_game *game, char *line)
{
	char	*pos_nl;
	int		len;

	pos_nl = ft_strchr(line, '\n');
	if (pos_nl != NULL)
		line[pos_nl - line] = '\0';
	len = strnlen(line, MAX_GRID_COL - 1);
	if (len > game->map.width)
	{
		game->map.width = len;
	}
	return (len);
}

int	read_scene(t_game *game, char *scene_path)
{
	int		fd;
	char	*line;

	fd = open(scene_path, O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	if (line == NULL)
		return (print_error(1, "INVALID Map. Map is empty."), 1);
	while (line != NULL)
	{
		if (handle_nl(line) == 0)
		{
			if (game->cfg.done == 1)
				ft_memcpy(game->map.grid[game->map.height++],
					line, update_width(game, line) + 1);
			else if (game->cfg.done == 0
				&& extract_param(&game->cfg, line) != 0)
				return (free(line), 1);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

int	get_scene(t_game *game, char *scene_path)
{
	if (check_cub_format(scene_path) != 0)
		return (1);
	reset_config(&game->cfg);
	reset_map(&game->map);
	if (read_scene(game, scene_path) != 0)
		return (1);
	if (validate_map(game) != 0)
		return (1);
	return (0);
}
