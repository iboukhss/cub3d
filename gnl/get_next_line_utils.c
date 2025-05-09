/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dennis <dennis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:23:11 by dennis            #+#    #+#             */
/*   Updated: 2025/05/09 16:35:44 by dennis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(char *s)
{
	char	*new_s;
	int		i;

	if (!s)
		return (NULL);
	new_s = malloc(ft_strlen(s) + 1);
	if (!new_s)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}

void	ft_memcpy(char *dst, char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*s3;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	s3 = malloc (len_s1 + len_s2 + 1);
	if (!s3)
		return (NULL);
	ft_memcpy(s3, s1, len_s1);
	ft_memcpy(s3 + len_s1, s2, len_s2);
	s3[len_s1 + len_s2] = '\0';
	return (s3);
}

char	*ft_strchr(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_substr(char *s, int start, int len)
{
	int		len_s;
	int		i;
	char	*new_s;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (len_s < start + len)
		len = len_s - start;
	new_s = malloc(len + 1);
	if (!new_s)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		new_s[i] = s[start + i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
