#include "nm.h"

//TODO: consider 0 params as searching for './a.out'
int	main(int argc, char *argv[])
{

	struct s_data ctx = {0};
	parse_cli(&ctx, &argc, argv);

	// free_data(&ctx);
	return (EXIT_SUCCESS);
}
