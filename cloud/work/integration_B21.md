B21 proves a private, code-free preservation model for lib_8700's genuine original TU boundary. osDpWait remains unaccepted; this packet implements no production framework and edits no root source/layout/locks/metadata.

The authoritative SPLAT pair is `[0x8700, c, rom/lib_8700]` then `[0x8800, c, rom/lib_8800]`. Therefore the original text extent is 256 bytes at `[0x80007B00,0x80007C00)`. The already accepted osDpSetNextBuffer occupies its first 128 bytes. osDpWait starts at 0x80007B80 and its original `endlabel` follows exactly eight words / 32 bytes at 0x80007BA0. All remaining 96 bytes through 0x80007BFC are genuine zero words in both original assembly and retail ROM. boundary_record.json pins the full extent, zero-tail and original assembly hashes.

The canonical C14 body `void osDpWait(void) { __osSpSetStatus(0x400); }` is unchanged. The candidate full TU preserves the accepted preceding body exactly; focused normalized-body checks confirm identity. Actual full-TU output contains 160 text bytes. Fresh function-scoped canonical stack-sensitive verification of that actual full-TU object is strict0/raw0 against the current trusted 32-byte target. There is no dummy C, register-pressure context or stand-in function.

The private linker model surrounds the real TU .text input with start/end symbols, asserts the original absolute address and one of the two proven input sizes (256-byte assembly baseline or 160-byte canonical candidate), and advances the linker location to the original 256-byte boundary under zero FILL. Candidate map shows real input `[0x80007B00,0x80007BA0)`, a 0x60 fill, then the following strchr at unchanged 0x80007C00. Baseline emits all 256 original bytes and needs zero additional fill. The address ASSERT uses ABSOLUTE because ordinary section-relative comparison inside the output section is not equivalent at linker evaluation time. No object-local or function padding symbol is invented.

Actual isolated full-ROM proofs on `watchman2:~/agents/B/B21-private-rom`:

| Step | Real TU text | Linker fill | ROM gate |
|---|---:|---:|---|
| Original baseline, ordinary linker | 256 | 0 | Exact |
| Canonical candidate, ordinary linker | 160 | 0 | Fails SHA as expected |
| Canonical candidate, validated boundary model | 160 | 96 | Exact |
| Real SPLAT regeneration, replay same metadata/model | 160 | 96 | Exact |
| Assembly baseline restored, boundary model retained | 256 | 0 | Exact |
| Full source/linker rollback | 256 | 0 | Exact |

All successful gates retain SHA-1 `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`. Extraction used actual `make extract SPLAT_PYTHON=/home/cburnes/.splat-venv/bin/python` in the private local source snapshot. Every build used pinned IDO and at most two jobs. The builder is restored to baseline source and ordinary linker. The snapshot derives from B18's private ROM-proven tree, predating later root acceptances; current-root integration/gates remain mandatory. Initial private syntax and section-relative-ASSERT probes failed explicitly before the final tested model; no failed experiment is reported as acceptance.

The narrow sustainable production proposal is an explicit text-boundary/zero-fill record, preserving C13 ROM slots and B18 storage blocks. It should describe original TU input/VRAM extent, original endlabel-derived logical endpoint, exact retail zero-tail hash, and body/padding accounting. Linker generation should validate these authorities, emit this boundary block after each regenerated ordinary SPLAT script, and reject ambiguous ownership or unproven nonzero bytes. Normal promotion and rollback should include the record/generated linker in their atomic package. A baseline with all original bytes naturally needs no fill, so it uses the same record without an activation special case. This proposal deliberately awaits root review instead of introducing a general framework now.

Coverage requires a separate logical body field. Current layout.derive assigns osDpWait size128 because the slot extends to the next segment; coverage() would therefore credit128 on promotion. That would overstate C instruction/body coverage by96. Correct acceptance increment is **one static function and 32 C body bytes**, with **96 preserved zero bytes separately reported and unclaimed**. Keep the existing static-range denominator for historical comparison unless a separately reviewed global denominator audit excludes all known non-code padding. The physical linked extent may be reported as128 (32 body +96 linker fill), but it must not become128 matched C instruction bytes. No numerical coverage is added by this packet.

Review artifacts: padding_model.py is the bounded private demonstrator; boundary_record.json and linker_block.txt show the exact metadata and output; strict_verification.json, full_rom_proof.json and map_proof.json pin hashes/counts/addresses. Four focused checks pass for authoritative zero-tail validation, nonzero-tail refusal, regenerated/idempotent linker replay with duplicate-input refusal, and unchanged actual bodies. Objects, ROMs, archives and raw byte streams remain ignored/private; deliverables contain source and hash/count evidence only.
