#include "ft_nm.h"

/* The passed s_data is not created again everytime, it is
 * the same object passed in main loop for all target files.
 * So upon freeing data, we also have to clear it
 * e.g. ctx->alloc_ptr = NULL;
 * However we still need the target_files pointer (which is just argv)
 * to continue processing the next file. And the total files count
 * */
static bool	free_and_clear_data(struct s_data *ctx)
{
	char	**target_files = ctx->target_files;
	short	total_files = ctx->total_files;

	if (ctx->symbols)
		free(ctx->symbols);
	if (ctx->map.base && !unmap_elf(&ctx->map))
		return false;
	bzero(ctx, sizeof(*ctx));
	ctx->target_files = target_files;
	ctx->total_files = total_files;
	return true;
}

static bool	ft_nm(struct s_data *ctx)
{
	int fd = open(ctx->target_file, O_RDONLY);
	if (fd < 0) {
		LOG_SYS("open(%s, O_RDONLY)", ctx->target_file);
		ft_printf("%s: error: %s: %s\n", PROG_NAME, ctx->target_file, strerror(errno));
		write(1, "\n", 1);
		return false;
	}
	if (ctx->total_files > 1)
		ft_printf("%s\n", ctx->target_file);
	if (!parse_elf(fd, ctx)) {
		write(1, "\n", 1);
		close(fd);
		return false;
	}
	display_symbols(ctx);
	close(fd);
	return free_and_clear_data(ctx);;
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

	return (EXIT_SUCCESS);
}
