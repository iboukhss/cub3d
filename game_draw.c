/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_draw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:49:31 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/13 16:17:33 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static int	draw_tile(t_image *frame, int pos_x, int pos_y, uint32_t color)
{
	t_rect	tile;

	tile.x = pos_x * TILE_SIZE;
	tile.y = pos_y * TILE_SIZE;
	tile.w = TILE_SIZE;
	tile.h = TILE_SIZE;
	fill_rect(frame, tile, color);
	return (0);
}

// Draws the tile map
int	draw_map(t_image *frame, t_map map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map.height)
	{
		x = 0;
		while (x < map.width)
		{
			if (map.grid[y][x] == '1')
				draw_tile(frame, x, y, COLOR_WHITE);
			else if (map.grid[y][x] == '0')
				draw_tile(frame, x, y, COLOR_BLACK);
			else
				draw_tile(frame, x, y, COLOR_GRAY);
			x++;
		}
		y++;
	}
	return (0);
}

// Draws the player's position
int	draw_player(t_image *frame, t_player player)
{
	t_rect	hitbox;

	hitbox.x = (player.cam.pos.x - (PLAYER_WIDTH / 2)) * TILE_SIZE;
	hitbox.y = (player.cam.pos.y - (PLAYER_WIDTH / 2)) * TILE_SIZE;
	hitbox.w = PLAYER_WIDTH * TILE_SIZE;
	hitbox.h = PLAYER_WIDTH * TILE_SIZE;
	fill_rect(frame, hitbox, COLOR_RED);
	return (0);
}

int	draw_floor_and_ceiling(t_image *frame, t_config conf)
{
	t_rect	floor;
	t_rect	ceil;

	ceil.x = 0;
	ceil.y = 0;
	ceil.w = frame->width;
	ceil.h = frame->height / 2;
	floor.x = 0;
	floor.y = ceil.h;
	floor.w = frame->width;
	floor.h = frame->height - ceil.h;
	fill_rect(frame, ceil, conf.ceil_color);
	fill_rect(frame, floor, conf.floor_color);
	return (0);
}

int	draw_ray_on_minimap(t_game *game, t_ray ray)
{
	t_vec2d	p1;
	t_vec2d	p2;
	t_line	line;

	p1 = ray.pos;
	p2 = vec2d_add(ray.pos, vec2d_mul(ray.dir, ray.len));
	p1 = vec2d_mul(p1, TILE_SIZE);
	p2 = vec2d_mul(p2, TILE_SIZE);
	line.x0 = (int)p1.x;
	line.y0 = (int)p1.y;
	line.x1 = (int)p2.x;
	line.y1 = (int)p2.y;
	draw_line(&game->debug.frame, line, 0xFFFF00);
	return (0);
}
