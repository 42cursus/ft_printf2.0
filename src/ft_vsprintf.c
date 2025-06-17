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
	int		result;
	va_list	ap_save;

	va_copy(ap_save, ap);
	result = ft_vsnprintf(NULL, 0, format, ap);
	if (result < 0)
		return (FT_ERROR);
	result = ft_vsnprintf(str, (size_t)result + 1, format, ap);
	va_end(ap_save);
	return (result);
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

ssize_t 	copy_as_is(size_t size, const char **fmt, char **buf)
{
	const char *const	ptr = ft_strchrnul((*fmt), '%');
	const size_t		i = (size_t)(ptr - *fmt);
	const size_t		desired = MIN(i + FT_TERMINATOR, size);

	ft_memcpy((*buf), *fmt, desired * sizeof(char));
	if (*buf)
		(*buf) += desired - 1 * (desired > 1);
	(*fmt) = (ptr - 1);
	return ((ssize_t)i);
}

int	ft_vsnprintf_internal(char **str, size_t size, const t_snprintf_f *lut,
							va_list *ap)
{
	ssize_t			ret;
	register char	c;
	char			*seek;
	int				error_flag = 0;
	const char		*fmt = str[0];
	char *const		buf = str[1];

	seek = buf;
	ret = 0;
	c = *fmt;
	while (c != '\0' && error_flag != FT_ERROR)
	{
		size_t rem = size - (seek - buf);
		if (c == '%')
		{
			c = fmt[1];
			if (c == '\0')
			{
				if (rem > 0 && seek)
				{
					error_flag = FT_ERROR;;
//					*seek++ = '%';
				}
				ret++;
			}
			else
			{
				if (lut[(u_char) c] != NULL)
				{
					ret += (ssize_t) lut[(u_char) c](&seek, rem, ap);
					fmt++;
				}
				else
				{
					if (seek && rem > 0)
						*seek++ = '%';
					if (seek && rem > 1)
						*seek++ = c;
					ret += 2; // even if we can't write, count the character
					fmt++;
				}
			}
		}
		else
			ret += copy_as_is(rem, &fmt, &seek);
//		if (!error_flag)
		fmt++;
		c = *fmt;
	}
	return (error_flag ? FT_ERROR : (int)ret);
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
	va_end(ap_save);
	if (size && ret >= 0)
		dst[MIN(mini_size, (u_int)ret)] = '\0';
	return (ret);
}
