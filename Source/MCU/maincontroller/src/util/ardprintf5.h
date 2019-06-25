#ifndef ARDPRINTF5_H
#define ARDPRINTF5_H

#define ARDBUFFER5 16
#include <stdarg.h>
#include <Arduino.h>

int ardprintf5(const char *str, ...)
{
	int i, size = 0, j = 0;
	char temp[ARDBUFFER5 + 1];
	for (i = 0; str[i] != '\0'; i++)
		if (str[i] == '%')
			size++;

	va_list argv;
	va_start(argv, str);
	for (i = 0, j = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '%')
		{
			temp[j] = '\0';
			Serial5.print(temp);
			j = 0;
			temp[0] = '\0';

			switch (str[++i])
			{
			case 'd':
				Serial5.print(va_arg(argv, int));
				break;
			case 'l':
				Serial5.print(va_arg(argv, long));
				break;
			case 'f':
				Serial5.print(va_arg(argv, double));
				break;
			case 'c':
				Serial5.print((char)va_arg(argv, int));
				break;
			case 's':
				Serial5.print(va_arg(argv, char *));
				break;
			default:;
			};
		}
		else
		{
			temp[j] = str[i];
			j = (j + 1) % ARDBUFFER5;
			if (j == 0)
			{
				temp[ARDBUFFER5] = '\0';
				Serial5.print(temp);
				temp[0] = '\0';
			}
		}
	};
	Serial5.println();
	return size + 1;
}
#undef ARDBUFFER5

#endif /* ARDPRINTF5_H */