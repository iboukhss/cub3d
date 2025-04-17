/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dheck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:24:56 by dheck             #+#    #+#             */
/*   Updated: 2024/03/15 13:27:35 by dheck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_nbr_words(char const *s, char c);
static int		ft_feed(char const *s, char c, size_t words, char **array_ptr);
static void		free_array(char **array_ptr, size_t word_count);

char	**ft_split(char const *s, char c)
{	
	size_t	word_count;
	char	**array_ptr;

	if (s == NULL)
		return (NULL);
	word_count = ft_nbr_words(s, c);
	array_ptr = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (array_ptr == NULL)
		return (NULL);
	array_ptr[word_count] = NULL;
	if (ft_feed(s, c, word_count, array_ptr) == 0)
	{
		free (array_ptr);
		return (NULL);
	}
	else
	{
		return (array_ptr);
	}	
}

static int	ft_feed(char const *s, char c, size_t words, char **array_ptr)
{
	size_t	index;
	size_t	start;
	size_t	end;
	size_t	i;

	index = 0;
	i = 0;
	while (s[i] != 0 && index < words)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] != c && s[i] != 0)
			i++;
		end = i;
		array_ptr[index] = malloc(sizeof(char) * (end - start + 1));
		if (array_ptr[index] == NULL)
		{
			free_array(array_ptr, words);
			return (0);
		}
		ft_strlcpy(array_ptr[index], s + start, end - start + 1);
		index++;
	}
	return (1);
}

static void	free_array(char **array_ptr, size_t word_count)
{
	size_t	index;

	index = 0;
	while (index < word_count)
	{
		free(array_ptr[index]);
		index++;
	}
}

static size_t	ft_nbr_words(char const *s, char c)
{
	size_t	i;
	size_t	word_count;
	size_t	start;
	size_t	end;

	i = 0;
	word_count = 0;
	start = 0;
	end = 0;
	while (s[i] != 0)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] != c && s[i] != 0)
			i++;
		end = i;
		if (start != end)
			word_count++;
	}
	return (word_count);
}
