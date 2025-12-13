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

# include "libft.h" // IWYU pragma: keep


# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define DBG __FILE__ " @ " TOSTRING(__LINE__)
# define ERR_SYS(...) do { dprintf(get_logfd(), __VA_ARGS__); _write_perr(DBG); } while (0);
# define ERR_MSG(...) do { dprintf(get_logfd(), __VA_ARGS__); _write_err(DBG); } while (0);


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

// Parsing
bool		parse_cli(struct s_data *ctx, int *argc, char *argv[]);

// ft_nm
bool	ft_nm(struct s_data *ctx);

#endif
