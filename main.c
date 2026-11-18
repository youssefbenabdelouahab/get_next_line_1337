// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include "get_next_line.h"
 
// void ft_upper_case(char *s)
// {
// 	int i = 0;
// 	if (!s)
// 		return ;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] >= 'a' && s[i] <= 'z')
// 			s[i] -= 32;
// 		i++;
// 	}
// }

// int	main(void)
// {
// 	int		fd;
// 	char	*line;
// 	int len;
 
// 	fd = open("test.txt", O_RDONLY);
// 	get_next_line(fd);
// 	get_next_line(fd);
// 	get_next_line(fd);
// 	line = get_next_line(fd);
// 	close(fd);
// 	fd = open("test.txt", O_RDWR);
// 	get_next_line(fd);
// 	get_next_line(fd);
// 	get_next_line(fd);

// 	ft_upper_case(line);
// 	len = ft_strlen(line);
// 	write(fd, line, len);
// 	close(fd);
// 	return (0);
// }



#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
 
void ft_upper_case(char *s)
{
	int i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] -= 32;
		i++;
	}
}

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);

	line = get_next_line(fd);
	if (!line)
		printf("NULL\n");
	while (line)
	{
		printf("%s", line);
		line = get_next_line(fd);
		free(line);
	}
	close(fd);
	return (0);
}
