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
int parse_map(t_game *game, t_map *map)
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
	char current;

	if (x < 0 || x >= map->width || y < 0 || y >= map->height)
	{
		print_error(1, "Invalid Map. Flood-fill outside map bounds.");
		return (1); // Out of bounds — leak
	}
	current = map->grid[y][x];
	if (current == ' ' || current == '\0' || current == '\n')
	{
		print_error(1, "Invalid Map. Not enclosed by walls");
		return (1); // Not enclosed — leak
	}
	if (current == '1' || current == 'X')
		return (0); // Wall or already visited — valid

	if (!(current == '0' || current == 'N' || current == 'S' || current == 'E' || current == 'W'))
	{
		print_error(1, "Invalid Map. Invalid char found.");
		return (1); // Invalid character
	}
	map->grid[y][x] = 'X'; // Mark as visited
	// Propagate any error (short-circuit on first failure)
	if (flood_fill(map, x + 1, y) != 0)
		return (1);
	if (flood_fill(map, x - 1, y) != 0)
		return (1);
	if (flood_fill(map, x, y + 1) != 0)
		return (1);
	if (flood_fill(map, x, y - 1) != 0)
		return (1);
	return (0); // Fully enclosed and valid
}

int	validate_map(t_game *game)
{
	if (parse_map(game, &game->map) != 0)
		return (1);
	clone_map(&game->map, &game->tmp_map);
	if (flood_fill(&game->tmp_map, game->player.start_x, game->player.start_y) != 0)
	{
		return (1);
	}
	return (0);
}
