/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   element_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:34:06 by dennis            #+#    #+#             */
/*   Updated: 2025/05/10 10:58:16 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "get_next_line.h"
#include "libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	validate_identifier(char *type_identifier)
{
	static const char	*identifiers[7] = {"NO",
		"SO", "WE", "EA", "F", "C", NULL};
	size_t				i;
	size_t				len;

	i = 0;
	len = ft_strlen(type_identifier);
	if (ft_strchr(TABS, *type_identifier) != NULL)
	{
		print_error(1, "Invalid scene. Only spaces allowed before type identifier.");
		return (1);
	}
	while (identifiers[i] != NULL)
	{
		if (ft_strncmp(type_identifier, identifiers[i], len) == 0)
			return (0);
		i++;
	}
	print_error(1, "Invalid Scene. Invalid type identifier.");
	return (1);
}

int validate_color(char *nbr, uint8_t *color)
{
	int i;
	int	color_nbr;
	
	if (!nbr || !color)
		return (1);
	i = 0;
	while (nbr[i] != '\0')
	{
		if (nbr[i] != ' ' && ft_isdigit(nbr[i]) != 1)
			return (2);
		i++;
	}
	color_nbr = ft_atoi(nbr);
	if (color_nbr < 0 || color_nbr > 255)
		return (3);
	*color = (uint8_t)color_nbr;
	return (0);
}

int	get_rgb(char *color_code, uint32_t *color)
{
	char	**split;
	uint8_t	red;
	uint8_t	green;
	uint8_t	blue;
	size_t	i;

	i = 0;
	if (color_code == NULL)
		return (1);
	split = ft_split(color_code, ',');
	if (!split)
		return (print_error(1, "Failed to split rgb code"), 1);
	if (validate_color(split[0], &red) != 0 ||
		validate_color(split[1], &green) != 0 ||
		validate_color(split[2], &blue) != 0)
	{
		free(color_code);
		print_error(1, "Invalid scene. RGB code invalid.");
		return (1);
	}	
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
	*color = rgb_to_hex(red, green, blue);
	free(color_code);
	return (0);
}

int	update_config(t_config *config, char *type_identifier, char *information)
{
	if (ft_strncmp(type_identifier, "NO", 2) == 0)
		config->NO = information;
	else if (ft_strncmp(type_identifier, "SO", 2) == 0)
		config->SO = information;
	else if (ft_strncmp(type_identifier, "WE", 2) == 0)
		config->WE = information;
	else if (ft_strncmp(type_identifier, "EA", 2) == 0)
		config->EA = information;
	else if (*type_identifier == 'F')
	{
		if (get_rgb(information, &config->floor_color) != 0)
			return (1);
	}
	else if (*type_identifier == 'C')
	{
		if (get_rgb(information, &config->ceil_color) != 0)
			return (1);
	}
	else
		return (1);
	if (config->NO != NULL && config->SO != NULL && config->EA != NULL
		&& config->WE != NULL && config->ceil_color != COLOR_UNSET
		&& config->floor_color != COLOR_UNSET)
		config->done = 1;
	return (0);
}

int	extract_param(t_config *config, char *line)
{
	char	*type_identifier;
	char	*information;

	line = skip_whitespace(line);
	if (*line == '\n')
		return (0);
	line = get_element(line, &type_identifier);
	if (!type_identifier)
		return (1);
	if (validate_identifier(type_identifier) != 0)
		return (free(type_identifier), 1);
	line = skip_whitespace(line);
	if (*line == '\n')
		return (free(type_identifier), 1);
	if (*type_identifier == 'C' || *type_identifier == 'F')
		line = get_rgb_code(line, &information);
	else
		line = get_element(line, &information);
	if (!information)
		return (free(type_identifier), 1);
	if (update_config(config, type_identifier, information) != 0)
		return (free(type_identifier), free(information), 1);
	return (free(type_identifier), 0);
}
