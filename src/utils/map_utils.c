#include "ft_nm.h"

bool	m_inbounds(struct s_map *map, size_t offset, size_t size)
{
	if (offset > map->size || size > map->size)
		return false;
	return (offset + size <= map->size);
}

bool	unmap_elf(struct s_map *map)
{
	if (map->base && map->base != MAP_FAILED)
		if (munmap(map->base, map->size) < 0)
			LOG_SYS("munmap")
	map->size = 0;
	map->base = NULL;
	return true;
}

bool	map_elf(struct s_map *map, int fd)
{
	struct stat st = {0};

	if (fstat(fd, &st) < 0) {
		LOG_SYS("fstat(%d, &st)", fd)
		return false;
	}
	map->size = st.st_size;
	map->base = mmap(NULL, map->size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map->base == MAP_FAILED) {
		LOG_SYS("mmap")
		return false;
	}
	return true;
}
