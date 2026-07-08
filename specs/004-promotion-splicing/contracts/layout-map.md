# Contract: layout map + ROM-aligned TU generation

Pi-local, `tools/conveyor/pipeline/layout.py`.

## Derivation

`layout derive` → `build/layout.us.json`, deterministic:

```json
{"segments": [{
   "yaml_name": "0x8800", "rom_tu": "rom/lib_8800",
   "vram_start": "0x80007C00", "size": 1234, "converted": false,
   "flagset": "-g0 -O2 -mips2 -G 0 -non_shared" | null,
   "functions": [{"name": "strlen", "vaddr": "0x80007C40", "size": 40,
                  "state": "passthrough" | "promoted"}],
   "refusal": null | "gap@0x...: 8 bytes not in any region" | "unnamed@0x..."
}]}
```

- Inputs: splat.us.yaml subsegment boundaries; 003 `index_asm_regions()`;
  `symbol_addrs.us.txt` canonical names; flag_registry for flagset.
- A segment derives cleanly only if regions tile its full range contiguously
  and every function has a canonical name; otherwise `refusal` is set and the
  segment is unconvertible (FR-002). Dynamic population range: one synthetic
  entry, `refusal: "dynamic-population (blob rebuild feature)"` (FR-011).

## TU generation / conversion

`layout convert <segment>`:

1. Refuse if `refusal` set, or segment already converted, or working tree dirty
   under the affected paths.
2. Edit `splat.us.yaml`: `[0xXXXX, asm]` → `[0xXXXX, c, rom/<name>]`.
3. Run splat re-split (`make extract`) → per-function asm lands in
   `asm/us/nonmatchings/rom/<name>/`, linker script regenerated.
4. Generate `src/rom/<name>.c`: file header (generated-by, segment, map hash) +
   per function, in layout order:
   `#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/<name>/<func>.s")`.
5. Print the build-and-verify instruction (the gate itself runs via `make`,
   on the builder — conversion is file-state only and reversible by git).

`layout convert --revert <segment>` restores the yaml + removes the TU (git
checkout for tracked, delete for generated).

Determinism: regenerating an unconverted-state artifact or an all-passthrough
TU twice yields identical bytes (map hash embedded in the TU header ties the
TU to the map version).

## Coverage

`layout coverage`: per segment and total — functions promoted/passthrough,
bytes promoted / static-code bytes; parses live TUs (a slot is `promoted` iff
its pragma was replaced), cross-checked against the map. Exposed to
`make progress` and the conveyor report (FR-009).
