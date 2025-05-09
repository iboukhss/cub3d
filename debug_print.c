/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:33:06 by dennis            #+#    #+#             */
/*   Updated: 2025/05/09 11:33:52 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>

void	print_config(t_config config)
{
	printf("NO -> %s\n", config.NO);
	printf("SO -> %s\n", config.SO);
	printf("WE -> %s\n", config.WE);
	printf("EA -> %s\n", config.EA);
	printf("F -> HEX %X\n", config.floor_color);
	printf("C -> HEX %X \n", config.ceil_color);
}

void	print_map(t_map map)
{
	int	i;

	i = 0;
	while (i < MAX_GRID_ROW && i < map.height)
	{
		printf("%s", map.grid[i]);
		i++;
	}
	printf("\n");
}

void	print_error(int default_prompt, char *error_msg)
{
	if (default_prompt == 1)
		printf("Error\n");
	if (error_msg != NULL)
		printf("%s\n", error_msg);
}
