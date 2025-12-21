#include "ft_nm.h"


static char	get_symbol_type(struct s_symbol *sym)
{
	if (sym->is_64_bit == false)
		goto sym32bit_switch;
	switch (ELF64_ST_TYPE(sym->info)) {
	case (STT_NOTYPE):
		return '0';
	default:
		return '?';
	}

sym32bit_switch:
	switch (ELF32_ST_TYPE(sym->info)) {
	case (STT_NOTYPE):
		return '0';
	default:
		return '?';
	}
}

void	display_symbols(struct s_symbol *sym, 
		        size_t symcount, 
			t_ft_nm_option options)
{
	uint64_t	symval;
	(void)options;

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
