project(rv8)
cmake_minimum_required(VERSION 3.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(ASMJIT_STATIC true)
add_subdirectory(third_party/asmjit)

set(libriscv_asm_SOURCES
	src/asm/assembler.cc
	src/asm/disasm.cc
	src/asm/format.cc
	src/asm/jit.cc
	src/asm/meta.cc
	src/asm/strings.cc)

set(libriscv_gen_SOURCES
	src/gen/gen-cc.cc
	src/gen/gen-constraints.cc
	src/gen/gen-fpu-test.cc
	src/gen/gen-interp.cc
	src/gen/gen-jit.cc
	src/gen/gen-latex.cc
	src/gen/gen-latex-alt.cc
	src/gen/gen-map.cc
	src/gen/gen-meta.cc
	src/gen/gen-markdown.cc
	src/gen/gen-operands.cc
	src/gen/gen-strings.cc
	src/gen/gen-switch.cc
	src/gen/gen-tablegen.cc)

set(libriscv_elf_SOURCES
	src/elf/elf.cc
	src/elf/elf-file.cc
	src/elf/elf-format.cc)

set(libriscv_model_SOURCES
	src/model/model.cc)

set(libriscv_util_SOURCES
	src/util/base64.cc
	src/util/bigint.cc
	src/util/cmdline.cc
	src/util/color.cc
	src/util/dtoa.cc
	src/util/fmt.cc
	src/util/hdtoa.cc
	src/util/host.cc
	src/util/itoa.cc
	src/util/util.cc
	src/util/sha512.cc)

set(rv_bin_SOURCES
	src/app/rv-dump.cc
	src/app/rv-histogram.cc
	src/app/rv-pte.cc
	src/app/rv-bin.cc)

include_directories(
	src/asm
	src/abi
	src/elf
	src/emu
	src/gen
	src/hash
	src/jit
	src/mem
	src/model
	src/rom
	src/util
	third_party/asmjit/src/asmjit)

if(UNIX AND NOT APPLE)
	set(mmap_SOURCES
		src/mem/mmap-core.c
		src/mem/mmap-linux.c)
	add_library(mmap-linux SHARED ${mmap_SOURCES})
	set(mmap_LIB mmap-linux)
	link_libraries("-pie -Wl,-Ttext-segment=0x7ffe00000000")
endif()

if(APPLE)
	set(mmap_SOURCES
		src/mem/mmap-core.c
		src/mem/mmap-macos.c)
	add_library(mmap-macos SHARED ${mmap_SOURCES})
	set(mmap_LIB mmap-macos)
	link_libraries("-Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x7ffe00000000")
endif()

add_library(riscv_asm ${libriscv_asm_SOURCES})
target_compile_features(riscv_asm PRIVATE cxx_generic_lambdas)

add_library(riscv_gen ${libriscv_gen_SOURCES})
target_compile_features(riscv_gen PRIVATE cxx_generic_lambdas)

add_library(riscv_elf ${libriscv_elf_SOURCES})
target_compile_features(riscv_elf PRIVATE cxx_generic_lambdas)

add_library(riscv_model ${libriscv_model_SOURCES})
target_compile_features(riscv_model PRIVATE cxx_generic_lambdas)

add_library(riscv_util ${libriscv_util_SOURCES})
target_compile_features(riscv_util PRIVATE cxx_generic_lambdas)

add_executable(rv-bin ${rv_bin_SOURCES})
target_compile_features(rv-bin PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-bin riscv_asm riscv_elf riscv_util)

add_executable(rv-meta src/app/rv-meta.cc)
target_compile_features(rv-meta PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-meta riscv_model riscv_gen riscv_util)

add_executable(rv-sys src/app/rv-sys.cc)
target_compile_features(rv-sys PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-sys ncurses riscv_asm riscv_elf riscv_util)

add_executable(rv-sim src/app/rv-sim.cc)
target_compile_features(rv-sim PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-sim ncurses riscv_asm riscv_elf riscv_util ${mmap_LIB})

add_executable(rv-jit src/app/rv-jit.cc)
target_compile_features(rv-jit PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-jit riscv_asm riscv_elf riscv_util asmjit ${mmap_LIB})
