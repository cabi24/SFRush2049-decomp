# A25: six real heap-release matches

Frozen group, literal flags `-g0 -O3 -mips2 -G 0 -non_shared`. Six new claims:192 words/768 bytes. Canonical `score.py group --claims` EXIT 0; each claim has strict0, exact emitted extent, zero extras and no unverified/unresolved relocations or verification errors. All five already accepted context functions independently match in this same group, with their complete definition text unchanged (source_audit.json). Shared source/layout/locks were not modified. Full TU SHA256: `e348f956130938f04fae6c3442b50ec1db946b282cd8ccd30c7a35445afccbe0`.

| Claim | Words |
|---|---:|
| audio_reverb_update | 61 |
| audio_effect_process | 23 |
| synced_model_render | 23 |
| MP_TargetSpeed | 28 |
| assign_default_paths | 28 |
| stat_race_end | 29 |

The complete real module is330 words:192 newly claimed words plus19-word range lookup func_80095F8C,38-word tagged block lookup func_80095EF4, and81 words of genuine audio_buffer_sync/object_counter_decrement/object_counter_increment callers. These last five are explicit unclaimed contexts. Their current accepted identity alone was not treated as ABI proof: all five independently strict-match under this exact group. The real external func_80095EC0 remains a declaration, not an ABI substitute body; its existing source fills whole words with0x7FFF0BAD, and the release code invokes it at the actual payload/header addresses and sizes. Queue operations are genuine SDK externals.

## Actual semantic and ABI audit

Despite its historical audio name, audio_reverb_update is a heap release/coalescing operation. Two consumed logical inputs are address and signed tag. Retail physical inputs are a1 and a2; incoming a0 is overwritten before use. The source has exactly these two consumed inputs, with no dummy first argument, hard-bound registers, fabricated wrapper, unused pressure parameter, or source quirk. IPO reproduces the actual argument lanes. The range lookup preserves address/tag across the call; the tagged lookup receives real owner/address/tag. Both exact accepted definitions independently match here. Release uses unsaved s0 for block and s1 for heap; all five callers reproduce actual saves/restores of those two registers.

Block header is32 bytes: next pointer+4, previous pointer bits+8, payload size+12, owning handle address+16, signed used/tag bytes+20/+21 and unsigned counter+22. Extending the existing lookup's partial Block view to the true32-byte header preserves its original member names and definition text. Heap blocks+8 and tail+12 retain real offsets; Pool count/base/next remain real32-bit fields. No new capacity or runtime check was asserted.

Release fills block payload at block+32 for its stored size, clears the real owning handle word if present, and coalesces an unused successor. It stores the successor's next pointer into the current block, tests that newly stored field, fixes the next block's previous link or heap tail, adds both payload sizes plus32, and fills the removed successor's32-byte header. It then clears current owner/used/tag/counter and similarly coalesces into an unused predecessor, fixing links/tail and filling the removed current header. The target's successor cached-value semantics were repaired explicitly: re-reading the other block's next field after writing the current field can differ under aliasing and emitted redundant loads. Testing the actual newly stored field reproduces target behavior and reaches strict0 for the entire61-word helper. This is a semantic source repair, with no added operation.

The two23-word wrappers take one actual consumed address, receive the queue, release address/tag0, and jam the queue. The two28-word flag wrappers inspect signed byte flags at0x8011ED04 and0x8011ED00, respectively, perform the same queue/release sequence on the true0x8038A400 allocation, and clear their flag afterward. stat_race_end consumes a full-word index, selects the allocation word in a real24-byte entry view at0x8013FEF4+index*24, snapshots it before waiting for the queue, releases it with tag0, and jams the queue. Only its entry allocation field/stride are asserted; other fields are opaque. Naming the genuinely used entry pointer before loading its allocation corrects the local address home from compiled sp+36 to actual sp+32, with no dummy storage or runtime operation.

## Bounded controls and reproduction

Eight source builds total: initial faithful closure; explicit cached-successor temporary; corrected newly stored successor field; four genuine stat pointer/signed/entry-view controls; final canonical verification. Initial four wrappers matched immediately, helper41/61 with3 extras and stat2/29. Cached successor removed extras but changed allocation to25/61; actual stored-field repair made helper61/61. Pointer-local, pointer-field, signed-local controls leave stat2/29. Actual entry view makes stat29/29; all other functions remain exact. controls.json contains sanitized counts only.

Reproduce: `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_heap_release_a25 --claims`. scores.json records final sanitized count/error/hash and captured EXIT0. Raw object/disassembly/CLI output remain ignored build/codex-A25 or private Rocky A, never this packet. Root must independently score and pass source-image/full-ROM gates before accepting claims. No ROM byte stream is included.
