#include "ft_nm.h"

bool	ft_nm(struct s_data *ctx)
{
	int fd = open(ctx->target_file, O_RDONLY);
	if (fd < 0) {
		LOG_SYS("open(%s, O_RDONLY)", ctx->target_file);
		ft_printf("ft_nm: %s: %s\n", ctx->target_file, strerror(errno));
		write(1, "\n", 1);
		return false;
	}
	ft_printf("ft_nm: %s\n", ctx->target_file);
	if (!parse_elf(fd)) {
		write(1, "\n", 1);
		close(fd);
		return false;
	}
	write(1, "\n", 1);
	close(fd);
	return true;
}

int	main(int argc, char *argv[])
{
	struct s_data	ctx = {0};
	char		*default_target_file = DEFAULT_TARGET_FILE;

	parse_cli(&ctx, &argc, argv);
	/*ctx now looks something like this :
	* ctx.target_files == {"file1", ..., NULL} // no allocation used, we took argv
	* ctx.target_file = {0} (the buffer is null-fileld, to be filled in while-loop)
	* ctx.exit_code = 0 // else we would exit earlier 
	* */
	

	// if no files specified, use default
	if (!*ctx.target_files)
		*ctx.target_files = default_target_file;

	// while there are files to process, process them
	while (*ctx.target_files) {
		ft_strlcpy(ctx.target_file, *ctx.target_files, sizeof(ctx.target_file));
		if (!ft_nm(&ctx))
			return (EXIT_FAILURE);
		ctx.target_files++;
	}

	// free_data(&ctx);
	return (EXIT_SUCCESS);
}
