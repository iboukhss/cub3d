#include "cub3d.h"
#include "get_next_line.h"
#include "libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*skip_whitespace(char *line)
{
	while(*line != '\n' && ft_strchr(WHITESPACE, *line) != NULL)
		line++;
	return (line);
}

char	*get_element(char *line, char **element)
{
	char *bgn;

	bgn = line;
	while(*line != '\n' && ft_strchr(WHITESPACE, *line) == NULL
		&& ft_strchr("\n", *line) == NULL)
		line++;
	*element = ft_substr(bgn, 0, (line - bgn));
	if (!element)
		return (NULL);
	return (line);	
}

int	validate_identifier(char *type_identifier)
{
	static const char *identifiers[7] = {"NO", "SO", "WE", "EA", "F", "C", NULL};
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(type_identifier);
	while (identifiers[i] != NULL)
	{
		if (ft_strncmp(type_identifier, identifiers[i], len) == 0)
			return (0);
		i++;
	}
	return(1);
}

//QUESTION : is 91   ,    45,0 a valid input ?

t_rgb *get_rgb(char *color_code)
{
	t_rgb	*rgb;
	char	**split;
	size_t	i;
	
	i = 0;
	if (color_code == NULL)
		return (NULL);
	rgb = malloc(sizeof(t_rgb));
	if (!rgb)
		return (NULL);
	split = ft_split(color_code, ',');
	if (!split)
		return (free(rgb), NULL);
	rgb->red = ft_atoi(split[0]);
	rgb->green = ft_atoi(split[1]);
	rgb->blue = ft_atoi(split[2]);
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (rgb);
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
		config->floor = get_rgb(information);
	else if (*type_identifier == 'C')
		config->ceiling = get_rgb(information);
	else
		return (1);
	if (config->NO != NULL && config->SO != NULL && config->EA != NULL && config->WE != NULL
		&& config->ceiling != NULL && config->floor != NULL)
			config->done = 1;
	return (0);
}

int extract_param(t_config *config, char *line)
{
	char	*type_identifier;
	char	*information;

	line = skip_whitespace(line);
	if (*line == '\n') //empty line -> whitespaces to be defined. Currently only skipping ASCII 20
		return (0);
	line = get_element(line, &type_identifier);
	if (!type_identifier)
		return (1);
	if (validate_identifier(type_identifier) != 0)
		return (free(type_identifier), 1);
	line = skip_whitespace(line);
	if (*line == '\n')
		return (print_error(1, "INVALID information. Path or rgb code missing after identifier"), 1); //check if we cannot return the file row on which the error occurs
	line = get_element(line, &information);
	if (!information)
		return (free(type_identifier), 1);
	if (update_config(config, type_identifier, information) != 0)
		return (free(type_identifier), 1);
	return (free(type_identifier), 0);
}
