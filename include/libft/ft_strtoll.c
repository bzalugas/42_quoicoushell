/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:56:05 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/21 17:32:37 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>

static int	ignore_start(const char *nptr, int *sign, int *base, char **endptr)
{
	int	i;

	if (endptr)
		*endptr = (char *)nptr;
	*sign = 1;
	i = 0;
	while (nptr[i] && ft_isspace(nptr[i]))
		i++;
	if ((nptr[i] == '-' || nptr[i] == '+') && ++i)
		*sign = ((nptr[i - 1] == '-') * -1) + ((nptr[i - 1] == '+') * 1);
	if (*base == 0)
	{
		if (!ft_strncmp(nptr, "0x", 2) || !ft_strncmp(nptr, "0X", 2))
			*base = 16;
		else if (nptr[0] == '0')
			*base = 8;
		else
			*base = 10;
	}
	if ((!ft_strncmp(nptr, "0x", 2) || !ft_strncmp(nptr, "0X", 2))
		&& *base == 16)
		i += 2;
	return (i);
}

static char	get_base_rank(char c, int base)
{
	char	res;

	res = -1;
	if (ft_isdigit(c))
		res = c - '0';
	else if (ft_isalpha(c) && c >= 'a' && c <= 'z')
		res = 10 + (c - 'a');
	else if (ft_isalpha(c) && c >= 'A' && c <= 'Z')
		res = 10 + (c - 'A');
	if (res >= base)
		res = -1;
	return (res);
}

static void	set_endptr(const char *nptr, char **endptr, int base)
{
	if (!endptr)
		return ;
	while (nptr && *nptr)
	{
		if (get_base_rank(*nptr, base) == -1)
			break ;
		nptr++;
	}
	if (nptr > *endptr + 1)
		*endptr = (char *)nptr;
}

static long long	stop_fun(long long res, int sign)
{
	if (sign > 0 && res > LLONG_MAX / sign)
		return (LLONG_MAX);
	if (res != 0 && sign < 0 && sign < LLONG_MIN / res)
		return (LLONG_MIN);
	return (res * sign);
}

long long	ft_strtoll(const char *nptr, char **endptr, int base)
{
	long long	res;
	int			sign;
	char		tmp;

	if (endptr)
			*endptr = NULL;
	if (!nptr || ((base != 0 && base < 2) || base > 36))
		return (0);
	nptr += ignore_start(nptr, &sign, &base, endptr);
	set_endptr(nptr, endptr, base);
	res = 0;
	while (nptr && *nptr)
	{
		tmp = get_base_rank(nptr[0], base);
		if (tmp == -1)
			return (stop_fun(res, sign));
		if (sign > 0 && res * base + tmp < res)
			return (errno = ERANGE, stop_fun(LLONG_MAX, 1));
		if (sign < 0 && res * base + tmp - 1 < res)
			return (errno = ERANGE, stop_fun(LLONG_MIN, -1));
		res = res * base + tmp;
		nptr++;
	}
	return (stop_fun(res, sign));
}
