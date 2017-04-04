#include "get_next_line.h"
#include "libft.h"

void    concat_buff_and_leftovers(const int fd, char *buffer, char **leftovers)
{
    char    *tmp;
    
    if (!leftovers[fd])
        tmp = ft_strdup(buffer);
    else
        tmp = ft_strjoin(leftovers[fd], buffer);
    free(leftovers[fd]);
    leftovers[fd] = tmp;
}

int     setline_resize(const int fd, char **line, char **leftovers, int splt)
{
    char    *tmp;
    
    *line = ft_strsub(leftovers[fd], 0, splt);
    tmp = ft_strsub(leftovers[fd], (splt + 1), ft_strlen(leftovers[fd]) - splt);
    free(leftovers[fd]);
    leftovers[fd] = tmp;
    return (1);
}


int gnl(const int fd, char **line, char **leftovers, int bytes_read)
{
    int         split_pt;
    char        buffer[BUFF_SIZE + 1];
    
    split_pt = 0;
    while((bytes_read = read(fd, buffer, BUFF_SIZE)) > 0) 
    {
        buffer[bytes_read] = '\0';
        if (leftovers[fd])
            concat_buff_and_leftovers(fd, buffer, leftovers);
        else
            leftovers[fd] = ft_strdup(buffer);
    }
    if (bytes_read < 0)
        return (-1);
    while (leftovers[fd][split_pt] != 0 && leftovers[fd][split_pt] != 10)
        split_pt++;
    if (leftovers[fd][split_pt] == 10)
        return (setline_resize(fd, line, leftovers, split_pt)); 
    if (leftovers[fd][split_pt] == 0 && split_pt > 0)
    {
        *line = ft_strsub(leftovers[fd], 0, split_pt);
        leftovers[fd][0] = 0;
        return (1);
    }
    return (0);
}


int get_next_line(const int fd, char **line)
{
    static char **leftovers;
    
    if (!leftovers)
        leftovers = (char **)malloc(sizeof(char *) * (MAX_FD + 1));
    return (gnl(fd, line, leftovers, 0));
}
