# Final C15 freeze: four eligible functions,836 logical bytes

Parent-supported B20 refresh is complete. `static_C15/ready_four.json` supersedes the initial two-function acceptance list. All four unchanged standalone sources and all four current-root-header sources pass strict stack/raw0, using B18's corrected u64 FP-pair OSThread header. Snapshot hashes in current_header_snapshot.json. Real original64/144/268/360-byte linked prefixes remain exact with zero masks/unresolved/unverified/errors; extra isolated-object tail words are zero alignment only and not coverage. Updated targets.json retains pre-refresh provenance for all affected targets. Alias current object hashes are003f0366f146328fdd098524974f78db29815c628a0ca0892e4ebc4a63c5b53e (osGetTime),6a0055529638e63974886853916a7c7087106788dd8d52248708ecf9da4412c1 (osViModeTableGet).

Independent current four-function invocation: `python3 cloud/work/static_C15/verify_four.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --targets ~/agents/D/scratch/static-C15 --output ~/agents/D/scratch/static-C15/verify`. Copy four `NAME.C15.target.o` files from C private scratch. Exact flags/source/target/object hashes in ready_four.json. No source files changed during alias refresh.

Prepared baseline for8440 is frozen prepared_lib_8440.c + local_task_context.h; shared IO_READ remains unchanged. Whole original lib8440 text before/after local preparation byte-identical, actual accepted neighbor osViModeNtscLpn1 unchanged. A separate private control also proved actual osSpTaskYielded/lib8330 text unchanged by that context; control then restored. Full private ROM with local preparation passes original SHA1 (`local_context_verification.json`, C15-local-task-context-ROM.log). Parent must use current source-built game blob for its integration gate. Minimal additional alias context/prototypes in minimal_alias_context.txt; full body/source hashes remain immutable.

DLL-init new SDK-defined-storage lever remains separate and unaccepted: defining original SDK clock globals instead of extern declarations makes its canonical body linked-exact, but genuine ownership/placement is still required. Private controls are C16 follow-up material, not part of these four ready bodies.

---

## Initial subset and retained provenance

Frozen ordinary ready subset: two functions,424 logical bytes. `static_C15/ready_two.json` pins sources, exact flags, current target hashes and candidate hashes. `verify_two.py` recompiles both self-contained TUs and requires hash identity plus strict stack/raw0. Current-root-header versions independently pass the same checks. Private objects/targets remain in Rocky `~/agents/C/scratch/static-C15/`; original retail_words.json remains private/ignored.

| Target | Segment | Logical bytes | Exact flags | Evidence |
| --- | --- | ---: | --- | --- |
| dll_remove | 0xcc50 | 64 | -g0 -O1 -mips2 -G 0 -non_shared | strict/raw0; all64 linked retail bytes0 |
| osViModeNtscLan1 | 0x8440 | 360 | -g0 -O2 -mips2 -G 0 -non_shared | strict/raw0; all360 linked retail bytes0 |

The first is the unchanged canonical SDK `__osDequeueThread` body from C5, legitimately unblocked by the current O1 cc50 pin. Parent supported refresh converted its old raw-word/no-asm target into reloc-aware/no fallback; old/current provenance is retained in targets.json. The second is SDK `osSpTaskLoad` from ultralib io/sptask.c, using existing named functions/types. Correct SDK `IO_READ` includes PHYS_TO_K1, and F3DEX-family yield size is0xC00. No runtime storage definitions or extra rodata are emitted. Standalone object includes normal final alignment; coverage counts360 logical bytes, not368 aligned bytes.

Minimal current context/signature audit is in `minimal_ready_two_context.txt`. All new declarations agree with existing accepted definitions. The actual existing shared IO_READ macro performs literal dereference; preserve that macro globally and use the genuine SDK PHYS_TO_K1 definition only locally in lib_8440 before this slot. Existing accepted osViModeNtscLpn1 (SDK osSpTaskStartGo) there never references IO_READ. The delivered canonical source/body remains frozen. Root may instead rename that macro locally after independent reproof/relock.

Independent invocation:
`python3 cloud/work/static_C15/verify_two.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --targets ~/agents/D/scratch/static-C15 --output ~/agents/D/scratch/static-C15/verify`
Copy `dll_remove.C15.target.o` and `osViModeNtscLan1.C15.target.o` from C private scratch into D. Recommended normal lock sources `cloud/work/static_C15/dll_remove.c:dll_remove` and `cloud/work/static_C15/osViModeNtscLan1.c:osViModeNtscLan1`; standard promotions `0xcc50:dll_remove` and `0x8440:osViModeNtscLan1`, preserving each exact flag pin and full-ROM gate.

Two canonical metadata-only blockers were independently linked to original retail symbols with zero masks, unresolved/unverified references or errors, but remain unaccepted until supported targets refresh:

| Target | Flags | Current strict/raw | Whole original linked result | Proven alias |
| --- | --- | --- | --- | --- |
| osGetTime | O1,mips2 | 10/1 | all144 bytes0 | gViTimeAccumLo=OSTime gViTimeAccumHi+4 |
| osViModeTableGet | O2,mips2 | 140/14 | all268 bytes0 | seven named pointers=OSTask gViModeTempBuffer fields |

Canonical bodies are SDK os/gettime.c `osGetTime` and io/sptask.c `_VirtualToPhysicalTask`, unchanged except real symbol mapping. Current source declarations directly use the real OSTime scalar and OSTask structure. Aliases are established by undefined_syms_auto.us.txt203–204 (time) and174–181 (task), plus SDK typedef/source and current include/types.h physical layout. Task offsets: ucode10,ucode_data18,dram_stack20,output_buff28,output_buff_size2c,data_ptr30,yield_data_ptr38. Parent assigned B20 a guarded assembler normalization, with exact original-word gate retained and no scorer change. All evidence paths were shared with B; these two canonical source files will not change during the refresh.

`dll_init` remains genuinely unmatched under current O1,mips2 (520/raw36;35 linked retail words differ). Its tail is canonical but current compiler emits the initial64-bit zero store high-first with one extra address load, unlike retail low-first/shared address. One directed mips1 control also failed (1185/raw29); mips3 is unsupported for32-bit IDO. No new flags are pinned and no stand-ins or formatting sweeps were used. Assembly-only osSetGlobalIntMask and truncated92-byte __osPfsRWInode were excluded. Previously exhausted MotorStop/Ai tails were not rerun without a new hypothesis. Shared sources, headers, layout, locks/state remain untouched by this worker.
