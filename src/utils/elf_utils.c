#include "ft_nm.h"

const Elf32_Shdr	*get_elf32_shdr(struct s_map *map)
{
	const Elf32_Ehdr *hdr = (const Elf32_Ehdr *)map->base;
	const Elf32_Shdr *shdr = (const Elf32_Shdr *)((char *)hdr + hdr->e_shoff);
	return shdr;
}

const Elf64_Shdr	*get_elf64_shdr(struct s_map *map)
{
	const Elf64_Ehdr *hdr = (const Elf64_Ehdr *)map->base;
	const Elf64_Shdr *shdr = (const Elf64_Shdr *)((char *)hdr + hdr->e_shoff);
	return shdr;
}

const Elf32_Shdr	*get_elf32_strtbl_shdr(struct s_map *map)
{
	const Elf32_Ehdr *hdr = (const Elf32_Ehdr *)map->base;
	const Elf32_Shdr *shdr = (const Elf32_Shdr *)((char *)hdr + hdr->e_shoff);
	return shdr + hdr->e_shstrndx;
}

const Elf64_Shdr	*get_elf64_strtbl_shdr(struct s_map *map)
{
	const Elf64_Ehdr *hdr = (const Elf64_Ehdr *)map->base;
	const Elf64_Shdr *shdr = (const Elf64_Shdr *)((char *)hdr + hdr->e_shoff);
	return shdr + hdr->e_shstrndx;
}

const Elf32_Shdr	*get_elf32_sym_sh_parent(struct s_map *map,
						 struct s_symbol *sym)
{
	const Elf32_Ehdr *hdr = (const Elf32_Ehdr *)map->base;

	if (sym->shndx == SHN_UNDEF ||
	    sym->shndx == SHN_ABS ||
	    sym->shndx == SHN_COMMON ||
	    sym->shndx >= SHN_LORESERVE ||
	    sym->shndx >= hdr->e_shnum)
		return NULL;

	const Elf32_Shdr	*shdr = get_elf32_shdr(map);
	const Elf32_Shdr	*sym_shdr = shdr + sym->shndx;
	return sym_shdr;
}

Elf32_Word	get_elf32_sym_parent_sh_type(struct s_map *map, 
					     struct s_symbol *sym)
{
	const Elf32_Shdr *sym_shdr = get_elf32_sym_sh_parent(map, sym);
	if (!sym_shdr)
		return 0;
	return sym_shdr->sh_type;
}


Elf32_Word	get_elf32_sym_parent_sh_flag(struct s_map *map, 
					     struct s_symbol *sym)
{
	const Elf32_Shdr *sym_shdr = get_elf32_sym_sh_parent(map, sym);
	if (!sym_shdr)
		return 0;
	return sym_shdr->sh_flags;
}

const Elf64_Shdr	*get_elf64_sym_sh_parent(struct s_map *map,
						 struct s_symbol *sym)
{
	const Elf64_Ehdr *hdr = (const Elf64_Ehdr *)map->base;

	if (sym->shndx == SHN_UNDEF ||
	    sym->shndx == SHN_ABS ||
	    sym->shndx == SHN_COMMON ||
	    sym->shndx >= SHN_LORESERVE ||
	    sym->shndx >= hdr->e_shnum)
		return NULL;

	const Elf64_Shdr	*shdr = get_elf64_shdr(map);
	const Elf64_Shdr	*sym_shdr = shdr + sym->shndx;
	return sym_shdr;
}

Elf64_Word	get_elf64_sym_parent_sh_type(struct s_map *map, 
					     struct s_symbol *sym)
{
	const Elf64_Shdr *sym_shdr = get_elf64_sym_sh_parent(map, sym);
	if (!sym_shdr)
		return 0;
	return sym_shdr->sh_type;
}


Elf64_Word	get_elf64_sym_parent_sh_flag(struct s_map *map, 
					     struct s_symbol *sym)
{
	const Elf64_Shdr *sym_shdr = get_elf64_sym_sh_parent(map, sym);
	if (!sym_shdr)
		return 0;
	return sym_shdr->sh_flags;
}

bool	is_sym32_global(unsigned char info)
{
	return ELF32_ST_BIND(info) == STB_GLOBAL;
}

bool	is_sym32_local(unsigned char info)
{
	return ELF32_ST_BIND(info) == STB_LOCAL;
}

bool	is_sym32_weak(unsigned char info)
{
	return ELF32_ST_BIND(info) == STB_WEAK;
}

bool	is_sym64_global(unsigned char info)
{
	return ELF64_ST_BIND(info) == STB_GLOBAL;
}

bool	is_sym64_local(unsigned char info)
{
	return ELF64_ST_BIND(info) == STB_LOCAL;
}

bool	is_sym64_weak(unsigned char info)
{
	return ELF64_ST_BIND(info) == STB_WEAK;
}
