# Generated from rom_owned_data.json storage_blocks; do not hand edit.
ifeq ($(COMPILER),ido)
LDFLAGS := -T rom_owned_storage.ld $(LDFLAGS)
$(ELF): rom_owned_storage.ld rom_owned_data.json
O_FILES := $(filter-out $(BUILD_DIR)/asm/us/7630.o,$(O_FILES))
$(BUILD_DIR)/src/rom/lib_7630.o: CFLAGS := $(filter-out -O%,$(CFLAGS)) -O2
$(BUILD_DIR)/src/rom/lib_7630.o: src/rom/lib_7630.c rom_owned_data.json include/PR/os_thread.h
	@mkdir -p $(dir $@)
	@echo "CC $< (complete owned storage)"
	$(V)$(CC) -c -g0 $(CFLAGS) -o $@ $<
	$(V)$(OBJCOPY) --rename-section .bss=.bss.vi_manager $@
endif
