#include "elf_loader.h"

#define ELF_RELOC_ERR 1
#define ELF_RELOC_OK 0

static inline void *elf_load_rel(Elf32_Ehdr *hdr) {
	int result;
	result = ELF_RELOC_OK;//elf_load_stage1(hdr); //TODO reallocator;
	if(result == ELF_RELOC_ERR) {
		ERROR("Unable to load ELF file.\n");
		return NULL;
	}
	result = ELF_RELOC_OK;//elf_load_stage2(hdr);
	if(result == ELF_RELOC_ERR) {
		ERROR("Unable to load ELF file.\n");
		return NULL;
	}
	// TODO : Parse the program header (if present)
	return (void *)NULL;//hdr->e_entry; TODO new value
}
 
void *elf_load_file(void *file) {
	Elf32_Ehdr *hdr = (Elf32_Ehdr *)file;
	if(!elf_check_supported(hdr)) {
		ERROR("ELF File cannot be loaded.\n");
		return;
	}
	switch(hdr->e_type) {
		case ET_EXEC:
			// TODO : Implement
			return NULL;
		case ET_REL:
			return elf_load_rel(hdr);
	}
	return NULL;
}
