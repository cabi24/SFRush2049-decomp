# asm-processor (vendored)

Wraps the IDO compiler for translation units that contain
`#pragma GLOBAL_ASM("...")` blocks: it extracts each block, compiles the C with
the real compiler, assembles the referenced `.s` file, and splices the assembled
machine code back into the object so a passthrough function is byte-identical to
pure assembly. This is the standard N64-decomp mechanism (constitution Tool
Requirements) and the basis of feature 004 (promotion splicing).

## Pin / provenance

Vendored 2026-07-09 from `reference/repos/sm64/tools/asm-processor/`
(sm64 repo commit `9921382a68bb0c865e5e45eb594d9c64db59b1af`, 2023-08-17), which
itself vendors simonlindholm/asm-processor. Pin identity:

- `asm_processor.py` sha256 `476c85718a9cbeb705710f1ba401040c039657940d21843c13b380c0a50430ee`

Do not edit these files; re-vendor to update.

## Usage (Makefile)

```
CC_ROM = python3 tools/asm-processor/build.py $(CC) -- $(AS) $(ASFLAGS) --
$(CC_ROM) -c $(CFLAGS) -o out.o src/rom/<name>.c
```

`build.py` parses its argv as `<asmproc_flags> <compiler...> -- <assembler...>
-- <compile_args... in.c>`. It runs on the builder (watchman) only — it drives
IDO, which cannot run on the Pi.
