#include "ft_nm.h"

void	check_opts(t_ft_nm_option options)
{
	if (FT_NM_EXTRACT_OPT(options, FT_NM_OPT_p)) {
		ft_printf("p options is set\n");
	}
	if (FT_NM_EXTRACT_OPT(options, FT_NM_OPT_r)) {
		ft_printf("r options is set\n");
	}
	if (FT_NM_EXTRACT_OPT(options, FT_NM_OPT_g)) {
		ft_printf("q options is set\n");
	}
	if (FT_NM_EXTRACT_OPT(options, FT_NM_OPT_u)) {
		ft_printf("u options is set\n");
	}
}

static char	get_symbol_type(struct s_data *ctx, struct s_symbol *sym)
{
	Elf64_Word	sh_flags64;
	Elf64_Word	sh_type64;

	if (sym->is_64_bit == false)
		goto sym32bit_switch;

	sh_flags64 = get_elf64_sym_parent_sh_flag(&ctx->map, sym);
	sh_type64 = get_elf64_sym_parent_sh_type(&ctx->map, sym);

	// "U"
	if (sym->shndx == SHN_UNDEF && is_sym64_global(sym->info))
		return FT_NM_SYM_UNDEF;

	// "t"
	if (sh_type64 == SHT_PROGBITS && is_sym64_local(sym->info) &&
	    sh_flags64 & SHF_ALLOC && sh_flags64 & SHF_EXECINSTR)
		return FT_NM_TEXT_SECTION_LOCAL;

	// "T"
	if (sh_type64 == SHT_PROGBITS && is_sym64_global(sym->info) &&
	    sh_flags64 & SHF_ALLOC && sh_flags64 & SHF_EXECINSTR)
		return FT_NM_TEXT_SECTION_GLOBAL;

	// "d"
	if ((sh_type64 == SHT_PROGBITS || sh_type64 == SHT_INIT_ARRAY || 
	     sh_type64 == SHT_FINI_ARRAY || sh_type64 == SHT_DYNAMIC) &&
	    is_sym64_local(sym->info) && sh_flags64 & SHF_ALLOC &&
	    sh_flags64 & SHF_WRITE)
		return FT_NM_SYM_DATA_LOCAL;

	// "D"
	if (sh_type64 == SHT_PROGBITS && is_sym64_global(sym->info) &&
	    sh_flags64 & SHF_ALLOC && sh_flags64 & SHF_WRITE)
		return FT_NM_SYM_DATA_GLOBAL;

	// "b"
	if (sh_type64 == SHT_NOBITS && sh_flags64 & SHF_ALLOC &&
	    sh_flags64 & SHF_WRITE && is_sym64_local(sym->info))
		return FT_NM_SYM_BSS_LOCAL;

	// "B"
	if (sh_type64 == SHT_NOBITS && sh_flags64 & SHF_ALLOC &&
	    sh_flags64 & SHF_WRITE && is_sym64_global(sym->info))
		return FT_NM_SYM_BSS_GLOBAL;

	// "w"
	if (sym->shndx == SHN_UNDEF && is_sym64_weak(sym->info))
		return FT_NM_SYM_WEAK_LOCAL;

	// "W"
	if (sym->shndx != SHN_UNDEF &&
	    is_sym64_weak(sym->info))
		return FT_NM_SYM_WEAK_GLOBAL;

	// "r"
	if ((sh_type64 == SHT_PROGBITS || sh_type64 == SHT_NOTE) &&
	    sh_flags64 & SHF_ALLOC && is_sym64_local(sym->info))
		return FT_NM_SYM_READ_ONLY_LOCAL;

	// "R"
	if ((sh_type64 == SHT_PROGBITS || sh_type64 == SHT_NOTE) &&
	    sh_flags64 & SHF_ALLOC && is_sym64_global(sym->info))
		return FT_NM_SYM_READ_ONLY_GLOBAL;

	// "0"
	if (ELF64_ST_TYPE(sym->info) == STT_NOTYPE)
		return FT_NM_STT_NO_TYPE;

	// "a"
	if (ELF64_ST_TYPE(sym->info) == STT_FILE)
		return FT_NM_STT_FILE;

	printf("\nsym->shndx = %d, bind = %x, info = %x, sh_flag = %x, sh_type = %x\n", \
		sym->shndx, ELF64_ST_BIND(sym->info), ELF64_ST_TYPE(sym->info), sh_flags64, sh_type64);
	fflush(stdout);

	return '?';

sym32bit_switch:
	if (get_elf64_sym_parent_sh_type(&ctx->map, sym) == SHT_NOBITS)
		return FT_NM_SYM_BSS_LOCAL;
	if (sym->shndx == SHN_UNDEF)
		return FT_NM_SYM_UNDEF;
	if (get_elf32_sym_parent_sh_type(&ctx->map, sym) == SHT_PROGBITS)
		return FT_NM_TEXT_SECTION_LOCAL;
	if (ELF32_ST_TYPE(sym->info) == STT_NOTYPE)
		return FT_NM_STT_NO_TYPE;
	if (ELF32_ST_TYPE(sym->info) == STT_FILE)
		return FT_NM_STT_FILE;
	return '?';

}

bool	can_display_symbol(struct s_data *ctx, struct s_symbol *sym)
{
	t_ft_nm_option	options = ctx->options;

	if (FT_NM_EXTRACT_OPT(options, FT_NM_OPT_a))
		return true;

	if (get_symbol_type(ctx, sym) == FT_NM_STT_FILE)
		return false;

	if (*sym->name == '\0')
		return false;
	return true;
}

void	find_sections(struct s_data *ctx, char *buf, struct s_symbol *sym)
{
	const Elf64_Shdr *shdr = get_elf64_shdr(&ctx->map);
	const Elf64_Shdr *sym_shdr = shdr + sym->shndx;

	if (sym->shndx == SHN_UNDEF) {
		strcpy(buf, "UNDEF");
		return ;
	}
	if (sym->shndx == SHN_ABS) {
		strcpy(buf, "ABS");
		return ;
	}
	if (sym->shndx == SHN_COMMON) {
		strcpy(buf, "COMMON");
		return ;
	}


	const Elf64_Shdr *strtbl_shdr = get_elf64_strtbl_shdr(&ctx->map);
	const char *sym_section_name = (const char *)(ctx->map.base) + strtbl_shdr->sh_offset + sym_shdr->sh_name;
	if (!sym_section_name)
		return ;
	strcpy(buf, sym_section_name);
}

void	display_symbols(struct s_data *ctx)
{
	size_t		symcount = ctx->sym_count;
	uint64_t	symval = 0;
	struct s_symbol	*sym = ctx->symbols;

	if (!sym || symcount == 0)
		return;

	for (size_t i = 0; i < symcount; i++, sym++) {
		symval = sym->value;
		if (!can_display_symbol(ctx, sym))
			continue;
		if (symval) {
			printf("%016" PRIx64, symval);
			fflush(stdout);
		}
		else {
			printf("%.*s", 16, "                ");
			fflush(stdout);
		}
		// char buf[4096] = {0};
		// find_sections(ctx, buf, sym);
		// ft_printf(" %c %s -- from section '%s'\n", get_symbol_type(ctx, sym), sym->name, buf);
		ft_printf(" %c %s\n", get_symbol_type(ctx, sym), sym->name);
	}
}
