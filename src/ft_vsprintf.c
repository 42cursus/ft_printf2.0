/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 05:11:22 by abelov            #+#    #+#             */
/*   Updated: 2025/01/19 05:11:23 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/param.h>
#include "ft_printf.h"

int	ft_vsprintf(char *str, const char *format, va_list ap)
{
	int	result;

	result = ft_vsnprintf(NULL, 0, format, ap);
	if (result < 0)
		return (FT_ERROR);
	return (ft_vsnprintf(str, (size_t)result + 1, format, ap));
}

char	*ft_strchrnul(const char *s, int c)
{
	while (*s)
	{
		if ((unsigned char) *s == (unsigned char) c)
			break ;
		s++;
	}
	return ((char *)s);
}

size_t	copy_as_is(size_t size, const char **fmt, char **buf)
{
	const char *const	ptr = ft_strchrnul((*fmt), '%');
	const size_t		i = ptr - (*fmt);
	char *const			to_put = ft_strndup((*fmt), i);
	size_t				desired;

	desired = MIN(i + FT_TERMINATOR, size);
	ft_memcpy((*buf), to_put, desired * sizeof(char));
	free(to_put);
	(*fmt) = (ptr - 1);
	if (desired > 1)
		desired--;
	if (*buf)
		(*buf) += desired;
	return (i);
}

int	ft_vsnprintf_internal(char **str, size_t size, const t_snprintf_f *lut,
							va_list *ap)
{
	size_t			ret;
	register char	c;
	char			*seek;
	const char		*fmt = str[0];
	char *const		buf = str[1];

	seek = buf;
	ret = 0;
	c = *fmt;
	while (c != '\0')
	{
		size_t leftover = size - (seek - buf);
		if (c == '%')
		{
			c = *++fmt;
			if (c == '\0')
				fmt--;
			else
			{
				if (lut[(u_char) c] != NULL)
					ret += lut[(u_char) c](&seek, leftover, ap);
				else
				{
					ret = -1;
					break;
				}
			}
		}
		else
			ret += copy_as_is(leftover, &fmt, &seek);
		c = *++fmt;
	}
	return ((int)ret);
}

int	ft_vsnprintf(char *dst, size_t size, const char *fmt, va_list ap)
{
	int					ret;
	size_t				mini_size;
	va_list				ap_save;
	static t_snprintf_f	lut[UCHAR_MAX] = {
	['c'] = ft_snprint_c,
	['%'] = ft_snprint_percent,
	['d'] = ft_snprint_d,
	['p'] = ft_snprint_p,
	['i'] = ft_snprint_d,
	['u'] = ft_snprint_u,
	['x'] = ft_snprint_x,
	['X'] = ft_snprint_upperx,
	['s'] = ft_snprint_s,
	};

	mini_size = size;
	if (size)
		mini_size--;
	va_copy(ap_save, ap);
	ret = ft_vsnprintf_internal((char *[]){(char *)fmt, dst},
			mini_size, lut, &ap_save);
	if (size)
		dst[MIN(mini_size, (u_int)ret)] = '\0';
	return (ret);
}
