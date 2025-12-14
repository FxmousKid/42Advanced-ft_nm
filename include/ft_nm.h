#ifndef NM_H
# define NM_H

# include <stdio.h> // IWYU pragma: keep
# include <fcntl.h>
# include <stdlib.h> // IWYU pragma: keep
# include <unistd.h>
# include <errno.h> // IWYU pragma: keep
# include <string.h> // IWYU pragma: keep
# include <stdbool.h>
# include <getopt.h>
# include <elf.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include "libft.h" // IWYU pragma: keep


# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define DBG __FILE__ " @ " TOSTRING(__LINE__)
# define LOG_SYS(...) do {write(1, "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_perr(DBG); } while (0);
# define LOG_MSG(...) do {write(1, "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_err(DBG);  } while (0);

# ifndef DEFAULT_TARGET_FILE
#  define DEFAULT_TARGET_FILE "a.out"
# endif

struct s_map {
	size_t		size;
	unsigned char	*base;
};

struct s_data {
	char	**target_files;
	char	target_file[PATH_MAX + 1];
	bool	exit_code;
};

// Utils
int		get_logfd(void);
void		_write_perr(const char *location);
void		_write_err(const char *location);
void		print_help(void);

// Map utils
bool		m_inbounds(struct s_map *map, size_t offset, size_t size);
void		unmap_elf(struct s_map *map);
bool		map_elf(struct s_map *map, int fd);

// Parsing
bool		parse_cli(struct s_data *ctx, int *argc, char *argv[]);

// ft_nm
bool		parse_elf(int fd);
bool		ft_nm(struct s_data *ctx);

#endif
