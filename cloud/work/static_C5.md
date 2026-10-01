# Static packet C5 — 2026-10-01

Five ready strict/stack-sensitive and raw-word object matches cover **596 potential static slot bytes**. A sixth exact object is blocked by the existing shared-TU O2 pin. Five other leads remain unmatched. This packet does not claim cartridge coverage before coordinator lock, shared-context integration, and full-ROM gates.

| Function | Segment | Slot bytes | Flags level | Strict/raw | Status |
|---|---|---:|---|---|---|
| __osSiRawStartDma | 0xf0b0 | 176 | O2 | 0/0 | Ready |
| __osSpDma | 0xe280 | 144 | O2 | 0/0 | Ready |
| __osSumcalc | 0xf700 | 116 | O2 | 0/0 | Ready |
| osPiReadIo | 0xe390 | 80 | O2 | 0/0 | Ready |
| osPiWriteWord | 0xe3f0 | 80 | O2 | 0/0 | Ready |
| dll_remove | 0xcc50 | 64 | O1 | 0/0 | Blocked by shared O2 pin |
| guOrthoF | 0x9660 | 340 | O2 | 370/15 | Unmatched |
| guPerspectiveF | 0x9820 | 560 | O2 | 370/65 | Unmatched |
| inflate_loop | 0x5610 | 92 | O2 | 160/10 | Unmatched |
| osMotorStop | 0xab20 | 96 | O2 | 500/9 | Unmatched |
| idle_thread_entry | 0x2cf0 | 148 | O2 | 100/9 | Unmatched |

Every ready file starts exactly `/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`. dll_remove starts with O1 in the corresponding line. Exact source SHA-256, target SHA-256, text-word lengths and explicit integration_status are recorded in `static_C5/verification.json`. Original selection and current authoritative layout extents are in `targets.json` and `targets_extra.json`; all selections were passthrough when chosen. Exported objects stay private at `/tmp/NAME.target.o` on the Pi and `~/agents/C/scratch/static-C5/NAME.target.o` on Rocky. The checksum's 116-byte function has a 128-byte aligned isolated object, and both target and candidate share that padding; slot coverage uses116.

Sources were re-derived from the local ultralib corpus @ e24c8367 and target assembly, with current external symbol names retained. __osSiRawStartDma uses the VERSION_J+ inline SI busy check and canonical cache/MMIO sequence (`src/io/sirawdma.c`). __osSpDma is canonical __osSpRawStartDma (`src/io/sprawdma.c`), replacing address-valued register macros with explicit volatile u32 lvalues. __osSumcalc uses the VERSION_J+ byte-sum helper (`src/io/contpfs.c`), summing in u32 and masking only at return. Pi-labelled osPiReadIo/osPiWriteWord actually implement SDK SI raw read/write behavior (`src/io/sirawread.c`, `sirawwrite.c`), performing K1 uncached address conversion and calling current symbol __osPiDeviceBusy. No misleading-symbol rename was attempted. dll_remove is canonical __osDequeueThread from `src/os/thread.c`; debug-only code is excluded. Its O2 compile remains strict1045/raw16, so the O1 zero is deliberately blocked by cc50's O2 pin. Minimal declarations and prototype audit are in `rom_tu_declarations.txt`.

The initial eight-target sweep found only two promotable zeros and the blocked dll_remove; three bounded additional canonical SDK probes all matched. Historical permuter sources were treated as leads, not accepted automatically. dll_remove's selected permuter lead had a pointer use before initialization; osSpDma's lead returned size-1 on one branch instead of0. Re-derived bodies remove those semantic errors. osMotorStop's current target passes its argument through to __osGetId, while the selected lead incorrectly declared a zero-argument call; the unmatched delivered lead corrects this.

GU F targets initially carried stale assemble_error/raw-word fallback for floating ABI aliases ($ft0 and $fa1). Coordinator supported refresh using the existing alias normalization made both reloc-aware with a passing assembly gate. This packet re-exported their new object hashes before the final evidence, without masks or scoring changes. Both still differ in genuine floating instruction scheduling: multiply/load ordering and a missing inter-multiply NOP in the emitted loop. The canonical Perspective constant additionally produces a local .rodata relocation; no raw-address stand-in was used. O1 and volatile-matrix experiments were farther away.

A bounded physical-line sweep scored740 token-preserving variants:100 dll_remove,160 each GU F,160 inflate_loop,100 osMotorStop and60 idle_thread_entry. It found no additional strict zero; `sweep_summary.json` records counts and best verdicts. Other bounded O1, volatile, loop spelling and explicit-return variants remain private in C scratch. inflate_loop differs in speculative load/delay-slot scheduling; a volatile final local reproduces all but two opcode words but still has strict400 (branch-likely scheduling), so it is not a match. osMotorStop and idle_thread_entry require redundant branch/epilogue structure absent from the O2 output. The delivered unmatched sources preserve behavior and do not force machine instructions with stand-ins.

Independent object verifier, copied along with six matching sources/manifest and target objects to D's private scratch:

```sh
python3 STATIC_DIR/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D_VERIFIED.json
```

Verifier checks six exact object matches, including explicitly blocked dll_remove, and excludes unmatched sources. It validates source/target hashes and exact flags, serially recompiles, enables stack-sensitive scoring and compares raw text words. Final C worker end-to-end run exited0 with all six strict/raw0. Passing it does not authorize dll_remove promotion under mixed flags.

Recommended coordinator commands for each of the five ready functions:

```sh
python3 -m tools.conveyor.pipeline.lock add cloud/work/static_C5/NAME.c:NAME --flags "-g0 -O2 -mips2 -G 0 -non_shared"
python3 -m tools.conveyor.pipeline.promote run SEGMENT:NAME --from cloud/work/static_C5/NAME.c --via-builder
```

Use the table's segment values, reconcile the minimal declarations first and obey clean-tree/ROM/lock gates. Parent owns any required segment conversion and pins; worker did not lock, promote, modify src/asm/layout/state, commit or push. Compilation was serial and no C5 searches remain running.
