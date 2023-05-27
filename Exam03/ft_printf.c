#include <unistd.h>
#include <stdarg.h>

void put_s(char *s ,int *len)
{
	if(!s)
		s = "(null)";
	while(*s)
	{
		*len += write(1, s++, 1);
	}
}

void put_d(long long int num, int base, int *len)
{
	char *dex = "0123456789abcdef";
	if(num < 0)
	{
		num *= -1;
		*len += write(1, "-", 1);
	}
	if(num >= base)
		put_d((num / base), base, len);
	*len += write(1, &dex[num % base], 1);
}

int ft_printf(const char *form, ... )
{
	int len = 0;
	va_list p;
	va_start(p, form);
	while (*form)
	{
		if(*form == '%' &&(*(form +1) == 's' || *(form +1) == 'd' || *(form +1) == 'x'))
		{
			form++;
			if (*form == 's')
				put_s(va_arg(p, char *), &len);
			else if (*form == 'd')
				put_d((long long int)va_arg(p, int), 10, &len);
			else if (*form == 'x')
				put_d((long long int)va_arg(p, unsigned int), 16, &len);
		}
		else
			len += write(1, form, 1);
		form++;
	}

	return (va_end(p), len);
}