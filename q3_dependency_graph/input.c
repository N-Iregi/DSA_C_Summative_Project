#include "graph.h"

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

/**
 * ci_equal - compare two strings ignoring letter case
 * @a: first string
 * @b: second string
 *
 * Return: 1 if equal, 0 otherwise
 */
int ci_equal(const char *a, const char *b)
{
	for (; *a != '\0' && *b != '\0'; a++, b++)
	{
		if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
			return (0);
	}
	return (*a == *b);
}

/**
 * prompt_name - ask for a server name
 * @prompt: text shown to the operator
 * @out: buffer that receives the trimmed name
 * @size: size of @out
 *
 * Return: 1 if a usable name was entered, 0 if blank or too long
 */
int prompt_name(const char *prompt, char *out, size_t size)
{
	char line[LINE_LEN], *s;
	int r;

	printf("%s", prompt);
	r = read_line(line, sizeof(line));
	s = trim(line);
	if (r < 0 || *s == '\0')
	{
		puts("No name entered - operation cancelled.");
		return (0);
	}
	if (r == 1 || strlen(s) >= size)
	{
		printf("Name too long (max %d characters).\n", NAME_LEN - 1);
		return (0);
	}
	snprintf(out, size, "%s", s);
	return (1);
}
