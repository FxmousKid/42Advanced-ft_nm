#ifndef NM_H
# define NM_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#ifdef LIBFT_H
# define LIBFT_H "\0"
#endif

# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define DBG __FILE__ " @ " TOSTRING(__LINE__)
# define ERR_SYS(...) do { dprintf(get_logfd(), __VA_ARGS__); _write_perr(DBG); } while (0);
# define ERR_MSG(...) do { dprintf(get_logfd(), __VA_ARGS__); _write_err(DBG); } while (0);


struct s_data {
	bool	exit_code;
};

// Utils
int		get_logfd(void);
void		_write_perr(const char *location);
void		_write_err(const char *location);
void		print_help(void);

// Parsing
bool		parse_cli(struct s_data *ctx, int *argc, char *argv[]);

#endif
