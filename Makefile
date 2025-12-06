# ============================================================
# San Francisco Rush 2049 (N64) Decompilation Makefile
# ============================================================
# Based on MK64/SM64 patterns
# See reference/repos/mk64/Makefile for full example

# Version selection (only US supported currently)
VERSION ?= us
$(if $(filter $(VERSION),us),,$(error Invalid VERSION: $(VERSION). Valid: us))

# Build configuration
NON_MATCHING ?= 0
COMPARE      ?= 1
VERBOSE      ?= 0

# Directories
BUILD_DIR    := build/$(VERSION)
SRC_DIR      := src
ASM_DIR      := asm
INCLUDE_DIR  := include
TOOLS_DIR    := tools
COURSES_DIR  := courses

# ROM files
BASEROM      := baserom.$(VERSION).z64
TARGET       := $(BUILD_DIR)/rush2049.$(VERSION).z64
ELF          := $(BUILD_DIR)/rush2049.$(VERSION).elf

# Tools (to be set up)
CROSS        ?= mips-linux-gnu-
AS           := $(CROSS)as
CC           := $(CROSS)gcc
LD           := $(CROSS)ld
OBJCOPY      := $(CROSS)objcopy
OBJDUMP      := $(CROSS)objdump

PYTHON       := python3

# Flags
ASFLAGS      := -march=vr4300 -mabi=32
CFLAGS       := -G0 -mno-shared -march=vr4300 -mfix4300 -mabi=32 \
                -fno-stack-protector -fno-common -fno-zero-initialized-in-bss \
                -fno-PIC -mno-abicalls -fno-strict-aliasing -fno-inline-functions \
                -ffreestanding -fwrapv -Wall -Wextra

# Include paths
CFLAGS       += -I$(INCLUDE_DIR) -I.

# Non-matching mode uses GCC optimizations
ifeq ($(NON_MATCHING),1)
CFLAGS       += -DNON_MATCHING -O2
else
# Matching mode (IDO compiler - to be configured)
CFLAGS       += -O2
endif

# Verbose output
ifeq ($(VERBOSE),0)
V := @
else
V :=
endif

# ============================================================
# Targets
# ============================================================

.PHONY: all clean extract test progress setup help

all: $(TARGET)
	@echo "Build complete: $(TARGET)"

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
	@echo "  NON_MATCHING=1      - Build non-matching ROM"
	@echo "  VERBOSE=1           - Show all commands"
	@echo ""
	@echo "Quick start:"
	@echo "  1. cp /path/to/rom baserom.us.z64"
	@echo "  2. make extract"
	@echo "  3. make NON_MATCHING=1"

# Check for baserom
$(BASEROM):
	@echo "Error: $(BASEROM) not found"
	@echo "Please place your Rush 2049 (US) ROM as $(BASEROM)"
	@echo "Expected SHA-1: f79223f8060a530d0dc8683a923c3c60615aa0a0"
	@exit 1

# Create build directory
$(BUILD_DIR):
	$(V)mkdir -p $(BUILD_DIR)

# Extract assets from ROM (requires splat setup)
extract: $(BASEROM)
	@echo "Asset extraction not yet configured"
	@echo "TODO: Set up splat with config.yaml"

# Placeholder ROM target (actual build rules TBD)
$(TARGET): $(BASEROM) | $(BUILD_DIR)
	@echo "Build system not yet complete"
	@echo "Copying baserom as placeholder..."
	$(V)cp $(BASEROM) $(TARGET)

# Verify ROM hash
test: $(TARGET)
	@echo "Verifying ROM hash..."
	$(V)sha1sum -c $(VERSION).sha1

# Show progress (placeholder)
progress:
	@echo "Progress tracking not yet implemented"
	@echo "TODO: Create progress.py script"
	@echo ""
	@echo "Current status: 0% (infrastructure setup)"

# Clean build artifacts
clean:
	$(V)rm -rf build/
	@echo "Build directory cleaned"

# Tool setup helper
setup:
	@echo "Tool setup checklist:"
	@echo ""
	@echo "[ ] MIPS toolchain:"
	@echo "    sudo apt install binutils-mips-linux-gnu gcc-mips-linux-gnu"
	@echo ""
	@echo "[ ] Python dependencies:"
	@echo "    pip install splat64"
	@echo ""
	@echo "[ ] IDO compiler (for matching builds):"
	@echo "    Copy from reference/repos/sm64/tools/ido-static-recomp/"
	@echo ""
	@echo "[ ] asm-processor:"
	@echo "    Copy from reference/repos/sm64/tools/asm-processor/"

# ============================================================
# Development helpers
# ============================================================

.PHONY: format lint

# Format all source files
format:
	@echo "Formatting source files..."
	$(V)find $(SRC_DIR) -name '*.c' -o -name '*.h' | xargs clang-format -i
	$(V)find $(INCLUDE_DIR) -name '*.h' | xargs clang-format -i

# Lint check
lint:
	@echo "Running lint checks..."
	$(V)find $(SRC_DIR) -name '*.c' -o -name '*.h' | xargs clang-format --dry-run -Werror

# ============================================================
# Diff helpers (when diff.py is set up)
# ============================================================

# Usage: make diff FUNC=function_name
.PHONY: diff
diff:
ifndef FUNC
	@echo "Usage: make diff FUNC=function_name"
else
	$(V)$(PYTHON) diff.py $(FUNC) -m
endif

# Watch mode
.PHONY: diff-watch
diff-watch:
ifndef FUNC
	@echo "Usage: make diff-watch FUNC=function_name"
else
	$(V)$(PYTHON) diff.py $(FUNC) -m -w
endif
