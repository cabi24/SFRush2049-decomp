"""The boot segment past the static code, labelled in place (ROM 0x283D0-0x2F4E0).

    python3 -m tools.conveyor.pipeline.boot_data report

splat ends the static code at ROM 0x283D0 (RAM 0x800277D0) because the function
scanner (pipeline.ovl_targets) stops on hand-written code it cannot parse; the
rest of the boot segment, up to the entrypoint's BSS clear at 0x8002E8E0, is the
first 28,944 B of the ROM `data` bin. It holds three kinds of bytes:

  cpu       ten hand-written R4300 routines: nine libgsm short-term/RPE/long-term
            synthesis kernels called by lib_26f60's decoder, and a D-cache
            create-dirty-exclusive loop called from six static functions;
  rsp_text  RSP microcode text (rspboot, a microcode no task names, F3DEX NoN
            fifo 2.08, aspMain), all linked .text before C .data;
  rsp_data  the F3DEX and aspMain data blocks the two tasks point at;
  data      the segment's C .data/.rodata (from __osScCurAudioTask at 0x8002AFA0).

Nothing here moves or rebuilds those bytes: assets/us/data.bin still carries
them (110 research receipts pin its sha256), and the ROM build is unchanged.
tests/conveyor/test_boot_data.py re-proves every label against the ROM. The cpu
routines count in the static coverage denominator (pipeline.blob_rom) as
functions not yet built from source; splitting them into their own segment is
the job of whoever first matches one.
"""
import argparse
import sys

ROM_START = 0x283D0
VRAM_START = 0x800277D0
VRAM_END = 0x8002E8E0          # entrypoint clears BSS from here to the game image

# (vaddr, size, name, role). Sizes tile their cpu region; an inline table rides
# with the routine that reads it (0x800278BC, 0x80027978).
CPU_ROUTINES = (
    (0x800277D0, 0xF4, "func_800277D0", "GSM RPE decoding (APCM inverse quantization, grid positioning)"),
    (0x800278C4, 0xBC, "func_800278C4", "GSM long-term synthesis into the decoder state"),
    (0x80027980, 0x1C4, "func_80027980", "GSM short-term synthesis filter loop (bal from 0x80027B44)"),
    (0x80027B44, 0x154, "func_80027B44", "GSM short-term synthesis driver"),
    (0x80027C98, 0xA4, "func_80027C98", "GSM LAR coefficients pass (bal; calls a PC-relative coefficient fn)"),
    (0x80027D3C, 0x18, "func_80027D3C", "GSM LAR interpolation, first variant (tail: 0x80027D7C)"),
    (0x80027D54, 0x10, "func_80027D54", "GSM LAR interpolation, second variant (tail: 0x80027D7C)"),
    (0x80027D64, 0x18, "func_80027D64", "GSM LAR interpolation, third variant (tail: 0x80027D7C)"),
    (0x80027D7C, 0x54, "func_80027D7C", "GSM LARp_to_rp (constants 11059/20070/26112)"),
    (0x8002C5E0, 0x1C, "func_8002C5E0", "D-cache create-dirty-exclusive loop over [a0, a1)"),
)

# (start, end, kind, label): tiles VRAM_START..VRAM_END.
REGIONS = (
    (0x800277D0, 0x80027DD0, "cpu", "GSM decoder kernels (9 routines)"),
    (0x80027DD0, 0x80027EA0, "rsp_text", "rspboot (both tasks' ucode_boot; size = end - start)"),
    (0x80027EA0, 0x80028CC0, "rsp_text", "microcode no task references (probably a second audio "
                                         "microcode: a copy of aspMain's resample table ends the "
                                         "data before F3DEX's)"),
    (0x80028CC0, 0x8002A050, "rsp_text", "F3DEX NoN fifo 2.08 graphics microcode (gfx task ucode)"),
    (0x8002A050, 0x8002AFA0, "rsp_text", "aspMain audio microcode (audio task ucode)"),
    (0x8002AFA0, 0x8002C5E0, "data", "static .data/.rodata"),
    (0x8002C5E0, 0x8002C5FC, "cpu", "func_8002C5E0 (hand-written, placed among data)"),
    (0x8002C5FC, 0x8002DDF0, "data", "static .data/.rodata (ends with a copy of aspMain's 512 B resample table)"),
    (0x8002DDF0, 0x8002E210, "rsp_data", "F3DEX data (gfx task ucode_data; version string)"),
    (0x8002E210, 0x8002E440, "rsp_data", "aspMain data (audio task ucode_data)"),
    (0x8002E440, 0x8002E8E0, "data", "static .data/.rodata (build-info record, 07-17-00 MERRILL)"),
)


def rom_offset(vaddr):
    return vaddr - VRAM_START + ROM_START


def coverage():
    """The cpu routines as static functions not yet built from source."""
    return {"functions": len(CPU_ROUTINES), "bytes": sum(size for _, size, _, _ in CPU_ROUTINES)}


def report():
    for start, end, kind, label in REGIONS:
        print(f"0x{start:08X}-0x{end:08X}  ROM 0x{rom_offset(start):05X}  {end - start:6d} B  "
              f"{kind:8s}  {label}")
    cov = coverage()
    print(f"cpu routines: {cov['functions']} ({cov['bytes']} B), counted as static functions")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=["report"])
    parser.parse_args(argv)
    report()
    return 0


if __name__ == "__main__":
    sys.exit(main())
