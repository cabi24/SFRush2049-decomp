# ============================================================
# San Francisco Rush 2049 (N64) Decompilation Makefile
# ============================================================

# Version selection (only US supported currently)
VERSION ?= us
$(if $(filter $(VERSION),us),,$(error Invalid VERSION: $(VERSION). Valid: us))

# Build configuration
NON_MATCHING ?= 0
COMPARE      ?= 1
VERBOSE      ?= 0

# Compiler selection: ido (matching) or gcc (non-matching)
# ido - Uses SGI IDO 5.3 compiler (required for matching builds)
# gcc - Uses mips-linux-gnu-gcc (faster iteration, non-matching)
COMPILER     ?= ido
$(if $(filter $(COMPILER),ido gcc),,$(error Invalid COMPILER: $(COMPILER). Valid: ido gcc))

# Directories
BUILD_DIR    := build/$(VERSION)
SRC_DIR      := src
ASM_DIR      := asm/$(VERSION)
ASSETS_DIR   := assets/$(VERSION)
INCLUDE_DIR  := include
TOOLS_DIR    := tools

# ROM files
BASEROM      := baserom.$(VERSION).z64
TARGET       := $(BUILD_DIR)/rush2049.$(VERSION).z64
ELF          := $(BUILD_DIR)/rush2049.$(VERSION).elf
LD_SCRIPT    := rush2049.$(VERSION).ld

# Tools
CROSS        ?= mips-linux-gnu-
AS           := $(CROSS)as
LD           := $(CROSS)ld
OBJCOPY      := $(CROSS)objcopy
OBJDUMP      := $(CROSS)objdump
PYTHON       := python3

# Compiler-specific configuration
ifeq ($(COMPILER),ido)
  # IDO 5.3 recompiled compiler (matching builds)
  IDO_ROOT     := $(TOOLS_DIR)/ido-static-recomp/build/out
  CC           := $(IDO_ROOT)/cc
  ACPP         := $(IDO_ROOT)/acpp
  COPT         := $(IDO_ROOT)/copt
  MIPSISET     := -mips2
  OPT_FLAGS    := -O2
else
  # GCC compiler (non-matching builds)
  CC           := $(CROSS)gcc
  MIPSISET     := -mips3
  OPT_FLAGS    := -O2
  NON_MATCHING := 1
endif

# Assembler flags
ASFLAGS      := -march=vr4300 -mabi=32 -I$(INCLUDE_DIR)

# C compiler flags
# Common include paths and defines
INCLUDE_CFLAGS := -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/PR -D_LANGUAGE_C

ifeq ($(COMPILER),ido)
  # IDO compiler flags (matching builds)
  # -G 0: No global pointer optimization
  # -Wab,-r4300_mul: Use R4300 multiply instructions
  # -non_shared: Don't use shared libraries
  CFLAGS := -G 0 $(MIPSISET) $(OPT_FLAGS) -non_shared \
            $(INCLUDE_CFLAGS) \
            -Wab,-r4300_mul -Xcpluscomm
else
  # GCC compiler flags (non-matching builds)
  # -march=vr4300: MIPS VR4300 (N64 CPU)
  # -mabi=32: Use 32-bit ABI (o32)
  # -mno-abicalls: No PIC/ABI call sequences
  # -fno-PIC: No position independent code
  CFLAGS := -march=vr4300 -mabi=32 -G 0 -mno-abicalls -fno-PIC \
            $(INCLUDE_CFLAGS) -D__USE_ISOC99 \
            -fno-builtin -ffreestanding $(OPT_FLAGS) \
            -Wall -Wno-unused-variable -Wno-unused-function
endif

ifeq ($(NON_MATCHING),1)
  CFLAGS += -DNON_MATCHING
endif

# Symbol files for undefined references (will be converted to linker script format)
UNDEFINED_SYMS := undefined_syms_auto.$(VERSION).txt
UNDEFINED_FUNCS := undefined_funcs_auto.$(VERSION).txt
HARDWARE_REGS := hardware_regs.ld
SYMS_LD := $(BUILD_DIR)/syms.ld

# Linker flags
# --accept-unknown-input-arch allows linking binary blobs with different apparent arch
LDFLAGS      := -T $(LD_SCRIPT) -T $(SYMS_LD) -T $(HARDWARE_REGS) -Map $(BUILD_DIR)/rush2049.$(VERSION).map --no-check-sections --accept-unknown-input-arch

# Verbose output
ifeq ($(VERBOSE),0)
  V := @
endif

# ============================================================
# File lists
# ============================================================

# Assembly source files
ASM_FILES    := $(wildcard $(ASM_DIR)/*.s)
ASM_O_FILES  := $(patsubst $(ASM_DIR)/%.s,$(BUILD_DIR)/$(ASM_DIR)/%.o,$(ASM_FILES))

# C source files (recursively find all .c files in src/)
C_FILES      := $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)
C_O_FILES    := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(C_FILES))

# Binary data files
BIN_FILES    := $(wildcard $(ASSETS_DIR)/*.bin)
BIN_O_FILES  := $(patsubst $(ASSETS_DIR)/%.bin,$(BUILD_DIR)/$(ASSETS_DIR)/%.o,$(BIN_FILES))

# All object files (C files compiled but not linked yet - need matching first)
# For now, only use assembly files for the ROM build
O_FILES      := $(ASM_O_FILES) $(BIN_O_FILES)

# ============================================================
# Targets
# ============================================================

.PHONY: all clean extract test progress setup help verify cc

all: $(TARGET)
ifeq ($(COMPARE),1)
	@$(MAKE) --no-print-directory verify
endif

# Compile all C files (for testing decompiled code)
cc: $(C_O_FILES)
	@echo "All C files compiled successfully!"
	@echo "Compiled $(words $(C_O_FILES)) files"

# Help target
help:
	@echo "Rush 2049 N64 Decompilation"
	@echo ""
	@echo "Usage: make [target] [options]"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build ROM (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  extract   - Extract assets from baserom"
	@echo "  test      - Verify ROM hash"
	@echo "  progress  - Show decompilation progress"
	@echo "  setup     - Install required tools"
	@echo "  help      - Show this message"
	@echo ""
	@echo "Options:"
	@echo "  VERSION=us          - Target version (default: us)"
	@echo "  COMPILER=ido        - Use IDO 5.3 compiler (matching, default)"
	@echo "  COMPILER=gcc        - Use GCC compiler (non-matching)"
	@echo "  NON_MATCHING=1      - Build non-matching ROM"
	@echo "  COMPARE=0           - Skip hash verification"
	@echo "  VERBOSE=1           - Show all commands"
	@echo ""
	@echo "Quick start:"
	@echo "  1. Place ROM as baserom.us.z64"
	@echo "  2. make extract"
	@echo "  3. make"

# ============================================================
# Build rules
# ============================================================

# Create build directories
$(BUILD_DIR)/$(ASM_DIR) $(BUILD_DIR)/$(ASSETS_DIR):
	$(V)mkdir -p $@

# Create C source build directories
$(BUILD_DIR)/$(SRC_DIR)/%:
	$(V)mkdir -p $@

# Create symbols linker script from undefined_syms and undefined_funcs files
$(SYMS_LD): $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) | $(BUILD_DIR)
	@echo "GEN $@"
	$(V)cat $(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) > $@

$(BUILD_DIR):
	$(V)mkdir -p $@

# Assemble .s files
$(BUILD_DIR)/$(ASM_DIR)/%.o: $(ASM_DIR)/%.s | $(BUILD_DIR)/$(ASM_DIR)
	@echo "AS $<"
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Compile C files
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(V)$(CC) -c $(CFLAGS) -o $@ $<

# os_sync.c matches with debug-style codegen; disable optimization for this file.
ifeq ($(COMPILER),ido)
$(BUILD_DIR)/src/libultra/os_sync.o: CFLAGS := -G 0 $(MIPSISET) -g -non_shared \
            $(INCLUDE_CFLAGS) \
            -Wab,-r4300_mul -Xcpluscomm
endif

# Convert binary files to objects (using MIPS big-endian ELF format)
# Use elf32-big for binary files to avoid ABI conflicts
$(BUILD_DIR)/$(ASSETS_DIR)/%.o: $(ASSETS_DIR)/%.bin | $(BUILD_DIR)/$(ASSETS_DIR)
	@echo "BIN $<"
	$(V)$(OBJCOPY) -I binary -O elf32-big $< $@

# Link ELF
$(ELF): $(O_FILES) $(LD_SCRIPT) $(SYMS_LD)
	@echo "LD $@"
	$(V)$(LD) $(LDFLAGS) -o $@ $(O_FILES)

# Extract ROM from ELF
$(TARGET): $(ELF)
	@echo "OBJCOPY $@"
	$(V)$(OBJCOPY) -O binary $< $@
	@echo "Build complete: $@"

# Verify ROM hash
verify: $(TARGET)
	@echo "Verifying ROM hash..."
	@sha1sum -c $(VERSION).sha1 && echo "ROM matches!" || echo "ROM does NOT match"

test: verify

# ============================================================
# Extraction
# ============================================================

extract: $(BASEROM)
	@echo "Running splat..."
	$(V)$(PYTHON) -m splat split splat.$(VERSION).yaml

# ============================================================
# Progress tracking
# ============================================================

progress:
	@echo "=== Rush 2049 Decompilation Progress ==="
	@echo ""
	@total_asm=$$(find $(ASM_DIR) -name '*.s' | wc -l); \
	total_c=$$(find $(SRC_DIR) -name '*.c' 2>/dev/null | wc -l); \
	echo "Assembly files: $$total_asm"; \
	echo "C source files: $$total_c"; \
	if [ $$total_asm -gt 0 ]; then \
		pct=$$((total_c * 100 / total_asm)); \
		echo "Progress: $$pct%"; \
	fi
	@echo ""
	@echo "Functions by file:"
	@grep -c 'glabel func_' $(ASM_DIR)/*.s 2>/dev/null | sort -t: -k2 -nr | head -10

# ============================================================
# Cleanup
# ============================================================

clean:
	$(V)rm -rf build/
	@echo "Build directory cleaned"

# ============================================================
# Setup helpers
# ============================================================

setup:
	@echo "Tool setup checklist:"
	@echo ""
	@echo "[x] MIPS toolchain:"
	@echo "    sudo apt install binutils-mips-linux-gnu gcc-mips-linux-gnu"
	@echo ""
	@echo "[x] Python dependencies:"
	@echo "    pip install splat64 spimdisasm"
	@echo ""
ifeq ($(wildcard $(TOOLS_DIR)/ido-static-recomp/build/out/cc),)
	@echo "[ ] IDO compiler (for matching builds):"
	@echo "    Run: cd tools/ido-static-recomp && make setup && make VERSION=5.3"
else
	@echo "[x] IDO 5.3 compiler (ready for matching builds)"
	@echo "    Location: $(TOOLS_DIR)/ido-static-recomp/build/out/"
endif

# ============================================================
# Development helpers
# ============================================================

.PHONY: format lint disasm

# Format all source files
format:
	@echo "Formatting source files..."
	$(V)find $(SRC_DIR) -name '*.c' -o -name '*.h' 2>/dev/null | xargs -r clang-format -i
	$(V)find $(INCLUDE_DIR) -name '*.h' | xargs -r clang-format -i

# Lint check
lint:
	@echo "Running lint checks..."
	$(V)find $(SRC_DIR) -name '*.c' -o -name '*.h' 2>/dev/null | xargs -r clang-format --dry-run -Werror

# Disassemble a function from the built ELF
disasm:
ifndef FUNC
	@echo "Usage: make disasm FUNC=func_80001234"
else
	$(V)$(OBJDUMP) -d $(ELF) | grep -A 100 "<$(FUNC)>:" | head -120
endif

# ============================================================
# Context for decomp.me
# ============================================================

.PHONY: context
context:
	@echo "// Target: N64 MIPS VR4300"
	@echo "// Compiler: IDO 7.1 (assumed)"
	@echo "// Flags: -O2 -mips2"
	@echo ""
	@cat $(INCLUDE_DIR)/types.h
	@echo ""
	@cat $(INCLUDE_DIR)/macros.h
