/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_element.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:35:28 by dennis            #+#    #+#             */
/*   Updated: 2025/05/10 10:40:42 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "libft.h"
#include "get_next_line.h"

char	*get_element(char *line, char **element)
{
	char	*bgn;

	bgn = line;
	while (*line != '\n' && ft_strchr(WHITESPACE, *line) == NULL)
	{
		if (ft_strchr(TABS, *line) != NULL)
		{
			*element = NULL;
			print_error(1, "Invalid scene. Only spaces allowed.");
			return (NULL);
		}
		line++;
	}
	*element = ft_substr(bgn, 0, (line - bgn));
	if (!(*element))
	{
		print_error(1, "Invalid Scene. Missing identifier or path.");
		return (NULL);
	}
	return (line);
}

char	*get_rgb_code(char *line, char **element)
{
	char	*bgn;

	bgn = line;
	while (*line != '\n')
	{
		if (ft_strchr(TABS, *line) != NULL)
		{
			*element = NULL;
			print_error(1, "Invalid scene. Only spaces allowed.");
			return (NULL);
		}
		line++;
	}
	*element = ft_substr(bgn, 0, (line - bgn));
	if (!(*element))
	{
		print_error(1, "Invalid Scene. Missing rgb code.");
		return (NULL);
	}
	return (line);
}

char	*skip_whitespace(char *line)
{
	while (*line != '\n' && ft_strchr(WHITESPACE, *line) != NULL)
		line++;
	return (line);
}
