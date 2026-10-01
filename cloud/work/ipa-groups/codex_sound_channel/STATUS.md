# Codex real sound-channel closure, 2026-10-01

Strict `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_sound_channel --claims` on Rocky A reports MATCH for mode_byte2_set (20 words), object_type_byte2_get (10), object_type_byte3_get (10), mode_byte_set (20): 240 bytes. Exact final group.c/group.json were copied to Rocky before rescoring. Claims do not overlap blob_matched.lock.json. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

## Real context and compilation quirk

No synthetic callers or replacement runtime bodies are present. The group contains reconstructed real sound_update_channel, real no-op func_80096288 and its real second caller slot_value_get. Kept roots are the four claims plus slot_value_get. Both sound_update_channel and func_80096288 are internal.

The real func_80096288 is `beqz a2,+8; nop; jr ra; nop`: it tests the third argument but has no runtime side effects. Its reconstructed C includes an unreachable `if (0) switch (a+b+c)` with stores, followed by the empty check. The unreachable switch inhibits umerge inlining; uopt deletes its effects. This is a compile-affecting source quirk, not an added runtime stand-in. Once this real callee is internal and has two real callsites, the four callers pass sound's force argument in t0 and match. Ordinary empty/early-return/goto forms lose that metadata; keeping the debug callee also fails. Eight first controls and eight keep/dead-switch controls were tested serially.

The group does NOT match its context: sound_update_channel 122/122 differing positions, with 3 extra nonzero words; func_80096288 emits two words (`jr ra;nop`) versus the four target words; slot_value_get 13/15 differing positions, with 1 extra nonzero word. Context is never claimed or linked as new coverage. The empty function's shorter body has the same observable behavior and no added clobbers. Only the image/ROM gate can accept claimed callers against preserved retail context.

## Semantic audit of sound's real body

Checked the full 122-word assembly, not just the call signature. It loads index D_80149780, calls func_80096288(index,0,0), fetches the bank via 20-byte D_80156D44[index] records and one pointer indirection, and refreshes state when forced or bank changes. Bank fields correspond to target offsets 1 (mode), 4/8 (mode bytes), 10 (pointer remapping flag), 11 (forced-record count), 12 (entry count). Entry stride is 12 bytes; target entry ID is offset 4. The historical sound.c seed incorrectly put it at offset 5; this group repairs Entry to `ptr,id,pad[7]` and the claims still match.

Pointer initialization uses bank+16, then bank+16+entry_count*12; each pointer advances by entry_count. The 256 signed-short index slots at D_80149878 initialize to -1 and then map byte IDs to entry indexes. Forced state loads the first word of the 8-byte D_80151AE8[D_801497A4] record, fills D_80149820 with increments of 36, and copies mode bytes. Final D_80149B08/D_80149B28 assignments are mode==1 ? 4:3 and mode==1 ? 0:1, matching target.

Remaining differences are compile choices, not identified behavior substitutions: the empty callee's parameter ABI, stack-home stores, reloads around that call, base/loop register allocation, and scheduling. No data definitions or invented address symbols are introduced. Semantic reconstruction has no known missing target operation after the entry-ID repair; it is not proven to be original source. The compiled sound body differs extensively, so do not promote it independently.

Coordinator must independently rescore, image-check, confirm context remains preserved, and pass full-ROM SHA-1 before claiming cartridge coverage. This worker made no splice, lock, layout, commit or coordinator-state changes.

Bonus mode_byte_set was first tested in scratch, then added to the exact delivered members/keep/claims. All four exact final callers were re-scored with strict score.py --claims after delivery.
