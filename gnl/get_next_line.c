#include "get_next_line.h"

size_t ft_strlen(char *s)
{
    int len = 0;
    while(s[len] != '\0')
    {
        len++;
    }
    return (len);
}

char *ft_strdup(char *s)
{
    if (!s)
        return (NULL);
    char *new_s = malloc(ft_strlen(s) + 1);
    if (!new_s)
        return (NULL);
    int i = 0;
    while (s[i] != '\0')
    {
        new_s[i] = s[i];
        i++;
    }
    new_s[i] = '\0';
    return (new_s);
}

void ft_memcpy(char *dst, char *src, int n)
{
    int i = 0;
    while (i < n && src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
}

char *ft_strjoin(char *s1, char *s2)
{
    size_t len_s1 = ft_strlen(s1);
    size_t len_s2 = ft_strlen(s2);
    char *s3 = malloc (len_s1 + len_s2 + 1);
    if (!s3)
        return (NULL);
    ft_memcpy(s3, s1, len_s1);
    ft_memcpy(s3 + len_s1, s2, len_s2); 
    s3[len_s1 + len_s2] = '\0';
    return (s3);
}

char *ft_strchr(char *s, char c)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if (s[i] == c)
            return (&s[i]);
        i++;
    }
    return (NULL);
}

char *ft_substr(char *s, int start, int len)
{
    if (!s)
        return (NULL);
    size_t len_s = ft_strlen(s);
    if (len_s < start + len)
        len = len_s - start;
    char *new_s = malloc(len + 1);
    if (!new_s)
        return (NULL);
    int i = 0;
    while (i < len && s[start + i] != '\0')
    {
        new_s[i] = s[start + i];
        i++;
    }
    new_s[i] = '\0';
    return (new_s);
}

char *create_line(char **remainder)
{
    char *line;
    char *tmp;
    if (*remainder == NULL || **remainder == '\0')
        return (NULL);
    char *pos = ft_strchr(*remainder, '\n');
    if (pos)
    {
        line = ft_substr(*remainder, 0, (pos - *remainder) + 1);
        tmp = ft_strdup(pos + 1);
        free(*remainder);
        *remainder = tmp;
        return (line);
    }
    line = *remainder;
    *remainder = NULL;
    return (line);
}


char *read_file(int fd, char **remainder)
{
    if (*remainder == NULL)
        *remainder = ft_strdup("\0");
    char *buffer;
    char *tmp;
    int bytes;
    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    while (ft_strchr(*remainder, '\n') == NULL)
    {
        bytes = read(fd, buffer, BUFFER_SIZE);
        if (bytes < 0)
            return (free(buffer), NULL);
        else if (bytes == 0)
            break ;
        buffer[bytes] = '\0';
        tmp = ft_strjoin(*remainder, buffer);
        free(*remainder);
        *remainder = tmp;
    }
    free(buffer);
    return (*remainder);
}

char *get_next_line(int fd)
{
    if (fd < 0 || fd > 1024 || BUFFER_SIZE < 0)
        return (NULL);
    char *line = NULL;
    static char *remainder;

    remainder = read_file(fd, &remainder);
    if (remainder == NULL)
    {
        return (NULL);
    }
    line = create_line(&remainder);
    if (remainder && *remainder == '\0')
    {
        free(remainder);
        remainder = NULL;
    }
    return (line);
}
