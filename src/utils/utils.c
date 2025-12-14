#include "ft_nm.h"

void	print_help(void)
{
	printf("\
Usage: ft_nm [OPTIONS] [FILE]\n");
}

int	get_logfd(void)
{
	static int fd = 0;

	if (fd == 0)
		fd = open(LOGFILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0644);

	if (fd < 0) {
		printf("Could not create logfile\n");
		exit(2);
	}

	return fd;
}

/**
 * @brief prints on stderr a custom error msg
 *
 * @param location the macro that wraps __FILE__ and __LINE__
 *
 * @note this is internal function, and location is usually
 * a macro expanding to file and line number
 */
void	_write_perr(const char *location)
{
	dprintf(get_logfd(), " %s [%s]\n", strerror(errno), location);
}

/**
 * @brief prints on stderr a custom error msg
 *
 * @param msg the custom error message
 *
 * @note this is internal function, and location is usually
 * a macro expanding to file and line number
 */
void	_write_err(const char *location)
{
	dprintf(get_logfd(), " [%s]\n", location);
}
