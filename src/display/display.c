#include "ft_nm.h"

char	get_symbol_type(struct s_symbol *sym)
{
	(void)sym;
	return '?';
}

void	display_symbols(struct s_symbol *sym, size_t symcount)
{
	uint64_t	symval;

	if (!sym || symcount == 0)
		return;
	for (size_t i = 0; i < symcount; i++, sym++) {
		symval = sym->value;
		if (sym->value)
			printf("%016" PRIx64, symval);
		else
			printf("%.*s", 16, "                ");
		printf(" %c %s\n", get_symbol_type(sym), sym->name);
	}
}
