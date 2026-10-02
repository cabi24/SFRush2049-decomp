# func_800F56E0 access audit

Canonical linked assembly: asm/us/blob/blob_800ef5b0.s, section func_800F56E0 beginning line6350; 518 words / 2072 bytes, next real head track_lighting_setup at 800F5EF8. No game-global or production mutation performed. `types.h` supplies a compact native context, with unknown padding explicit and no invented runtime values.

## Confirmed roots and arrays

| Address | Declaration | Evidence |
| --- | --- | --- |
| 8014A118 | input_rec0, Player76[] | Player loop advances 76B; index lbu+0, selector lbu+1, count lhu+64, handle lw/sw+72 |
| 8014A108 | active_player_count, s16 | lh at function entry and after notification |
| 80146150 | Object*[] | fallback stores address of pointer slot selector*4 into Player.handle |
| 80152818 | player_array, Model952[] | index computed `((index*32-index)*8-index)*8`=952*index; lb238/239, lwc1+264 |
| 80150F88 | Stats96[] | mode*96, selected as second table; resource table is data+140+mode*96 |
| 80151690 | Owners60[] | mode*60, first pointer table+0, second+20; untouched bytes40..59 retain padding |
| 80151AC0 | s8 insertion player tags[10] | first table tags+rank, second tags+5+rank; no mode index |
| 80149A78 | Times32[] | input player loop index*32, samples consumed sequentially in steps4 |
| 80144018 | u8 sample counts[] | indexed by input player loop index |
| 8014978C | s8 mode | signed byte; default table index is mode, alternate is mode+6 |
| 80152570 | s8 alternate bank | nonzero selects index mode+6 and resource notification mode+19 |
| 80142760 | s8 special flag | nonzero enables additional completion counters |
| 80152734 | s16 requested samples/laps | compared with unsigned per-player sample count before +82 increment |
| 80152744 | s8 participant count | thresholds>=2,>=3,>=4 for placing0,1,2 respectively |

Resource pointer chain: Player.handle->Object (+0), Object.resource (+44), Resource.data (+0). Object word+8 gates updating the owner pointer on insertion; it does NOT gate writing the player tag or shifting existing owners. Null Resource returns from the whole function. No direct calls except `func_800CD8EC(handle,(u8)notification_selector)` after both tables per player.

## Two ranked arrays and aggregate fields

Stats96 best_individual is five floats at +4; best_average is five floats at +24. Each actual sample is inserted independently into the first array if an entry is zero or sample<entry. Existing entries shift down from index4 to insertion index+1. Equal nonzero values do not insert at that slot. The average is computed from all current samples and inserted similarly into the second array when sample_count>0. Default/global table insertions also shift associated owner pointers and player tags. There is no profile ownership bookkeeping on the resource table.

For each selected stats record:
- +64 f32 total_time accumulates each sample separately, retaining original addition order.
- +68 u16 total_samples adds current sample_count.
- +70 u16 completions increments only when model byte+239 is nonzero.
- +72/+74/+76 u16 podium counts increment for model signed place+238 equal0/1/2 and global participant count>=2/3/4 respectively. Also gated by byte+239!=0.
- +78 u16 count_sum adds Player.count(+64), regardless of completion.
- +80 u16 enabled_completions increments under completion gate and special flag.
- +82 u16 enabled_full_runs increments under same gates AND requested samples==current sample_count. Naming this field 'enabled_firsts' or testing place would change behavior.
- +88 u32 distance updates as `(u32)((float)stats->distance + model->distance / 528.0f)`. This is NOT equivalent to `stats->distance += (u32)(model->distance/528.0f)`; full round trip and compiler float->u32 sequence must remain. Divisor bits44040000 are exactly528, not5280.
- +92 u16 bits is accessed by neighboring func_800B78F0; target itself does not access it.

## Reference and naming confidence

Compared actual accepted sources `src/blob/graphics_chunk_b.c`, `src/blob/audio_update_a.c`, and cloud/work/tiny_A102/func_800B78F0.c. These confirm Player76 and Resource/Object/Handle layouts independently; B78F0 confirms resource record bank+140, stride96, and halfword+92. Their existing names graphics/audio are not evidence of target rendering/audio behavior: these bodies aggregate profile/game statistics.

Searched rushtherock `game/stats.c`, `game/hiscore.c`, `game/modeldat.h`, checkpoint sources and full game tree for best-lap/time/distance/profile logic. No matching two-five-entry per-profile routine found. Arcade CAR_DATA has named `place` and `distance`, supporting those meanings with moderate confidence, but its offsets/layout are not N64 truth. Arcade audit stats and ten-entry high-score tables are different systems. Best-individual/best-average naming follows direct target comparisons and arithmetic; 'lap' naming remains inferred from per-player lap/sample accumulation.
