/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utilc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 20:12:15 by abelov            #+#    #+#             */
/*   Updated: 2023/12/11 20:12:17 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_print_p(int fd, va_list *ap)
{
	size_t		i;
	char		*str;
	const void	*ptr = va_arg(*ap, void *);

	str = ft_print_pointer((unsigned long long int) ptr);
	if (!str)
	{
		free(str);
		return (0);
	}
	ft_putstr_fd(str, fd);
	i = ft_strlen(str);
	free(str);
	return (i);
}

size_t	ft_print_d(int fd, va_list *ap)
{
	size_t	i;
	char	*str;

	str = ft_itoa(va_arg(*ap, int));
	if (!str)
	{
		free(str);
		return (0);
	}
	ft_putstr_fd(str, fd);
	i = ft_strlen(str);
	free(str);
	return (i);
}

size_t	ft_print_s(int fd, va_list *ap)
{
	size_t		i;
	const char	*str;

	str = va_arg(*ap, char *);
	if (!str)
	{
		str = "(null)";
		ft_putstr_fd(str, fd);
		return (ft_strlen(str));
	}
	ft_putstr_fd(str, fd);
	i = ft_strlen(str);
	return (i);
}

size_t	ft_print_upperx(int fd, va_list *ap)
{
	size_t		i;
	char		*str;
	const char	*base = "0123456789ABCDEF";

	str = ft_uitoa_base(va_arg(*ap, unsigned long), base);
	if (!str)
	{
		free(str);
		return (0);
	}
	ft_putstr_fd(str, fd);
	i = ft_strlen(str);
	free(str);
	return (i);
}

size_t	ft_print_x(int fd, va_list *ap)
{
	size_t		i;
	char		*str;
	const char	*base = "0123456789abcdef";

	str = ft_uitoa_base(va_arg(*ap, unsigned long), base);
	if (!str)
	{
		free(str);
		return (0);
	}
	ft_putstr_fd(str, fd);
	i = ft_strlen(str);
	free(str);
	return (i);
}
