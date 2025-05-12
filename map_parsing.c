/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:31:48 by dennis            #+#    #+#             */
/*   Updated: 2025/05/12 21:34:58 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "get_next_line.h"

int	validate_point(t_game *game, char pt, int y, int x)
{
	if ((game->map.spawn_orientation != '\0')
		&& (pt == 'N' || pt == 'S' || pt == 'E' || pt == 'W'))
	{
		print_error(1, "Invalid Map. Multiple spawn positions.");
		return (1);
	}
	else if (pt == 'N' || pt == 'S' || pt == 'E' || pt == 'W')
		set_player_pos(game, pt, y, x);
	else if (pt != '1' && pt != '0' && pt != ' ' && pt != '\0')
	{
		print_error(1, "Invalid Map. Invalid char found.");
		return (1);
	}
	return (0);
}

//check that no invalid char is in the map
int	parse_map(t_game *game, t_map *map)
{
	int	row;
	int	col;

	row = 0;
	while (row < map->height)
	{
		col = 0;
		while (col < map->width)
		{
			if (validate_point(game, map->grid[row][col], row, col) != 0)
			{
				return (1);
			}
			col++;
		}
		row++;
	}
	if (game->map.spawn_orientation == '\0')
	{
		print_error(1, "Invalid Map. Missing spawn point.");
		return (1);
	}
	return (0);
}

int	flood_fill(t_map *map, int x, int y)
{
	char	current;

	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
		return (print_error(1, "Invalid Map. Out of bounds."), 1);
	current = map->grid[y][x];
	if (current == ' ' || current == '\0' || current == '\n')
		return (print_error(1, "Invalid Map. Not enclosed by walls"), 1);
	if (current == '1' || current == 'X')
		return (0);
	if (!(current == '0' || current == 'N' || current == 'S'
			|| current == 'E' || current == 'W'))
		return (print_error(1, "Invalid Map. Invalid char found."), 1);
	map->grid[y][x] = 'X';
	if (flood_fill(map, x + 1, y) != 0)
		return (1);
	if (flood_fill(map, x - 1, y) != 0)
		return (1);
	if (flood_fill(map, x, y + 1) != 0)
		return (1);
	if (flood_fill(map, x, y - 1) != 0)
		return (1);
	return (0);
}

int	validate_map(t_game *game)
{
	if (parse_map(game, &game->map) != 0)
		return (1);
	clone_map(&game->map, &game->tmp_map);
	if (flood_fill(&game->tmp_map, game->player.start_x,
			game->player.start_y) != 0)
	{
		return (1);
	}
	return (0);
}
