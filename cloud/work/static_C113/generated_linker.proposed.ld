OUTPUT_ARCH(mips)
__scTaskReady = 0x80000BA4;
SECTIONS {
 .text 0x80000FE8 : SUBALIGN(4) {
  build/us/src/rom/lib_1050_sdk.o(.text);
 }
 .rodata : {
  /* ROM_OWNED_SECTION_MOVED build/us/src/rom/lib_1050_sdk.o(.rodata) */
 }
 .data 0x8000F400 : /* ROM_OWNED_SUBALIGN SUBALIGN(16) */ {
  /* ROM_OWNED_DATA_BEGIN build/us/assets/us/data.o(.data) */
  _binary_assets_us_data_bin_start = ABSOLUTE(.);
  _binary_build_us_assets_us_data_composed_bin_start = ABSOLUTE(.);
  build/us/assets/us/data.o(.data.owned0);
  rom_owned___scScheduleCore_START = ABSOLUTE(.);
  build/us/src/rom/lib_1050_sdk.o(.rodata);
  rom_owned___scScheduleCore_END = ABSOLUTE(.);
  ASSERT(rom_owned___scScheduleCore_START == 0x8002d4a4, "owned ROM slot address changed");
  ASSERT(rom_owned___scScheduleCore_END - rom_owned___scScheduleCore_START == 0x1c, "owned ROM slot size changed");
  ASSERT(rom_owned___scScheduleCore_START % 4 == 0, "owned table alignment changed");
  build/us/assets/us/data.o(.data.owned1);
  _binary_assets_us_data_bin_end = ABSOLUTE(.);
  _binary_assets_us_data_bin_size = ABSOLUTE(_binary_assets_us_data_bin_end - _binary_assets_us_data_bin_start);
  _binary_build_us_assets_us_data_composed_bin_end = ABSOLUTE(.);
  _binary_build_us_assets_us_data_composed_bin_size = ABSOLUTE(_binary_build_us_assets_us_data_composed_bin_end - _binary_build_us_assets_us_data_composed_bin_start);
  /* ROM_OWNED_DATA_END build/us/assets/us/data.o(.data) */
 }
 /DISCARD/ : { *(.bss) *(.reginfo) *(.options) *(.mdebug) *(.MIPS.abiflags) }
}
