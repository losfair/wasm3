M3_SOURCES := \
	../../source/m3_bind.o \
	../../source/m3_code.o \
	../../source/m3_compile.o \
	../../source/m3_core.o \
	../../source/m3_emit.o \
	../../source/m3_env.o \
	../../source/m3_exec.o \
	../../source/m3_info.o \
	../../source/m3_module.o \
	../../source/m3_parse.o
VIOLET_TOOLS := /home/zhy/Projects/Violet/Scripts
SYSROOT := /home/zhy/Projects/riscv-newlib/build/riscv32-unknown-elf
CC := clang
SHARED_CFLAGS := -target riscv32-unknown-elf -nostdlib \
	--sysroot $(SYSROOT) \
	-I../../source \
	-Os -Wall -march=rv32im -mabi=ilp32
CFLAGS := $(SHARED_CFLAGS)
ASFLAGS := $(SHARED_CFLAGS)

all: m3 main.o polyfill_undefined.o entry.o
	ld.lld -o build.elf \
		-T./linker.ld \
		--sysroot $(SYSROOT) \
		-L$(SYSROOT)/lib \
		$(M3_SOURCES) main.o polyfill_undefined.o entry.o \
		-lc -lm -error-limit=0 \
		--static
	riscv64-unknown-elf-objcopy ./build.elf \
		--strip-all \
		-O binary firmware.bin
	python3 $(VIOLET_TOOLS)/im_encode.py < firmware.bin > im.txt
	python3 $(VIOLET_TOOLS)/hexencode.py < firmware.bin > dm.txt

m3: $(M3_SOURCES)

clean:
	find ../.. -name "*.o" -exec rm '{}' ';'

.PHONY: clean