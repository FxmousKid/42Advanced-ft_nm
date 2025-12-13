#include "nm.h"

//TODO: consider 0 params as searching for './a.out'
int	main(int argc, char *argv[])
{
	struct s_data	ctx = {0};

	parse_cli(&ctx, &argc, argv);
	while (*ctx.target_files) {
		ft_strlcpy(ctx.target_file, *ctx.target_files, sizeof(ctx.target_file));
		if (!ft_nm(&ctx))
			return (EXIT_FAILURE);
		ctx.target_files++;
	}

	// free_data(&ctx);
	return (EXIT_SUCCESS);
}
