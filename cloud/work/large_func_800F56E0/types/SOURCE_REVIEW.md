# Final source-shape review

Reviewed `audit/entry/load_twice.c` and `reconstruction/direct_record.c` independently against the original native behavior and initial reconstruction.

`resource_course=D_8014978C; course=D_8014978C;` uses two consecutive source reads of the same nonvolatile signed byte. There is no intervening write or call; common-subexpression elimination to the original single load is permitted and produces equivalent values. This expresses the two actual independently consumed selector locals.

Replacing local `PlayerRecord *record=&input_rec0[player]` references with `input_rec0[player].field` preserves the exact object addressed: player remains unchanged throughout both stats phases, and the static input_rec0 root does not change. Field reads are still live reads; only the redundant local pointer is removed. The existing resource fallback assignment, null return, model lookup, owner gate and callback remain at the original control-flow points.

Combined source still implements both five-entry ranked arrays, native owner/tag shifts, all completion and sample counters, and the full uint distance/float round trip. There is no artificial stack pressure, unused local, runtime operation, conditional omission, helper-only claim or hidden function tail.

The original type audit's completion flag correction is reflected here: Model952 byte+239 is nonzero on the completion branch; +82 checks requested sample count equality and does not check placing.
