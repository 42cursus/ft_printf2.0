/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 00:46:19 by abelov            #+#    #+#             */
/*   Updated: 2025/01/18 22:57:40 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

# define STRPTRNULL "(null)"
# ifdef __linux__
#  define PTRNULL "(nil)"
# endif

# ifndef PTRNULL
#  ifdef __APPLE__
#   define PTRNULL "0x0"
#  endif
# endif

# define FT_TERMINATOR 1
# define FT_ERROR -1

typedef size_t	(*t_printf_f)(int fd, va_list *ap);
typedef size_t	(*t_snprintf_f)(char **buf, size_t size, va_list *ap);

typedef struct s_vsnprintf
{
	char			*buf;
	const char		*fmt;
	char			*seek;
	int				error_flag;
	size_t			rem;
	t_snprintf_f	*lut;
}	t_printf_var;

/* ---------- PRINTF -------------------- */
int		ft_printf(const char *format, ...);
int		ft_sprintf(char *str, const char *format, ...);
int		ft_snprintf(char *str, size_t size, const char *format, ...);
int		ft_dprintf(int fd, const char *format, ...);
int		ft_vprintf(const char *fmt, va_list ap);
int		ft_vdprintf(int fd, const char	*fmt, va_list ap);
int		ft_vsnprintf(char *dst, size_t size, const char *fmt, va_list ap);
int		ft_vsprintf(char *str, const char *format, va_list ap);

size_t	ft_print_p(int fd, va_list *ap);
size_t	ft_print_d(int fd, va_list *ap);
size_t	ft_print_s(int fd, va_list *ap);
size_t	ft_print_x(int fd, va_list *ap);
size_t	ft_print_upperx(int fd, va_list *ap);
size_t	ft_print_u(int fd, va_list *ap);
size_t	ft_print_c(int fd, va_list *ap);
size_t	ft_print_percent(int fd, va_list *ap);

size_t	ft_snprint_d(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_s(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_upperx(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_x(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_p(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_u(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_percent(char **buf, size_t size, va_list *ap);
size_t	ft_snprint_c(char **buf, size_t size, va_list *ap);

char	*ft_strchrnul(const char *s, int c);
char	*ft_uitoa(unsigned int nb);
char	*ft_uitoa_base(unsigned int nb, const char *base);
char	*ft_ultoa_buf_base(unsigned long abs,
			char *buf, size_t length, const char *base);
char	*ft_print_pointer(unsigned long long nb);
void	*ft_reallocarray(void *ptr, size_t oldnmemb, size_t newnmemb,
			size_t size);
#endif //FT_PRINTF_H
