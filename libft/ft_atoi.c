/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dheck <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:40:00 by dheck             #+#    #+#             */
/*   Updated: 2024/03/12 13:58:38 by dheck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_char_to_ignore(const char *nptr);

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 0;
	num = 0;
	while (ft_char_to_ignore(nptr + i) == 1)
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		sign = '-';
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = (num * 10) + (nptr[i] - '0');
		i++;
	}
	if (sign == '-')
		return (-num);
	return (num);
}

static int	ft_char_to_ignore(const char *nptr)
{
	if (*nptr == 9 || *nptr == 10 || *nptr == 11 || *nptr == 12 || *nptr == 13)
		return (1);
	else if (*nptr == 32)
		return (1);
	return (0);
}
