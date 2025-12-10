#include "nm.h"

static bool	opts_handle(struct s_data *ctx, int opt, char *argv[])
{
	switch(opt) {
	

	// display help dialog
	case 'h':
		print_help();
		return false;

	// Unknown opts
	case '?':
		ERR_MSG("Unkown option '%s'", argv[optind]);
		fprintf(stderr, "Check usage with -h, --help\n");
		ctx->exit_code = false;
		return false;

	// if no option provided
	default:
		return false;
	}

	return true;
}

static void	parse_options(struct s_data *ctx, int argc, char *argv[])
{
	int		opt;
	extern int	opterr;
	const char	*shortopts = "h";
	struct option	longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{NULL, no_argument, NULL, 0}
	};

	opterr = 0;
	while ((opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		if (!opts_handle(ctx, opt, argv)) {
			exit(ctx->exit_code);
		}
	}
}

bool	parse_cli(struct s_data *ctx, int *argc, char *argv[])
{
	parse_options(ctx, *argc, argv);
	*argc -= optind;
	argv += optind;	

	return true;
}
