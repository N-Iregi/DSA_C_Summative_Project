#include "event_logger.h"

/**
 * read_line - read one line from stdin without its newline
 * @buf: destination buffer
 * @size: size of @buf in bytes
 *
 * Return: -1 on EOF, 1 if the line was too long (the excess is
 * discarded), 0 otherwise
 */
int read_line(char *buf, size_t size)
{
	size_t len;
	int c, truncated = 0;

	if (fgets(buf, (int)size, stdin) == NULL)
	{
		buf[0] = '\0';
		return (-1);
	}
	len = strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
	{
		buf[--len] = '\0';
		if (len > 0 && buf[len - 1] == '\r')
			buf[--len] = '\0';
		return (0);
	}
	while ((c = getchar()) != '\n' && c != EOF)
		truncated = 1;
	return (truncated);
}

/**
 * trim - remove leading and trailing white space in place
 * @s: string to trim
 *
 * Return: pointer to the first non-space character of @s
 */
char *trim(char *s)
{
	char *end;

	while (isspace((unsigned char)*s))
		s++;
	if (*s == '\0')
		return (s);
	end = s + strlen(s) - 1;
	while (end > s && isspace((unsigned char)*end))
		*end-- = '\0';
	return (s);
}
