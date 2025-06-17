/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 19:21:40 by abelov            #+#    #+#             */
/*   Updated: 2025/01/17 16:55:30 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <ctype.h>
#include "ft_printf.h"

# define BUF_SIZE 1024
# define ENDL FT_RESET

# define FT_RED   "\033[0;31m"
# define FT_GREEN "\033[0;32m"
# define FT_CYAN  "\033[36m"
# define FT_RESET "\e[0m"



struct STORAGE_T
{
	jmp_buf AbortFrame;
	const char* TestFile;
	unsigned long CurrentTestLineNumber;
} Tester;

#define TEST_PROTECT() (setjmp(Tester.AbortFrame) == 0)
//#define TEST_ABORT() longjmp(Tester.AbortFrame, 1)
#define TEST_ABORT()
#define TEST_RUN(statement) do { if (TEST_PROTECT()) statement; } while (0)

#define TEST_FAIL() fprintf(stdout, "%s:%ld - %s\n", Tester.TestFile, Tester.CurrentTestLineNumber, __func__); TEST_ABORT();

#define RUN_TEST(...) RUN_TEST_AT_LINE(__VA_ARGS__, __FILE__, __LINE__, throwaway)
#define RUN_TEST_AT_LINE(statement, file, line, ...) Tester.TestFile = file; Tester.CurrentTestLineNumber = line; TEST_RUN(statement)

void	check(bool succes)
{
	if (succes)
		printf("> "FT_GREEN".OK "FT_RESET"\n");
	else
		printf("> "FT_RED".KO "FT_RESET"\n");
}

void sigsegv(int signal)
{
	(void) signal;
	dprintf(STDERR_FILENO, "> "FT_CYAN".SIGSEGV"FT_RESET"\n");
	exit(EXIT_FAILURE);
}

void	ft_print_title(char *title)
{
	printf("--------------------------------------------------\n");
	printf("> %s%s%s\n", FT_CYAN, title, FT_RESET);
	printf("--------------------------------------------------\n");
}

void	snprint_test(const char *s, ...)
{
	int		printf_ret, ft_printf_ret;
	va_list	printf_args, ft_printf_args;
	char	ft_printf_buf[BUF_SIZE] = {0};
	char	printf_buf[BUF_SIZE] = {0};
	char	err_buf[3 * BUF_SIZE + 1] = {0}; // Enough space for \xXX sequences + null terminator
	char	ft_err_buf[3 * BUF_SIZE + 1] = {0}; // Enough space for \xXX sequences + null terminator
	char	*p = NULL;
	int		check_val;

	va_start(printf_args, s);
	printf_ret = vsprintf(printf_buf, s, printf_args);
	printf_buf[printf_ret] = '\0';
	va_end(printf_args);

	va_start(ft_printf_args, s);
	ft_printf_ret = ft_vsnprintf(ft_printf_buf, BUF_SIZE, s, ft_printf_args);
	va_end(ft_printf_args);

	check_val = (printf_ret == ft_printf_ret);
	if (!check_val)
	{
		fprintf(stdout, "ft_printf_ret doesn't match expected value\n");
		fprintf(stdout, "got return size \"%d\" whilst [\"%d\"] was to be expected\n",
				ft_printf_ret, printf_ret);
//		TEST_FAIL();
	}
	if (printf_ret != 0)
	{
		check_val = !strcmp(ft_printf_buf, printf_buf);
		if (!check_val)
		{
			fprintf(stdout, "got \"%s\" whilst \"%s\" was to be expected\n", ft_printf_buf, printf_buf);
			TEST_FAIL();
		}
	}
	check(check_val);
}

#define __USE_GNU
#include <dlfcn.h>
#include <link.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

typedef int (*vsnprintf_fn_t)(char *, size_t, const char *, va_list);
typedef int (*vprintf_fn_t)(const char *, va_list);

#if defined(__GNUC__) && !defined(__clang__)
__attribute__((optimize("no-builtin")))
#else
__attribute__((no_builtin))
#endif
__attribute__((no_sanitize("all")))
void *resolve_libc_symbol(const char *symbol_name)
{
	void			*libc_sym;
	struct link_map	*map = NULL;
	void			*handle;
	void			*libc_handle;

	handle = dlopen(NULL, RTLD_LAZY);
	if (!handle)
	{
		ft_dprintf(STDERR_FILENO, "dlopen(NULL) failed: %s\n", dlerror());
		return (NULL);
	}
	if (dlinfo(handle, RTLD_DI_LINKMAP, &map) != 0)
	{
		ft_dprintf(STDERR_FILENO, "dlinfo failed\n");
		return (NULL);
	}

	while (map != NULL)
	{
		if (ft_strstr(map->l_name, "libc.so"))
		{
			libc_handle = dlopen(map->l_name, RTLD_LAZY);
			if (!libc_handle)
			{
				ft_dprintf(STDERR_FILENO, "dlopen(%s) failed: %s\n", map->l_name, dlerror());
				return (NULL);
			}
			libc_sym = (void *) dlsym(libc_handle, symbol_name);
			if (!libc_sym)
			{
				ft_dprintf(STDERR_FILENO, "dlsym failed: %s\n", dlerror());
				return (NULL);
			}
			return (libc_sym);
		}
		map = map->l_next;
	}
	return NULL;
}

__attribute__((no_sanitize("all")))
__attribute__((optnone))
int real_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{

	static vsnprintf_fn_t	libc_vsnprintf = NULL;

	if (!libc_vsnprintf)
		libc_vsnprintf = resolve_libc_symbol("vsnprintf");
	va_list ap_copy;
	va_copy(ap_copy, ap);
	int ret = libc_vsnprintf(buf, size, fmt, ap_copy);
	va_end(ap_copy);
	return ret;
}

int real_snprintf(char *buf, size_t size, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int ret = real_vsnprintf(buf, size, fmt, ap);
	va_end(ap);
	return ret;
}

__attribute__((no_sanitize("all")))
int not_so_real_vprintf(const char *fmt, va_list ap)
{
	static vprintf_fn_t	real_vprintf_ptr = NULL;

	if (!real_vprintf_ptr)
		real_vprintf_ptr = (vprintf_fn_t)dlsym(RTLD_NEXT, "vprintf");
	va_list ap_copy;
	va_copy(ap_copy, ap);
	int ret = real_vprintf_ptr(fmt, ap_copy);
	va_end(ap_copy);
	return ret;
}

/**
 * Also doesn't work with gcc and sanitizers
 * @param fmt
 * @param ap
 * @return
 */
__attribute__((no_sanitize("all")))
int very_real_vprintf(const char *fmt, va_list ap)
{
	void				*handle;
	static vprintf_fn_t	libc_vprintf = NULL;

	if (!libc_vprintf)
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		if (!handle)
		{
			dprintf(STDERR_FILENO, "failed to open libc: %s\n", dlerror());
			return (-1);
		}
		libc_vprintf = (vprintf_fn_t)dlsym(handle, "vprintf");
	}
	va_list ap_copy;
	va_copy(ap_copy, ap);
	int ret = libc_vprintf(fmt, ap_copy);  // real vprintf, no interceptor
	va_end(ap_copy);
	return ret;
}

void	print_test(const char *fmt, ...)
{
	int 	fd[2];
	int 	*read_end, *write_end, std_out;
	ssize_t ft_read_return;
	ssize_t read_return;
	int		printf_ret, ft_printf_ret;
	va_list	printf_args, ft_printf_args;
	char	ft_printf_buf[BUF_SIZE] = {0};
	char	printf_buf[BUF_SIZE] = {0};
	char	err_buf[3 * BUF_SIZE + 1] = {0}; // Enough space for \xXX sequences + null terminator
	char	ft_err_buf[3 * BUF_SIZE + 1] = {0}; // Enough space for \xXX sequences + null terminator
	char	*p = NULL;
	int		i;
	int		check_val;

	for (i = -1; ++i < BUF_SIZE / 4; printf_buf[i] = ft_printf_buf[i] = 0xbe);
	if (pipe(fd) < 0)
	{
		dprintf(STDERR_FILENO, "(%s:%d, %s): ", __FILE__, __LINE__, __func__);
		perror("pipe failed");
		exit(1);
	}

	read_end = &fd[0];
	write_end = &fd[STDOUT_FILENO];

	std_out = dup(STDOUT_FILENO);

	dup2(fd[STDOUT_FILENO], STDOUT_FILENO);

	va_start(printf_args, fmt);

	printf_ret = very_real_vprintf(fmt, printf_args);

	close(fd[STDOUT_FILENO]);
	dup2(std_out, STDOUT_FILENO);

	if ((read_return = read(*read_end, printf_buf, BUF_SIZE)) < 0)
		return (perror("read failed"));
	printf_buf[read_return] = '\0';

	close(*read_end);
	va_end(printf_args);

	if (pipe(fd) < 0)
	{
		dprintf(STDERR_FILENO, "pipe failed: %s \n on %s at %s:%d ",
			strerror(errno), __func__, __FILE__, __LINE__);
		exit(1);
	}

	std_out = dup(STDOUT_FILENO);
	dup2(*write_end, STDOUT_FILENO);

	va_start(ft_printf_args, fmt);
	ft_printf_ret = ft_vprintf(fmt, ft_printf_args);

	close(fd[STDOUT_FILENO]);
	dup2(std_out, STDOUT_FILENO);

	if ((ft_read_return = read(*read_end, ft_printf_buf, BUF_SIZE)) < 0)
		return (perror("read failed"));
	ft_printf_buf[ft_read_return] = '\0';

	close(*read_end);
	va_end(ft_printf_args);

	dup2(std_out, STDOUT_FILENO);

	check_val = (printf_ret == ft_printf_ret);
	if (!check_val)
	{
		fprintf(stdout, "ft_printf_ret doesn't match expected value\n");
		fprintf(stdout, "got return size \"%d\" whilst [\"%d\"] was to be expected\n",
			ft_printf_ret, printf_ret);
//		TEST_FAIL();
	}
	if (printf_ret != 0)
	{
		check_val = !ft_memcmp(ft_printf_buf, printf_buf, MAX(ft_read_return, read_return));
		if (!check_val)
		{
			char *format;
			p = ft_err_buf;
			i = -1;
			while (++i < ft_read_return)
			{
				format = isprint(ft_printf_buf[i]) ? "%c" : "\\x%02X";
				p += sprintf(p, format, ft_printf_buf[i]);
			}
			p = err_buf;
			i = -1;
			while (++i < read_return)
			{
				format = isprint(printf_buf[i]) ? "%c" : "\\x%02X";
				p += sprintf(p, format, printf_buf[i]);
			}
			fprintf(stdout, "for input format \"%s\" got \"%s\" whilst"
				" \"%s\" was to be expected\n", fmt, ft_err_buf, err_buf);
			TEST_FAIL();
		}
	}
	check(check_val);
}

/*
 * fflush(stdout);
 * setbuf(stdout, NULL);
 * */
int	main(void)
{
	char *null_str = NULL;
	setbuf(stdout, NULL);
	signal(SIGSEGV, sigsegv);

	const char *s2 = "Mussum Ipsum, cacilds vidis litro abertis. Posuere libero varius. Nullam a nisl ut ante blandit hendrerit. Aenean sit amet nisi. Atirei o pau no gatis, per gatis num morreus.";

	ft_print_title("ft_dprintf_test");

	char buf[10];

	ft_snprintf(buf, 10, "%d0123456789", 10);
	check(ft_strlen(buf) == 9);

	ft_print_title("printf_test");

	RUN_TEST(print_test("%c%c%c*", '\0', '1', 1));
	RUN_TEST(print_test("%%"));
	RUN_TEST(print_test(" %% "));
	RUN_TEST(print_test(" %%"));
	RUN_TEST(print_test("%%c"));

	RUN_TEST(print_test("%%%%%%"));
	RUN_TEST(print_test("%%%c", 'x'));

	RUN_TEST(print_test("%c", 'x'));
	RUN_TEST(print_test(" %c", 'x'));
	RUN_TEST(print_test("%c ", 'x'));
	RUN_TEST(print_test("%c%c%c", 'a', '\t', 'b'));
	RUN_TEST(print_test("%cc%cc%c", 'a', '\t', 'b'));
	RUN_TEST(print_test("%cs%cs%c", 'c', 'b', 'a'));

	RUN_TEST(print_test("%s", "hey"));
	RUN_TEST(print_test("%s", (char *)NULL));
	RUN_TEST(print_test("This is a simple test.\nSecond sentence.\n"));

	RUN_TEST(print_test(" %s", ""));
	RUN_TEST(print_test("%s ", ""));
	RUN_TEST(print_test(" %s ", ""));
	RUN_TEST(print_test(" %s ", "-"));
	RUN_TEST(print_test(" %s %s ", "", "-"));
	RUN_TEST(print_test(" %s %s ", " - ", ""));
	RUN_TEST(print_test(" %s %s %s %s %s", " - ", "", "4", "", s2));
	RUN_TEST(print_test(" %s %s %s %s %s ", " - ", "", "4", "", "2 "));
	RUN_TEST(print_test(" NULL %s NULL ", NULL));

	RUN_TEST(print_test("%d", 123));
	RUN_TEST(print_test("%d", 46546546564));
	RUN_TEST(print_test("%c", '0'));
	RUN_TEST(print_test(" %c ", '0'));
	RUN_TEST(print_test(" %c", '0' - 256));
	RUN_TEST(print_test("%c ", '0' + 256));
	RUN_TEST(print_test(" %c %c %c ", '0', 0, '1'));
	RUN_TEST(print_test(" %c %c %c ", ' ', ' ', ' '));
	RUN_TEST(print_test(" %c %c %c ", '1', '2', '3'));
	RUN_TEST(print_test(" %c %c %c ", '2', '1', 0));
	RUN_TEST(print_test(" %c %c %c ", 0, '1', '2'));
	RUN_TEST(print_test("\n---%x\n", 0));
	RUN_TEST(print_test("\n%x", 0));

	static char     a01;
	static unsigned char a02;
	static short a03;
	static unsigned short a04;
	static int a05;
	static unsigned int a06;
	static long a07;
	static unsigned long a08;
	static long long a09;
	static unsigned long long a10;
	static char *a11;
	static void *a12;
	RUN_TEST(print_test("%p%p%p%p%p%p%p%p%p%p%p%p",&a01,&a02,&a03,&a04,&a05,&a06,&a07,&a08,&a09,&a10,&a11,&a12));

	ft_print_title("snprintf_test");

	RUN_TEST(snprint_test("%%"));
	RUN_TEST(snprint_test(" %%"));
	RUN_TEST(snprint_test("%%c"));
	RUN_TEST(snprint_test("%%%%%%"));
	RUN_TEST(snprint_test("%%%c", 'x'));

	RUN_TEST(snprint_test("%c", 'x'));
	RUN_TEST(snprint_test(" %c", 'x'));
	RUN_TEST(snprint_test("%c ", 'x'));
	RUN_TEST(snprint_test("%c%c%c", 'a', '\t', 'b'));
	RUN_TEST(snprint_test("%cc%cc%c", 'a', '\t', 'b'));
	RUN_TEST(snprint_test("%cs%cs%c", 'c', 'b', 'a'));

	RUN_TEST(snprint_test("%s", "hey"));
	RUN_TEST(snprint_test("%s", (char *)NULL));
	RUN_TEST(snprint_test("This is a simple test.\nSecond sentence.\n"));

	RUN_TEST(snprint_test("%c%c%c*", '\0', '1', 1));
	RUN_TEST(snprint_test("%c %s - %p - %d - %i %u - %x - %X %%", '\0', null_str, (void *)0xdeadc0de, 0, (int)-2147483648, -1, -1, 200000000));
	RUN_TEST(print_test("%c %s - %p - %d - %i %u - %x - %X %%", '\0', null_str, (void *)0xdeadc0de, 0, (int)-2147483648, -1, -1, 200000000));

	RUN_TEST(snprint_test("%d", 123));
	RUN_TEST(snprint_test("%c", '0'));
	RUN_TEST(snprint_test(" %c ", '0'));
	RUN_TEST(snprint_test(" %c", '0' - 256));
	RUN_TEST(snprint_test("%c ", '0' + 256));
	RUN_TEST(snprint_test(" %c %c %c ", '0', 0, '1'));
	RUN_TEST(snprint_test(" %c %c %c ", ' ', ' ', ' '));
	RUN_TEST(snprint_test(" %c %c %c ", '1', '2', '3'));
	RUN_TEST(snprint_test(" %c %c %c ", '2', '1', 0));
	RUN_TEST(snprint_test(" %c %c %c ", 0, '1', '2'));
	RUN_TEST(snprint_test("\n---%x\n", 0));
	RUN_TEST(snprint_test("\n%x", 0));

	RUN_TEST(snprint_test("%p%p%p%p%p%p%p%p%p%p%p%p",&a01,&a02,&a03,&a04,&a05,&a06,&a07,&a08,&a09,&a10,&a11,&a12));


	RUN_TEST(print_test("%%%p"));
	RUN_TEST(print_test("%%%k"));
	RUN_TEST(print_test("%%%b"));
	RUN_TEST(print_test("%%k")); // Escaped percent + unknown specifier
	RUN_TEST(print_test("Hello %b world")); 	// Valid format with unknown specifier
	RUN_TEST(print_test("Hello %")); // Dangling '%'
	RUN_TEST(print_test("%k %b %v")); // Multiple unknowns

	//	RUN_TEST(print_test("%a %b %c")); // Multiple unknowns
//	RUN_TEST(print_test("%%%z"));
//	RUN_TEST(print_test("%%%j"));
//	RUN_TEST(snprint_test("%5%"));
//	RUN_TEST(print_test("%015d", 0));
//	RUN_TEST(print_test("\n%a%x", 0));
//	RUN_TEST(print_test("\n%", 0));
//	RUN_TEST(print_test("%5%"));
//	RUN_TEST(print_test("%-5%"));
//	RUN_TEST(print_test("%.c", 'a'));
	return (0);
}
