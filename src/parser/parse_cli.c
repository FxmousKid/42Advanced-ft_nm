#include "ft_nm.h"

/*
  -a
  -g
  -u
  -r
  -p
*/

static bool	opts_handle(struct s_data *ctx, int opt, char *argv[])
{
	switch(opt) {

	// -a: display all symbols
	case 'a':
		ctx->options |= FT_NM_OPT_a;
		break;
	
	case 'g':
		ctx->options |= FT_NM_OPT_g;
		break;
	
	case 'u':
		ctx->options |= FT_NM_OPT_u;
		break;

	// -r : 	
	case 'r':
		ctx->options |= FT_NM_OPT_r;
		break;

	// -p: no sort
	case 'p':
		ctx->options |= FT_NM_OPT_p;
		break;

	// display help dialog
	case 'h':
		print_help();
		return false;

	// Unknown opts
	case '?':
		LOG_MSG("Unkown option '%s'", argv[optind]);
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
	const char	*shortopts = "haprgu";
	struct option	longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"debug-syms", no_argument, NULL, 'a'},
		{"no-sort", no_argument, NULL, 'p'},
		{"extern-only", no_argument, NULL, 'g'},
		{"reverse-sort", no_argument, NULL, 'r'},
		{"undefined-only", no_argument, NULL, 'u'},
		{NULL, no_argument, NULL, 0}
	};

	opterr = 0;
	while ((opt = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		if (!opts_handle(ctx, opt, argv)) {
			exit(ctx->exit_code);
		}
	}
}

static bool	parse_arguments(struct s_data *ctx, char *argv[])
{
	ctx->target_files = argv;
	while (*argv) {
		if (strlen(*argv) > PATH_MAX) {
			LOG_MSG("file name [%s] is too long", *argv)
			return false;
		}
		argv++;
	}
	return true;
}

bool	parse_cli(struct s_data *ctx, int *argc, char *argv[])
{
	parse_options(ctx, *argc, argv);
	*argc -= optind;
	argv += optind;	
	ctx->total_files = *argc;
	if (!parse_arguments(ctx, argv))
		return false;
	return true;
}
