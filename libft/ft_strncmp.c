/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dheck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:32:31 by dheck             #+#    #+#             */
/*   Updated: 2024/03/11 15:54:28 by dheck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t		i;
	int			diff;

	i = 0;
	diff = 0;
	if (n == 0)
		return (0);
	while (i < n && diff == 0)
	{
		diff = (unsigned char)s1[i] - (unsigned char)s2[i];
		i++;
	}
	return (diff);
}
