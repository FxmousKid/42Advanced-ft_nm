#include "ft_nm.h"

bool	parse_elf_symbols64 (Elf64_Shdr *symsec, Elf64_Word sh_type)
{
	(void)sh_type;
	(void)symsec;

	return true;
}

