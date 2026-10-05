# PrevMaxPath cluster (side result of NextMaxPath; not assigned, not claimed)

`lead.c` is a single-file lead: PrevMaxPath 4/24, InitMaxPath 8/34, sync_maxpath_to_checkpoint 8/34 with
`--keep InitMaxPath,sync_maxpath_to_checkpoint,display_enable,MP_TargetSpeed,assign_default_paths`
(`../full.sh PrevMaxPath/lead.c NAME --flags '"-g0 -O3 -mips2 -G 0 -non_shared"' --keep …`).
`MP_TargetSpeed`, `assign_default_paths` and `display_enable` in it are rough context, not the locked sources.
`g/` is the same idea as a real multi-file group on top of `groups/NextMaxPath` (integer-literal form, older).
`gs/`, `gs3/`, `ex.c`, `t*.c`, `base*.c`, `v1/`, `v2/` under `../NextMaxPath/` and here are experiments; anything
named `standin_*` is a stand-in and proves nothing.
See RESULTS.md section 2 for the findings and the open register permutation.
