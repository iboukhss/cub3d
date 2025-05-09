/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:24:43 by dennis            #+#    #+#             */
/*   Updated: 2025/05/09 16:31:54 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *s)
{
	int	len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*create_line(char **remainder)
{
	char	*line;
	char	*tmp;
	char	*pos;

	if (*remainder == NULL || **remainder == '\0')
		return (NULL);
	pos = ft_strchr(*remainder, '\n');
	if (pos)
	{
		line = ft_substr(*remainder, 0, (pos - *remainder) + 1);
		tmp = ft_strdup(pos + 1);
		free(*remainder);
		*remainder = tmp;
		return (line);
	}
	line = *remainder;
	*remainder = NULL;
	return (line);
}

char	*read_file(int fd, char **remainder)
{
	char	*buffer;
	char	*tmp;
	int		bytes;

	if (*remainder == NULL)
		*remainder = ft_strdup("\0");
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (ft_strchr(*remainder, '\n') == NULL)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free(buffer), NULL);
		else if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		tmp = ft_strjoin(*remainder, buffer);
		free(*remainder);
		*remainder = tmp;
	}
	free(buffer);
	return (*remainder);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*remainder;

	line = NULL;
	if (fd < 0 || fd > 1024 || BUFFER_SIZE < 0)
		return (NULL);
	remainder = read_file(fd, &remainder);
	if (remainder == NULL)
	{
		return (NULL);
	}
	line = create_line(&remainder);
	if (remainder && *remainder == '\0')
	{
		free(remainder);
		remainder = NULL;
	}
	return (line);
}
