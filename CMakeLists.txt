project(riscv-meta)
cmake_minimum_required(VERSION 3.1.0)

set(
	libedit_SOURCES
	src/edit/chared.c
	src/edit/common.c
	src/edit/el.c
	src/edit/eln.c
	src/edit/emacs.c
	src/edit/filecomplete.c
	src/edit/hist.c
	src/edit/history.c
	src/edit/historyn.c
	src/edit/keymacro.c
	src/edit/map.c
	src/edit/chartype.c
	src/edit/parse.c
	src/edit/prompt.c
	src/edit/read.c
	src/edit/readline.c
	src/edit/refresh.c
	src/edit/search.c
	src/edit/sig.c
	src/edit/terminal.c
	src/edit/tokenizer.c
	src/edit/tokenizern.c
	src/edit/tty.c
	src/edit/unvis.c
	src/edit/vis.c
	src/edit/vi.c
)

set(
	libexpr_SOURCES
	src/expr/builtin-features.cc
	src/expr/catch.cc
	src/expr/functions.cc
	src/expr/objects.cc
	src/expr/packToken.cc
	src/expr/shunting-yard.cc
)

set(
	libriscv_asm_SOURCES
	src/asm/assembler.cc
	src/asm/disasm.cc
	src/asm/format.cc
	src/asm/jit.cc
	src/asm/meta.cc
	src/asm/strings.cc
)

set(
	libriscv_crypto_SOURCES
	src/crypto/sha512.cc
)

set(
	libriscv_gen_SOURCES
	src/gen/gen-cc.cc
	src/gen/gen-constraints.cc
	src/gen/gen-fpu-test.cc
	src/gen/gen-interp.cc
	src/gen/gen-jit.cc
	src/gen/gen-latex.cc
	src/gen/gen-latex-alt.cc
	src/gen/gen-map.cc
	src/gen/gen-meta.cc
	src/gen/gen-operands.cc
	src/gen/gen-strings.cc
	src/gen/gen-switch.cc
	src/gen/gen-tablegen.cc
)

set(
	libriscv_elf_SOURCES
	src/elf/elf.cc
	src/elf/elf-file.cc
	src/elf/elf-format.cc
)

set(
	libriscv_fmt_SOURCES
	src/fmt/bigint.cc
	src/fmt/dtoa.cc
	src/fmt/hdtoa.cc
	src/fmt/itoa.cc
	src/fmt/fmt.cc
)

set(
	libriscv_model_SOURCES
	src/model/model.cc
)

set(
	libriscv_util_SOURCES
	src/util/base64.cc
	src/util/cmdline.cc
	src/util/color.cc
	src/util/config-string.cc
	src/util/config-parser.cc
	src/util/host.cc
	src/util/util.cc
)

set(
	rv_asm_SOURCES
	src/app/rv-asm.cc
)

set(
	rv_bin_SOURCES
	src/app/rv-compress.cc
	src/app/rv-dump.cc
	src/app/rv-histogram.cc
	src/app/rv-pte.cc
	src/app/rv-bin.cc
)

set(
	rv_meta_SOURCES
	src/app/rv-meta.cc
)

set(
	rv_sim_SOURCES
	src/app/rv-sim.cc
)

set(
	rv_sys_SOURCES
	src/app/rv-sys.cc
)

include_directories(
	src/asm
	src/abi
	src/crypto
	src/edit
	src/elf
	src/emu
	src/expr
	src/fmt
	src/gen
	src/model
	src/rom
	src/util
)

add_library(edit ${libedit_SOURCES})

add_library(expr ${libexpr_SOURCES})
target_compile_features(expr PRIVATE cxx_generic_lambdas)

add_library(riscv_asm ${libriscv_asm_SOURCES})
target_compile_features(riscv_asm PRIVATE cxx_generic_lambdas)

add_library(riscv_crypto ${libriscv_crypto_SOURCES})

add_library(riscv_gen ${libriscv_gen_SOURCES})
target_compile_features(riscv_gen PRIVATE cxx_generic_lambdas)

add_library(riscv_elf ${libriscv_elf_SOURCES})
target_compile_features(riscv_elf PRIVATE cxx_generic_lambdas)

add_library(riscv_fmt ${libriscv_fmt_SOURCES})
target_compile_features(riscv_fmt PRIVATE cxx_generic_lambdas)

add_library(riscv_model ${libriscv_model_SOURCES})
target_compile_features(riscv_model PRIVATE cxx_generic_lambdas)

add_library(riscv_util ${libriscv_util_SOURCES})
target_compile_features(riscv_util PRIVATE cxx_generic_lambdas)

add_executable(rv-asm ${rv_asm_SOURCES})
target_compile_features(rv-asm PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-asm expr riscv_asm riscv_elf riscv_fmt riscv_util)

add_executable(rv-bin ${rv_bin_SOURCES})
target_compile_features(rv-bin PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-bin riscv_asm riscv_elf riscv_fmt riscv_util)

add_executable(rv-meta ${rv_meta_SOURCES})
target_compile_features(rv-meta PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-meta riscv_model riscv_gen riscv_util)

add_executable(rv-sim ${rv_sim_SOURCES})
target_compile_features(rv-sim PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-sim ncurses edit riscv_asm riscv_crypto riscv_elf riscv_fmt riscv_util)

add_executable(rv-sys ${rv_sys_SOURCES})
target_compile_features(rv-sys PRIVATE cxx_generic_lambdas)
target_link_libraries(rv-sys ncurses edit riscv_asm riscv_crypto riscv_elf riscv_fmt riscv_util)
