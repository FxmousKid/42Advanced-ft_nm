#include "ft_nm.h"

void	parse_elf_symbols64(const char *strtab,
			    const Elf64_Sym *sym_elf,
			    struct s_symbol *sym)
{
	sym->shndx = sym_elf->st_shndx;
	sym->name = strtab + sym_elf->st_name;
	sym->value = sym_elf->st_value;
	sym->info = sym_elf->st_info;
	sym->is_64_bit = true;
}
