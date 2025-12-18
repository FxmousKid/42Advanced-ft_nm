#include "ft_nm.h"

void	parse_elf_symbols64(const char *strtab,
			    const Elf64_Sym *sym_elf,
			    struct s_symbol *sym)
{
	sym->name = strtab + sym_elf->st_name;
	sym->value = sym_elf->st_value;
}
