#include "nm.h"

//TODO: consider 0 params as searching for './a.out'
int	main(int argc, char *argv[])
{
	struct s_data	ctx = {0};
	char		*target;

	parse_cli(&ctx, &argc, argv);
	while (*ctx.target_files) {
		target = ctx.target_file;
		ft_strlcpy(target, *ctx.target_files, sizeof(target));
		if (!ft_nm(&ctx))
			return (EXIT_FAILURE);
		ctx.target_files++;
	}

	// free_data(&ctx);
	return (EXIT_SUCCESS);
}
