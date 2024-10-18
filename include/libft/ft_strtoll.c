/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:56:05 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/18 18:56:58 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ignore_start(const char *nptr, int *sign, int *base)
{
	int	i;

	*sign = 1;
	i = 0;
	while (nptr[i] && ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || (nptr[i] == '+' && i++))
		*sign = ((nptr[i - 1] == '-') * -1) + ((nptr[i - 1] == '+') * 1);
	if (*base == 0)
	{
		if (ft_strncmp(nptr, "0x", 2) && ft_strncmp(nptr, "0X", 2))
			*base = 10;
		else if (nptr[0] == '0')
			*base = 8;
	}
	if (!ft_strncmp(nptr, "0x", 2) || !ft_strncmp(nptr, "0X", 2))
		i += 2;
	return (i);
}

static char get_base_rank(char c, int base)
{
	char	res;

	res = -1;
	if (ft_isdigit(c))
		res = c - '0';
	if (ft_isalpha(c) && c >= 'a' && c <= 'z')
		res = 10 + (c - 'a');
	if (ft_isalpha(c) && c >= 'A' && c <= 'Z')
		res = 10 + (c - 'A');
	if (res >= base)
		res = -1;
	return (res);
}

static long long stop_fun(long long res, const char *end, char **endptr)
{
	if (endptr)
		*endptr = (char *)end;
	return (res);
}

long long	ft_strtoll(const char *nptr, char **endptr, int base)
{
	long long	res;
	int			sign;
	char		tmp;

	if (!nptr || ((base != 0 && base < 2) || base > 36))
	{
		if (endptr)
			*endptr = NULL;
		return (0);
	}
	nptr += ignore_start(nptr, &sign, &base);
	res = 0;
	while (nptr)
	{
		tmp = get_base_rank(nptr[0], base);
		if (tmp == -1)
			return (stop_fun(res, nptr, endptr));
		res = res * base + tmp;
		nptr++;
	}
	return (stop_fun(res, nptr, endptr));
}
#include <stdio.h>
int main(int ac, char **av)
{
	(void)ac;
	char *nb = av[1];
	int base = atoi(av[2]);
	char *end1 = NULL;
	char *end2 = NULL;
	long long res1;
	long long res2;
//need to fix 09AZB 0
	res1 = strtoll(nb, &end1, base);
	printf("res1 = %lld, end1 = <%s>\n", res1, end1);
	res2 = ft_strtoll(nb, &end2, base);
	printf("res2 = %lld, end2 = <%s>\n", res2, end2);
}
