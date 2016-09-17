#include "read_elf.h"

static void PrintSegmentType(uint32_t p_type) {
  switch (p_type) {
    case PT_NULL: printf("Unused entry"); break;
    case PT_LOAD: printf("Loadable segment"); break;
    case PT_DYNAMIC: printf("Dynamic linking information"); break;
    case PT_INTERP:
      printf("NULL-terminated pathname to invoke as an interpreter"); break;
    case PT_NOTE: printf("Auxiliary information"); break;
    case PT_SHLIB: printf("Reserved"); break;
    case PT_PHDR: printf("Program header itself"); break;
    case PT_LOPROC:
    case PT_HIPROC:
      printf("Reserved for processor-specific sematics"); break;
    case PT_GNU_STACK: printf("GNU extension for states of stack"); break;
    default: printf("Other segment");
  }
}

static void PrintELFType(uint16_t e_type) {
  switch (e_type) {
    case ET_NONE: printf("Unknown Type"); break;
    case ET_REL: printf("Relocatable file"); break;
    case ET_EXEC: printf("Executable file"); break;
    case ET_DYN: printf("Shared object"); break;
    case ET_CORE: printf("Core file"); break;
    default: printf("Other kind");
  }
}

static void PrintELFArchitecture(uint16_t e_machine) {
  switch (e_machine) {
    case EM_NONE: printf("Unknown machine"); break;
    case EM_M32: printf("AT&T WE 32100"); break;
    case EM_SPARC: printf("Sun Microsystems SPARC"); break;
    case EM_386: printf("Intel 80386"); break;
    case EM_68K: printf("Motorola 68000"); break;
    case EM_88K: printf("Motorola 88000"); break;
    case EM_860: printf("Intel 80860"); break;
    case EM_MIPS: printf("MIPS RS3000"); break;
    case EM_PARISC: printf("HP/PA"); break;
    case EM_SPARC32PLUS:
      printf("SPARC with enhanced instruction set"); break;
    case EM_PPC: printf("PowerPC"); break;
    case EM_PPC64: printf("PowerPC 64-bit"); break;
    case EM_S390: printf("IBM S/390"); break;
    case EM_ARM: printf("Advanced RISC Machines"); break;
    case EM_SH: printf("Renesas SuperH"); break;
    case EM_SPARCV9: printf("SPARC v9 64-bit"); break;
    case EM_IA_64: printf("Intel Itanium"); break;
    case EM_X86_64: printf("AMD x86-84"); break;
    case EM_VAX: printf("DEC Vax"); break;
    default: printf("Other architecture");
  }
}

bool ReadELF::AnalyzeELF() {
  int fd = open(file_name_.c_str(), O_RDONLY);
  if (fd < 0){
    printf("File does not exist!\n");
    return false;
  }
  if (!AnalyzeELFHeader(fd)) {
    printf("Format mismatch!\n");
    close(fd);
    return false;
  }
  if (!AnalyzeProgramHeader(fd)) {
    printf("Broken program header!\n");
    close(fd);
    return false;
  }
  if (!AnalyzeSectionHeader(fd)) {
    printf("Broken section header!\n");
    close(fd);
    return false;
  }
  close(fd);
  return true;
}

void ReadELF::PrintELFHeader() {
  switch (elf_ident_[EI_CLASS]) {
    case ELFCLASS32:
        PrintELF32Header();
      break;
    case ELFCLASS64:
        PrintELF64Header();
      break;
    default:
        printf("Bad Header\n");
  }
}

bool ReadELF::AnalyzeELFHeader(int fd) {
  if (lseek(fd, 0, SEEK_SET) < 0) {
    return false;
  }
  read(fd, &elf_ident_, EI_NIDENT);
#ifdef LOG_FOR_TEST
  printf("Print elf_ident[EI_NIDENT]:\n\t");
  for (int i = 0; i < EI_NIDENT; ++i) {
    printf("%02x ", elf_ident_[i]);
  }
  printf("\n");
#endif
  // Confirm the magic numbers
  if (memcmp(elf_ident_, kELFMagic, 4) != 0) {
    return false;
  }
  if (lseek(fd, 0, SEEK_SET) < 0) {
    return false;
  }
  // Identify the architecture for this ELF.
  switch (elf_ident_[EI_CLASS]) {
    case ELFCLASS32:
      read(fd, &elf32_hdr_, sizeof(elf32_hdr_));
      break;
    case ELFCLASS64:
      read(fd, &elf64_hdr_, sizeof(elf64_hdr_));
      break;
    default:
      return false;
  }
#ifdef LOG_FOR_TEST
  printf("\nThe ELF Header.\n");
  PrintELFHeader();
#endif
  return true;
}

bool ReadELF::AnalyzeProgramHeader(int fd) {
  switch (elf_ident_[EI_CLASS]) {
    case ELFCLASS32:
      return AnalyzeProgramHeader32(fd);
    case ELFCLASS64:
      return AnalyzeProgramHeader64(fd);
    default:
      return false;
  }
  return true;
}

bool ReadELF::AnalyzeSectionHeader(int fd) {
  switch (elf_ident_[EI_CLASS]) {
    case ELFCLASS32:
      return AnalyzeSectionHeader32(fd);
    case ELFCLASS64:
      return AnalyzeSectionHeader64(fd);
    default:
      return false;
  }
  return true;
}

bool ReadELF::AnalyzeProgramHeader32(int fd) {
  uint32_t offset = elf32_hdr_.e_phoff;
  elf32_program_hdr_.clear();
  for (int i = 0; i < elf32_hdr_.e_phnum; ++i) {
    if (lseek(fd, offset, SEEK_SET) < 0) {
      return false;
    }
    Elf32_Phdr hdr = {0};
    read(fd, &hdr, elf32_hdr_.e_phentsize);
#ifdef LOG_FOR_TEST
    printf("\nThe %dth Program Header.\n", i+1);
    PrintProgramHeader32(&hdr);
#endif
    elf32_program_hdr_.push_back(hdr);
    offset += elf32_hdr_.e_phentsize;
  }
  return true;
}

bool ReadELF::AnalyzeProgramHeader64(int fd) {
  uint64_t offset = elf64_hdr_.e_phoff;
  elf64_program_hdr_.clear();
  for (int i = 0; i < elf64_hdr_.e_phnum; ++i) {
    if (lseek(fd, offset, SEEK_SET) < 0) {
      return false;
    }
    Elf64_Phdr hdr = {0};
    read(fd, &hdr, elf64_hdr_.e_phentsize);
#ifdef LOG_FOR_TEST
    printf("\nThe %dth Program Header.\n", i+1);
    PrintProgramHeader64(&hdr);
#endif
    elf64_program_hdr_.push_back(hdr);
    offset += elf64_hdr_.e_phentsize;
  }
  return true;
}

void ReadELF::PrintProgramHeader32(Elf32_Phdr *hdr) {
  printf("p_type: \t");
  PrintSegmentType(hdr->p_type);
  printf("\np_offset:\t%u", static_cast<uint32_t>(hdr->p_offset));
  printf("\np_vaddr:\t%#08x", static_cast<uint32_t>(hdr->p_vaddr));
  printf("\np_paddr:\t%#08x", static_cast<uint32_t>(hdr->p_paddr));
  printf("\np_filesz:\t%u", static_cast<uint32_t>(hdr->p_filesz));
  printf("\np_memsz:\t%u", static_cast<uint32_t>(hdr->p_memsz));
  printf("\np_flags:\t%u", static_cast<uint32_t>(hdr->p_flags));
  printf("\np_align:\t%u", static_cast<uint32_t>(hdr->p_align));
  printf("\n");
}

void ReadELF::PrintProgramHeader64(Elf64_Phdr *hdr) {
  printf("p_type: \t");
  PrintSegmentType(hdr->p_type);
  printf("\np_offset:\t%lu", static_cast<uint64_t>(hdr->p_offset));
  printf("\np_vaddr:\t%#016lx", static_cast<uint64_t>(hdr->p_vaddr));
  printf("\np_paddr:\t%#016lx", static_cast<uint64_t>(hdr->p_paddr));
  printf("\np_filesz:\t%lu", static_cast<uint64_t>(hdr->p_filesz));
  printf("\np_memsz:\t%lu", static_cast<uint64_t>(hdr->p_memsz));
  printf("\np_flags:\t%lu", static_cast<uint64_t>(hdr->p_flags));
  printf("\np_align:\t%lu", static_cast<uint64_t>(hdr->p_align));
  printf("\n");
}

bool ReadELF::AnalyzeSectionHeader32(int fd) {
  return true;
}

bool ReadELF::AnalyzeSectionHeader64(int fd) {
  return true;
}

void ReadELF::PrintELF32Header() {
  printf("e_ident:\t");
  for (int i = 0; i < EI_NIDENT; ++i) {
    printf("%02x ", elf32_hdr_.e_ident[i]);
  }
  printf("\ne_type: \t");
  PrintELFType(elf32_hdr_.e_type);
  printf("\ne_machine:\t");
  PrintELFArchitecture(elf32_hdr_.e_machine);
  printf("\ne_version:\t");
  switch (elf32_hdr_.e_version) {
    case EV_NONE: printf("Invalid version"); break;
    case EV_CURRENT: printf("Current version"); break;
    default: printf("Other version");
  }
  printf("\ne_entry:\t%#08x", static_cast<uint32_t>(elf32_hdr_.e_entry));
  printf("\ne_phoff:\t%u", static_cast<uint32_t>(elf32_hdr_.e_phoff));
  printf("\ne_shoff:\t%u", static_cast<uint32_t>(elf32_hdr_.e_shoff));
  printf("\ne_flags:\t%u", static_cast<uint32_t>(elf32_hdr_.e_flags));
  printf("\ne_ehsize:\t%u", static_cast<uint32_t>(elf32_hdr_.e_ehsize));
  printf("\ne_phentsize:\t%u",
         static_cast<uint32_t>(elf32_hdr_.e_phentsize));
  printf("\ne_phnum:\t%u", static_cast<uint32_t>(elf32_hdr_.e_phnum));
  printf("\ne_shentsize:\t%u",
         static_cast<uint32_t>(elf32_hdr_.e_shentsize));
  printf("\ne_shnum:\t%u", static_cast<uint32_t>(elf32_hdr_.e_shnum));
  printf("\ne_shstrndx:\t%u",
         static_cast<uint32_t>(elf32_hdr_.e_shstrndx));
  printf("\n");
}

void ReadELF::PrintELF64Header() {
  printf("e_ident:\t");
  for (int i = 0; i < EI_NIDENT; ++i) {
    printf("%02x ", elf64_hdr_.e_ident[i]);
  }
  printf("\ne_type: \t");
  PrintELFType(elf64_hdr_.e_type);
  printf("\ne_machine:\t");
  PrintELFArchitecture(elf64_hdr_.e_machine);
  printf("\ne_version:\t");
  switch (elf64_hdr_.e_version) {
    case EV_NONE: printf("Invalid version"); break;
    case EV_CURRENT: printf("Current version"); break;
    default: printf("Other version");
  }
  printf("\ne_entry:\t%#016lx", static_cast<uint64_t>(elf64_hdr_.e_entry));
  printf("\ne_phoff:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_phoff));
  printf("\ne_shoff:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_shoff));
  printf("\ne_flags:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_flags));
  printf("\ne_ehsize:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_ehsize));
  printf("\ne_phentsize:\t%lu",
         static_cast<uint64_t>(elf64_hdr_.e_phentsize));
  printf("\ne_phnum:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_phnum));
  printf("\ne_shentsize:\t%lu",
         static_cast<uint64_t>(elf64_hdr_.e_shentsize));
  printf("\ne_shnum:\t%lu", static_cast<uint64_t>(elf64_hdr_.e_shnum));
  printf("\ne_shstrndx:\t%lu",
         static_cast<uint64_t>(elf64_hdr_.e_shstrndx));
  printf("\n");
}
