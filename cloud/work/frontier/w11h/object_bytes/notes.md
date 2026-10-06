# object_bytes23_sum (72 B) 4/18, object_bytes_sum_global (84 B) 11/21 - IPA kill-set residual

Sources (best.c) are the locked group's context bodies (codex_sound_channel_extra). Unit:
`blob_unit --tag w11h score object_bytes23_sum object_bytes_sum_global --with .../best.c --neighbours` ->
`FAIL object_bytes23_sum: 4 of 18 words differ` / `FAIL object_bytes_sum_global: 11 of 21 words differ` /
`locked bodies that differ in this unit: 0`. group_probe/ = the locked group with both added as members
(same 4/18, 11/21 in score.py group).

Traced (`ctrace.sh object_bytes23_sum ob`): the &D_801497F0 web picks the lowest caller-saved register not killed
by `sound_update_channel(0)`: available0=0x00f80000 = t1..t5 (t0 is the register parameter), so t2. Retail takes
t4, so in retail the kill set of sound_update_channel (with its callee func_80096288) contained t2 and t3 but not
t1/t4. Neither body emits t2/t3 in retail, so the difference is in what uopt *believed* those procedures use.
The locked func_80096288 is a stand-in (`if(0){switch...} if(c){}`). Next hypothesis: a func_80096288 (or
sound_update_channel) source that compiles to the same words but has uopt-coloured webs in t2/t3 (e.g. a
compiled-out debug arm with live parameters); test with blob_unit --with on both callers.
