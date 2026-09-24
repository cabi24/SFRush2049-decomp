# 008 Game-Code Image Rebuild — close-out scorecard (2026-09-24)

Branch `008-blob-image-rebuild`. Evidence: `quickstart.md` (§1-§5 actuals),
`build/blob_layout.json`, `blob_matched.lock.json`, `make progress`.

| SC | Verdict | Evidence |
|----|---------|----------|
| SC-001 all-passthrough build byte-identical, reproducibly | **MET** | 647,072 bytes, sha `bf7da3fa6283…`, 0.3 s, on the Pi with no IDO; map derivation byte-identical across runs |
| SC-002 ≥50 of 76 matched functions spliced, each with provenance | **MET — 56** | `blob_matched.lock.json` (56 entries, each with source sha, flagset, toolkit, `verified: image_gate`) |
| SC-003 corruption drill fails with the offending offset named | **MET** | one word → `0xDEADBEEF` gave offset 62576, vram `0x80095EC0`, region `blob_800959dc`, owner `func_80095EC0`, exit 1 |
| SC-004 image and ROM coverage reported separately with the caveat | **MET** | `make progress` prints both under labelled headings; the image line carries the cartridge caveat |
| SC-005 revert restores the prior image hash | **MET** | `object_process_thunk` and `Input_SetPadEnabledFlag` reverted and re-spliced; hash returned both times |
| SC-006 suite green; ROM hash, ROM coverage and `matched.lock.json` untouched | **MET** | 256 tests pass; cartridge coverage still 19/230; ROM lock unchanged (pre-commit hook green on every commit) |

All six met. That is not the interesting part of this feature.

## What it actually cost, and what that says

Six defects stood between "the image links" and "56 functions spliced", and
**every one was caught by the gate, not by reading code**:

1. `ld` **segfaulted** on IDO's `.reginfo`/`.options`/`.mdebug` — no message
   at all. Stripping them converted a crash into a real diagnostic.
2. `undefined reference to 'pad_config'` — data globals live inside opaque
   runs, so nothing declares them.
3. A `BuildError` escaping the splice loop aborted the run instead of
   refusing one body.
4. **IDO pads `.text` to 16 bytes.** A 72-byte function occupies 80 in its
   object and the padding overwrote its neighbour; truncating the section
   loses its relocations. This forced the design change: spliced C
   contributes *bytes*, linked alone at its image address, cut to the extent.
5. Bodies spliced at `-O2` that had matched at `-O1`. The sweep had already
   recorded which flagset scored 0; the splice was ignoring it.
6. A function linked alone cannot see its siblings — 37 refusals of
   `undefined reference to 'func_…'`.

Run-by-run: **6 → 34 → 56**. The value of the byte-identity gate is that not
one of those six produced an image that passed anyway.

## The 20 refusals

- **12 unresolved callees** (`entity_flags_apply`, `UpdateActiveObjects`,
  `AdjustSpeed`, `Input_ApplyPadConfig`, `func_803914b4`): `extent_conflict`
  suffix rows and the out-of-image `0x8038xxxx` class. Neither has a real
  address to provide — 007 residuals, not this feature's business.
- **8 image differences**, including `func_80095EC0` and `func_800C8738`, the
  first two functions the project ever matched. Their score of 0 was measured
  against a **raw-word target before the reloc-aware rebuild**, so matching a
  relocation-blind comparison does not imply matching in place. The gate is
  right to refuse them; they are re-scoring candidates, not regressions.

## Deviation from the contract

The contract specified 004's `#pragma GLOBAL_ASM` TUs. Implementation used
per-entry sections plus a generated linker script instead: simpler for a
standalone image, no asm-processor, and it let the whole gate be proven on
the Pi before the builder was involved. Recorded in `quickstart.md` under
"Contract amendment"; contract §7-§9 should be read with that substitution.

## Honest limits

- Image coverage is **0.64%** (56/912 functions, 4,168/647,072 bytes). The
  mechanism is proven; the volume is not there yet and depends entirely on
  how many more functions match.
- **None of this reaches the cartridge.** Stage 2 — reproducing the DEFLATE
  stream — is unbuilt, and until it exists the ROM still embeds the original
  compressed bytes and ROM coverage stays 19/230.
- One commit (`913fefc`) was made with a red test that asserted the
  pre-redesign behaviour; fixed in `113bf07` rather than amended.

## Next

- Re-score the 8 refused bodies against reloc-aware targets; they were
  matched under the old comparison.
- 007 residuals would close most of the 12 unresolved callees.
- Stage 2: time-box encoder archaeology (historical zlib builds first) and
  decide on the two-tier verification fallback with data.
