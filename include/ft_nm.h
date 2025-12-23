#ifndef NM_H
# define NM_H

# include <stdio.h> // IWYU pragma: keep
# include <fcntl.h>
# include <stdlib.h> // IWYU pragma: keep
# include <unistd.h>
# include <errno.h> // IWYU pragma: keep
# include <string.h> // IWYU pragma: keep
# include <stdbool.h>
# include <getopt.h>
# include <elf.h>
# include <inttypes.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include "libft.h" // IWYU pragma: keep

# define PROG_NAME "ft_nm"
# define STRINGIFY(x) #x
# define TOSTRING(x) STRINGIFY(x)
# define DBG __FILE__ " @ " TOSTRING(__LINE__)
# define LOG_SYS(...) do {write(get_logfd(), "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_perr(DBG); } while (0);
# define LOG_MSG(...) do {write(get_logfd(), "ft_nm: ", 7); dprintf(get_logfd(), __VA_ARGS__); _write_err(DBG);  } while (0);

# ifndef DEFAULT_TARGET_FILE
#  define DEFAULT_TARGET_FILE "a.out"
# endif

# define FT_NM_OPT_a 1 << 0
# define FT_NM_OPT_g 1 << 1
# define FT_NM_OPT_u 1 << 2
# define FT_NM_OPT_r 1 << 3
# define FT_NM_OPT_p 1 << 4

# define FT_NM_EXTRACT_OPT(var, opt) (var & opt)

# define FT_NM_STT_NO_TYPE '0'
# define FT_NM_SYM_UNDEF 'U'
# define FT_NM_STT_FILE 'a' 
# define FT_NM_TEXT_SECTION_LOCAL 't'
# define FT_NM_TEXT_SECTION_GLOBAL 'T'
# define FT_NM_SYM_BSS_LOCAL 'b'
# define FT_NM_SYM_BSS_GLOBAL 'B'
# define FT_NM_SYM_WEAK_LOCAL 'w'
# define FT_NM_SYM_WEAK_GLOBAL 'W'
# define FT_NM_SYM_READ_ONLY_LOCAL 'r'
# define FT_NM_SYM_READ_ONLY_GLOBAL 'R'
# define FT_NM_SYM_DATA_LOCAL 'd'
# define FT_NM_SYM_DATA_GLOBAL 'D'


typedef uint8_t t_ft_nm_option;

struct s_map {
	size_t		size;
	unsigned char	*base;
};

struct s_symbol {
	uint16_t	shndx;
	const char	*name;
	/** @brief Storing both 32 and 64-bit addresses since
	*          they both fit in Elf64_Addr */
	Elf64_Addr	value;
	/** @brief stores both 32 and 64_bit st_info field
	*          needs a flag variable to know which macro to call. */
	unsigned char	info;
	/** @brief true if we're on 64-bit, false if not (then 32-bit) */
	bool		is_64_bit;
};

struct s_data {
	uint8_t		options;
	struct s_map	map;
	size_t		sym_count;
	/** @brief terminated by a 0-initialized s_symbol. */
	struct s_symbol	*symbols;
	char		**target_files;
	char		target_file[PATH_MAX + 1];
	short		total_files;
	bool		exit_code;
};

// Utils
int		get_logfd(void);
void		_write_perr(const char *location);
void		_write_err(const char *location);
void		print_help(void);

// elf utils
const Elf32_Shdr	*get_elf32_shdr(struct s_map *map);
const Elf32_Shdr	*get_elf32_strtbl_shdr(struct s_map *map);
Elf32_Word		get_elf32_sym_parent_sh_type(struct s_map *map,
						     struct s_symbol *sym);
Elf32_Word		get_elf32_sym_parent_sh_flag(struct s_map *map, 
						     struct s_symbol *sym);
bool			is_sym32_local(unsigned char info);
bool			is_sym32_global(unsigned char info);
bool			is_sym32_weak(unsigned char info);


const Elf64_Shdr	*get_elf64_shdr(struct s_map *map);
const Elf64_Shdr	*get_elf64_strtbl_shdr(struct s_map *map);
Elf64_Word		get_elf64_sym_parent_sh_type(struct s_map *map,
						     struct s_symbol *sym);
Elf64_Word		get_elf64_sym_parent_sh_flag(struct s_map *map, 
						     struct s_symbol *sym);
bool			is_sym64_local(unsigned char info);
bool			is_sym64_global(unsigned char info);
bool			is_sym64_weak(unsigned char info);

// Map utils
bool		m_inbounds(struct s_map *map, size_t offset, size_t size);
bool		unmap_elf(struct s_map *map);
bool		map_elf(struct s_map *map, int fd);

// Parsing
bool		parse_cli(struct s_data *ctx, int *argc, char *argv[]);
void		parse_elf_symbols64(const char *strtab,
				    const Elf64_Sym *sym_elf,
				    struct s_symbol *sym);

// ft_nm
void		display_symbols(struct s_data *ctx);
bool		parse_elf(int fd, struct s_data *ctx);

#endif
