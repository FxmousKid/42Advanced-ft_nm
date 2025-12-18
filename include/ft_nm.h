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
# include <inttypes.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include "libft.h" // IWYU pragma: keep

# define PROG_NAME "ft_nm"
# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define DBG __FILE__ " @ " TOSTRING(__LINE__)
# define LOG_SYS(...) do {write(get_logfd(), "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_perr(DBG); } while (0);
# define LOG_MSG(...) do {write(get_logfd(), "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_err(DBG);  } while (0);

# ifndef DEFAULT_TARGET_FILE
#  define DEFAULT_TARGET_FILE "a.out"
# endif

struct s_map {
	size_t		size;
	unsigned char	*base;
};

struct s_symbol {
	const char	*name;
	Elf64_Addr	value;
};

struct s_data {
	struct s_map	map;
	size_t		sym_count;
	/** @brief terminated by a 0-initialized s_symbol. */
	struct s_symbol	*symbols;
	char		**target_files;
	char		target_file[PATH_MAX + 1];
	bool		exit_code;
};

// Utils
int		get_logfd(void);
void		_write_perr(const char *location);
void		_write_err(const char *location);
void		print_help(void);

// Map utils
bool		m_inbounds(struct s_map *map, size_t offset, size_t size);
bool		unmap_elf(struct s_map *map);
bool		map_elf(struct s_map *map, int fd);

// Parsing
bool		parse_cli(struct s_data *ctx, int *argc, char *argv[]);
void		parse_elf_symbols64(const char *strtab,
				    const Elf64_Sym *sym_elf,
				    struct s_symbol *sym);

// ft_nm
void		display_symbols(struct s_symbol *sym, size_t symcount);
bool		parse_elf(int fd, struct s_data *ctx);

#endif
