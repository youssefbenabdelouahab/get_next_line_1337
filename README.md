*This project has been created as part of the 42 curriculum by yben-abd.*
 
# get_next_line
 
## Description
 
**get_next_line** is a C function that returns one line at a time from a file descriptor. Each call to `get_next_line(fd)` returns the next line of the file (including the terminating `\n`, unless the last line of the file has none), and returns `NULL` when there is nothing left to read or when an error occurs.
 
The goal of the project is to learn about:
 
- **static variables**, which let a function keep its state between calls
- reading with `read()` using a configurable buffer size (`BUFFER_SIZE`)
- dynamic memory management (no leaks, no double frees)
- working with any file descriptor: regular files, standard input, pipes, etc.
```c
char *get_next_line(int fd);
```
 
### Project files
 
| File | Description |
|------|-------------|
| `get_next_line.c` | Main function and its static helpers |
| `get_next_line_utils.c` | Helper functions (`ft_strlen`, `ft_strjoin`, `ft_substr`, `ft_strchr`, `ft_strdup`) |
| `get_next_line.h` | Header: prototypes and default `BUFFER_SIZE` |
| `get_next_line_bonus.c` | Bonus: multiple file descriptors at once |
| `get_next_line_utils_bonus.c` | Bonus helper functions |
| `get_next_line_bonus.h` | Bonus header |
 
### Allowed / forbidden
 
- Allowed external functions: `read`, `malloc`, `free`
- Forbidden: `libft`, `lseek()`, global variables
## Instructions
 
### Compilation
 
The project is compiled with the usual flags, and `BUFFER_SIZE` can be set at compile time:
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```
 
If `-D BUFFER_SIZE` is not given, a default value of **42** is used (defined in `get_next_line.h`).
 
**Bonus version:**
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```
 
> `main.c` is your own test file. It is not part of the submission.
 
### Usage example
 
```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"
 
int	main(void)
{
	int		fd;
	char	*line;
 
	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```
 
It also works on standard input (`fd = 0`):
 
```bash
echo "hello\nworld" | ./a.out
```
 
### Testing with different buffer sizes
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1        *.c   # smallest buffer
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999     *.c   # medium buffer
cc -Wall -Wextra -Werror -D BUFFER_SIZE=10000000 *.c   # very large buffer
```
 
Memory leaks can be checked with:
 
```bash
valgrind --leak-check=full ./a.out
```
 
## Algorithm
 
The implementation relies on a **static stash**: a `static char *` that survives between calls and stores whatever was read from the file descriptor but not yet returned to the caller.
 
### How a call works
 
1. **Validation.** If `fd < 0`, `BUFFER_SIZE <= 0`, or `read(fd, NULL, 0) < 0` (invalid / unreadable descriptor), the stash is freed and `NULL` is returned.
2. **Allocate a buffer** of `BUFFER_SIZE + 1` bytes (the extra byte is for the `'\0'` terminator).
3. **`fill_stash`**: repeatedly call `read(fd, buffer, BUFFER_SIZE)`, null-terminate what was read, and append it to the stash with `ft_strjoin`. The loop stops as soon as:
   - the stash contains a `\n` (a full line is available), or
   - `read` returns `0` (end of file), or
   - `read` returns `-1` (error, so `NULL` is returned).
4. **`extract_line`**: copy from the start of the stash up to and including the first `\n` (or to the end if there is none) with `ft_substr`. This is the line returned to the caller.
5. **`stash_trim`**: keep only what comes after the returned line. This remainder becomes the new static stash for the next call.
6. The buffer and the old stash are freed, and the line is returned.
### Why this design
 
- **Reads as little as possible.** The loop stops as soon as a newline appears in the stash, so the file is never read entirely in advance, as the subject requires.
- **Works for any `BUFFER_SIZE`.** Whether the buffer is 1 byte or 10 million bytes, the stash accumulates data until a newline or EOF is found. A small buffer just means more `read` calls, and a large one means more data is kept in the stash and carried over to the next call.
- **Correct EOF handling.** If the file does not end with `\n`, the remaining text is returned as the last line without a newline. After that the stash becomes empty and the next call returns `NULL`.
- **No leaks.** Every intermediate string (old stash, buffer, full data) is freed right after use, and the stash is freed on every error path.
- **Simple and readable.** Each step has its own small static function (`fill_stash`, `extract_line`, `stash_trim`), which keeps the code easy to explain and Norm-compliant.
### Bonus: multiple file descriptors
 
The bonus version uses **a single static variable**, an array of stashes indexed by file descriptor:
 
```c
static char *stash[1024];
```
 
Each fd has its own stash at `stash[fd]`, so you can alternate calls between fd 3, 4, 5, and so on without losing the reading state of any of them or returning a line from the wrong one. The algorithm is otherwise identical to the mandatory part.
 
## Resources
 
### References
 
- `man 2 read`, `man 3 malloc`, `man 3 free`
- [Static variables in C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/)
- [Understanding file descriptors (Wikipedia)](https://en.wikipedia.org/wiki/File_descriptor)
- [Valgrind documentation](https://valgrind.org/docs/manual/manual.html)
- 42 Norm and the project subject (version 14.3)
### Use of AI
 
AI was used for the following tasks only:
 
- **Explaining concepts and theory**: static variables, file descriptors, how `read()` behaves, and the effect of different `BUFFER_SIZE` values.
- **Helping write this `README.md`**: structure and wording, based on the project subject and on my own code.
AI was **not** used to write the project's source code. The algorithm and implementation are my own work, and I can explain every part of them during peer evaluation.