/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misargsy <misargsy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 17:32:53 by misargsy          #+#    #+#             */
/*   Updated: 2023/12/18 06:28:36 by misargsy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_single_conversion(
	const t_format format,
	va_list args,
	const char conv)
{
	if (conv == 'c')
		return (print_char(format, va_arg(args, int)));
	else if (conv == 's')
		return (print_string(format, va_arg(args, char *)));
	else if (conv == 'p')
		return (print_pointer(format, va_arg(args, void *)));
	else if (conv == 'd' || conv == 'i')
		return (print_int(format, va_arg(args, int)));
	else if (conv == 'u')
		return (print_uint(format, va_arg(args, unsigned int)));
	else if (conv == 'x')
		return (print_hex(format, va_arg(args, unsigned int), HEX_LC));
	else if (conv == 'X')
		return (print_hex(format, va_arg(args, unsigned int), HEX_UC));
	else if (conv == '%')
		return (print_percent(format));
	else
		return (INT_MIN);
}

static int	print_in_format_text(const char **format, const int fd)
{
	size_t	bytes_to_write;
	int		chars_printed;

	bytes_to_write = 0;
	chars_printed = 0;
	while ((*format)[bytes_to_write] != '\0'
		&& (*format)[bytes_to_write] != '%')
		bytes_to_write++;
	chars_printed = write(fd, *format, bytes_to_write);
	*format += bytes_to_write - 1;
	return (chars_printed);
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	t_format	data;
	int			chars_printed;

	chars_printed = 0;
	va_start(args, format);
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			if (*format == '\0')
				return (va_end(args), -1);
			get_format(&format, STDOUT_FILENO, &data);
			chars_printed += print_single_conversion(data, args, *format);
		}
		else
			chars_printed += print_in_format_text(&format, STDOUT_FILENO);
		if (chars_printed < 0)
			return (va_end(args), -1);
		format++;
	}
	va_end(args);
	return (chars_printed);
}
