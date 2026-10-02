# C20 — fresh decompression supply, no strict matches

Selected current genuine passthroughs from converted5610: inflate_entry_alt (128 instruction bytes/132 tiled), inflate_read_bits (320), inflate_stored (544) and lzss_decode (596). Existing accepted inflate_flush_window and huft_alloc pin canonical -g0 -O2 -mips2 -G 0 -non_shared. No shared source, header, target metadata, lock, layout or state writes. targets.json records privately assembled current-source targets with successful existing assembly gates; these were not published to the DB. No accepted zero is claimed.

The fresh ordinary source bodies reproduce the current assembly operations with real named symbols and current SDK prototypes. inflate_entry_alt initializes input/output pointers, allocates and frees the real12000-byte window around inflate_loop, and returns actual output advancement. inflate_read_bits waits for the outstanding DMA completion, swaps the real4096-byte buffers, advances source/end pointers, invalidates cache, queues the actual seven-argument PI DMA API, and reads the next little-endian16-bit word. inflate_stored discards partial-byte bits, reads little-endian LEN/NLEN, checks their complements, copies bytes through the real input refill path, and saves the remaining bit state. Scalars/pointer declarations were narrowed through directed controls; accepted neighbors are untouched.

| Honest delivered lead | O2 strict / raw-word differences |
| --- | --- |
| inflate_entry_alt | 485 / 25 |
| inflate_read_bits | 1695 / 68 |
| inflate_stored | 1885 / 108 |

Recorded controls: eight O2/O1 baselines; twelve pointer declaration/local-return controls; eight alternate-entry parameter controls; six numeric pointer-representation controls; eight input-reader loop/order/local controls; four stored-block scalar/declaration controls. These are46 serial compiler controls across the packet, far below40 probes per target. O1 is diagnostic only and does not override the authoritative O2 pin. The numeric address declarations and parameter register spellings did not produce an accepted match. No line sweep, scorer masks, altered assembler driver or invented initialized local was used. Current source artifacts retain the best ordinary bodies; private controls and objects remain in Rocky ~/agents/C/scratch/static-C20. Results JSON files preserve every failed verdict.

## Genuine whole-program ABI blocker

lzss_decode cannot be represented truthfully by the initial normal void inflate_io_wait prototype. Real asm inflate_io_wait returns the next input buffer in t1 and clobbers s0–s4 without restoring them; lzss_decode spills/restores its live s0 explicitly across each call and then consumes the new t1 value. The helper's final `or t1,s1,zero` and the caller's subsequent `lbu ...,0(t1)` prove the interface. It is a nonstandard whole-program register contract, not an ordinary ABI call. The source ABI hypothesis in lzss_decode.c is retained for investigation but explicitly excluded from ordinary-body acceptance; it does not express the helper's return side effect and must not be promoted.

The appropriate next lever is a genuine complete helper/caller source group, with the real helper returning the buffer pointer and normal source assignments receiving it, compiled through the actual whole-program IDO pipeline. That compiler may select this custom return register and spill only live shared registers. A stand-in helper, edited assembly or guessed local storage cannot establish the interface. Both original bodies are current source inputs in asm/us/nonmatchings/rom/lib_5610. Source group membership should be proved before any such run. Other ordinary routines also show branch-likely scheduling absent from retail; source-only controls left truthful nonzero residuals rather than relaxing the scoring protocol.

C19 remains the useful frozen accepted-protocol candidate from this worker: unchanged complete initialization source is now strict/raw0 privately, pending parent supported target normalization review and coherent real storage activation.
