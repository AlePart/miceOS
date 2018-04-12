#ifndef ELF_CHECKER_H
#define ELF_CHECKER_H
#include "elf_data_types.h"

bool elf_check_supported(Elf32_Ehdr *hdr);

void ERROR(uint8_t*); // must be implemented and relocated in error management

#endif
