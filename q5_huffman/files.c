#include "huffman.h"

/**
 * read_whole_file - load a file into memory
 * @path: file to read
 * @size_out: receives the number of bytes read
 *
 * Return: malloc'ed buffer (never NULL on success, even for an empty
 * file), or NULL on error after printing a message
 */
unsigned char *read_whole_file(const char *path, size_t *size_out)
{
	FILE *fp;
	unsigned char *buf, *bigger;
	size_t cap = 4096, len = 0, got;

	fp = fopen(path, "rb");
	if (fp == NULL)
	{
		printf("Error: cannot open '%s' (%s).\n", path,
				strerror(errno));
		return (NULL);
	}
	buf = malloc(cap);
	while (buf != NULL && (got = fread(buf + len, 1, cap - len, fp)) > 0)
	{
		len += got;
		if (len < cap)
			continue;
		bigger = realloc(buf, cap * 2);
		if (bigger == NULL)
			free(buf);
		buf = bigger;
		cap *= 2;
	}
	if (buf == NULL || ferror(fp))
	{
		printf("Error: could not read '%s'.\n", path);
		free(buf);
		fclose(fp);
		return (NULL);
	}
	fclose(fp);
	*size_out = len;
	return (buf);
}

/**
 * file_size - size of a file in bytes
 * @path: file to measure
 *
 * Return: the size, or -1 if the file cannot be opened
 */
long file_size(const char *path)
{
	FILE *fp;
	long size = -1;

	fp = fopen(path, "rb");
	if (fp == NULL)
		return (-1);
	if (fseek(fp, 0, SEEK_END) == 0)
		size = ftell(fp);
	fclose(fp);
	return (size);
}
