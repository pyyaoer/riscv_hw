include_HEADERS = include/read_elf.h \
                   include/vmemory.h \
                   include/inst.h

bin_PROGRAMS = bin/riscv_hw \
                bin/read_elf_test \
                bin/vmemory_test \
                bin/inst_test
bin_riscv_hw_CPPFLAGS = -std=c++11 -Wno-unused-result
bin_riscv_hw_SOURCES = src/read_elf.cc \
                        src/vmemory.cc \
                        src/inst.cc \
                        src/main.cc
bin_read_elf_test_CPPFLAGS = -std=c++11 -Wno-unused-result
bin_read_elf_test_SOURCES = src/read_elf.cc \
                             src/read_elf_test.cc
bin_vmemory_test_CPPFLAGS = -std=c++11 -Wno-unused-result
bin_vmemory_test_SOURCES = src/vmemory.cc \
                            src/vmemory_test.cc
bin_inst_test_CPPFLAGS = -std=c++11 -Wno-unused-result
bin_inst_test_SOURCES = src/inst.cc \
                         src/inst_test.cc
