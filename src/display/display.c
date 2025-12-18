#include "ft_nm.h"

void	display_symbols(struct s_symbol *sym, size_t symcount)
{
	if (!sym || symcount == 0)
		return;
	for (size_t i = 0; i < symcount; i++, sym++) {
		if (sym->value)
			printf("%016" PRIx64 " ? %s\n", (uint64_t)sym->value, sym->name);
		else
			printf("%.*s ? %s\n", 16, "                ", sym->name);
	}
}
