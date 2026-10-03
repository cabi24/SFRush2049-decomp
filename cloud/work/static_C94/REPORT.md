# C94 complete Huffman decoder, nonmatch

Reconstructed the complete1272-byte original decoder historically called inflate_free_window. The actual function decodes literal/length and distance tables; it reads little-endian16-bit input words, performs both real invalid-code99 checks, preserves local bit/count state on successful block end, and uses the target's signed negative displacement rule: memcpy for offsets below-8, forward byte-by-byte overlap expansion otherwise. All original branches, inputs and stores are represented.

The authoritative assembly target was privately built through production assemble_region and passed the complete original-word gate. Best source inflate_free_window.c compiles with the existing shared O2 recipe, exit0/empty stderr, strict7032 including stack differences. Candidate305 true instructions versus original318, with88-byte rather than96-byte frame; this remains a clear nonmatch, no credit. Workbench reports135 edit distance and145 aligned residuals.

Grounded input-pointer reload qualification and canonical K&R definition controls do not improve strict score. O1 and g1 controls are diagnostic only and materially worse; no shared compiler pin is changed. Pointer qualification remains speculative and is not adopted. The baseline uses normal existing global declarations and complete grounded Huft8-byte table carrier. Compiler's generic linked-game scorer refuses static-section lookup, so only actual assembled-target strict/raw evidence is claimed; no linked or ROM proof is claimed.

No shared sources, headers, tools, storage/ownership registry, target DB, locks, layout or Git are modified. Frozen complete source SHA is recorded in manifest.json.
