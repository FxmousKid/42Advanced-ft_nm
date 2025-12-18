#include "ft_nm.h"

static bool	parse_elf32(int fd)
{
	ssize_t		read_qty = 0;
	Elf32_Ehdr	hdr = {0};

	if ((read_qty = read(fd, &hdr, sizeof(hdr))) < (ssize_t)sizeof(hdr)) {
		LOG_MSG("couldn't read header, instead read %zu", read_qty)
		return false;
	}

	return (true);
}

static bool	parse_elf64(struct s_map *map, struct s_data *ctx)
{
	const Elf64_Ehdr*	hdr;
	const Elf64_Shdr*	shdr;
	int			idx = 0;

	if (!m_inbounds(map, 0, sizeof(Elf64_Ehdr))) {
		LOG_MSG("File doesn't have a ELF header");
		return false;
	}

	// Parse ELF header
	hdr = (const Elf64_Ehdr *)map->base;
	if (!hdr->e_shnum || !hdr->e_shoff) {
		LOG_MSG("No section header in ELF found")
		return false;
	}
	shdr = (const Elf64_Shdr *)(map->base + hdr->e_shoff);

	// Get .symtab section
	const Elf64_Shdr *tmp = NULL, *symtab_shdr = NULL;
	while (shdr && ++idx < hdr->e_shnum) {
		tmp = shdr + idx;
		if (tmp->sh_type == SHT_SYMTAB)
			symtab_shdr = tmp;
	}
	if (!symtab_shdr) {
		LOG_MSG("Symbol table not found")
		return false;
	}

	size_t symcount = symtab_shdr->sh_size / symtab_shdr->sh_entsize;
	ctx->symbols = ft_calloc(symcount + 1, sizeof(*ctx->symbols));
	if (!ctx->symbols) {
		LOG_SYS("ft_calloc(%zu, %zu)", symcount + 1, sizeof(*ctx->symbols));
		return false;
	}

	const Elf64_Shdr *strtbl_shdr = shdr + symtab_shdr->sh_link;
	const Elf64_Sym *sym_cur = (const Elf64_Sym *)(map->base + symtab_shdr->sh_offset);
	const Elf64_Sym *sym_end = sym_cur + symcount;
	const char *strtab = (const char *)(map->base + strtbl_shdr->sh_offset);
	idx = 0;
	while (sym_cur < sym_end) {
		if (sym_cur->st_name != 0)
			parse_elf_symbols64(strtab, sym_cur, ctx->symbols + idx++);
		sym_cur++;
	}
	ctx->sym_count = idx;

	return (true);
}

bool	parse_elf(int fd, struct s_data *ctx)
{
	struct s_map		*map;
	const unsigned char	*elf_ident;
	bool			ret = true;

	if (!map_elf(&ctx->map, fd))
		return false;
	map = &ctx->map;

	if (!m_inbounds(map, 0, EI_NIDENT)) {
		LOG_MSG("File too small : %zu", map->size);
		unmap_elf(map);
		return false;
	}

	elf_ident = (const unsigned char *)map->base;
	if (memcmp(elf_ident, ELFMAG, SELFMAG)) {
		LOG_MSG("Not a ELF file")
		unmap_elf(map);
		return false;
	}

	switch(elf_ident[EI_CLASS]) {
	case (ELFCLASS64):
		ret = parse_elf64(map, ctx);
		break;
	case (ELFCLASS32):
		ret = parse_elf32(fd);
		break;
	default:
		ret = false;
		LOG_MSG("Unknown Architecture : %x", elf_ident[EI_CLASS])
	}
	return ret;
}
