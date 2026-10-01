# A14: real guarded heap load — NONMATCH, five-word permutation

Frozen2026-10-01. Empty claims. Current shared root locks checked: all five module functions are unlocked. No stand-in, fake caller, unused invented formal, guard trick, frame padding or dead runtime read. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Five authentic bodies232 retail words: PrevMaxPath24, InitMaxPath34, sync_maxpath_to_checkpoint34, display_enable79 and audio_reverb_update61.

Final PrevMaxPath strict5/24, emitted24, zero extras/unresolved/unverified/errors. All five differences are genuine incoming saved-register inputs; instruction forms, size and branches otherwise match. Retail uses dst s0, clear_end s1, clear_start s2, flag s3, ROM source s4, image end a2. Compiler uses dst s0, clear_start s1, flag s2, ROM source s3, clear_end s4, same a2 end. Twenty-four permutations of the four consumed clear/flag/ROM formals with corresponding actual-call permutations are inert at5/24. No physical register names appear in source.

The canonical --claims exit0 means work in progress because claims are empty. Context is unclaimed and NONMATCH: Init34/34 emitted28, sync34/34 emitted28, display75/79 emitted79, real free helper21/61 emitted61. scores.json and score_claims.txt preserve the verdicts. No source-image or ROM acceptance is asserted.

## Actual source and logical input audit

PrevMaxPath800A11E4 consumes six real inputs: destination, destination end, clear range start/end, loaded-byte pointer and compressed-ROM pointer. If the loaded byte is nonzero, it returns. Otherwise it reserves the destination range through actual NextMaxPath(dst,end-dst), calls actual audio_loop_control(reservation,0), decompresses through inflate_decompress(rom,dst,1), clears the requested range with bzero(start,end-start), and sets the flag byte to1. This is the complete24-word retail sequence. Six-formal declaration order is a reconstruction; historical source ordering is not asserted.

InitMaxPath supplies dst8038A400/end803BB380, clear80394F70..8039B440, flagD_8011ED04, ROM pointer loaded fromD_8002B024. sync_maxpath_to_checkpoint supplies the same destination range, clear803B9A50..803BAE20, flagD_8011ED00, ROM pointer loaded fromD_8002B020. The actual callers also store two historical dead stack values (D_80123860/3850 and BE4C70/BDA100) that PrevMaxPath never reads. Those were not turned into fabricated unused formals or pressure-only locals; hence caller frame56 versus retail64 and missing six caller words are documented, not claimed matches.

Display enable uses D_801174C0 as its mode flag. Before changing mode it waits on the actual heap queue D_80152770 when the previous image flag is set, frees the actual8038A400 heap allocation, jams the queue back and clears the previous flag. Enabling then invokes PrevMaxPath with the sync input set and sets the mode flag; disabling only clears it after optional free. Actual free helper takes one consumed logical pointer; retail incominga1 carries it. Its historical unuseda0/a2 inputs were not invented as formals. Free helper's actual body is included so whole-program compilation can select its own internal ABI rather than a false public prototype. Its retail/candidate ABI/allocation mismatch remains unclaimed.

Free helper body was reconstructed from its own61 words: finds heap owner and block, releases memory through func_80095EC0, clears optional owner backreference, joins the next free block (updating neighbor backpointer/heap tail), clears block ownership/used bytes, then joins a free previous block. Real func_80095F8C/95EF4 interfaces return pointers; real95EC0 is a consumed two-input operation with ignored return. No heap stubs replace those callees.

## Closure limit and next concrete lever

Direct graph gives171 words for target and its three callers. Full caller-chain evidence adds real playgame_state_change, speed_set, audio_frame_sync and slot_state_setup, reaching1046 words: display_enable itself depends on its genuine parent's preservation of s0–s4. Current packet keeps display's ABI, making it save five registers that retail leaves for that parent. Recovering those parent and hidden-callee inputs is a justified future context lever. It has not been replaced with a dummy wrapper or arbitrary duplicate arguments.

A NextMaxPath/audio_loop_control decompiler inspection identified unset-register artifacts caused by authentic live t0/t2/a3 contracts. Those raw decompiler bodies are only ignored build scratch and are not delivered as source context. Further inclusion would require a real input/clobber repair, not zero-valued M2C_ERROR placeholders. The delivered five definitions contain none of those placeholders.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_heap_load_a14/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_heap_load_a14/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_heap_load_a14 --claims'
```

All controls were sequential on Rocky A; caches, objects and disassembly stay ignored/private. Frozen group.c SHA256: `7af5a414128626d8194289e8e2e9f4e2e40058c8ce5976917959aaa1d26ca929`.
