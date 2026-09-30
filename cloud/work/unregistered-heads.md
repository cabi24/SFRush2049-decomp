# Unregistered function heads in the game image

Prepared for the maintainers to register in the function table. Branch `claude/round3-cloud-work`.
Every row was checked against the retail words: the game image was inflated from `assets/us/data.bin`
(raw DEFLATE at ROM 0xB0CB10, 326180 bytes, image base 0x80086A50, 647072 bytes) as `cloud/work/tools/extscore.py`
does, and compared with `asm/us/blob/symbols.json`. Words = (end - address) / 4, end is exclusive and
includes the `jr ra` delay slot. "Frame" is the `addiu sp,sp,-N` size of the prologue.

## Read this first: INDEX labels are not function names

The group labels `dynamic_difficulty`, `catchup_logic`, `reconnect_attempt`, `skill_rating_update`,
`matchmaking`, `session_host`, `session_join`, `session_leave`, `disconnect_handler`, `highscore_entry_anim`,
`game_over_screen`, `trophy_animation`, `name_entry_screen` (0x80104320), `award_ceremony`, `race_results_screen`,
`statistics_display`, `achievements_check`, `achievement_unlock/display`, `profile_stats_update`,
`difficulty_scaling` and similar are all **mid-function (tail) labels** inside larger unregistered functions; the
INDEX "insns" counts are the words from that label to the function's epilogue. The real functions are the heads
below and should be named `func_<addr>` (for example `dynamic_difficulty` is the tail of `func_80107EDC`,
`catchup_logic` of `func_801084D4`, `reconnect_attempt` of `func_8010BC84`, `highscore_entry_anim` of
`func_80104704`, `game_over_screen` of `func_80102F30`). The suggested names below are placeholders of that form.

Evidence pattern for every row: the head follows a `jr ra` + delay slot, starts with (or within 2 instructions of) an
`addiu sp,sp,-N` prologue, contains exactly one `jr ra` (so the extent is unambiguous), and is referenced either by a
`jal` or by a raw pointer word in a handler/descriptor table (0x80114xxx to 0x80117xxx). None of these addresses
has a symbol in `symbols.json` (checked), so they are unregistered. Each opaque `.incbin` run in
`asm/us/blob/blob_*.s` hides these heads.

## Confirmed: heads named in the dynamic_difficulty / catchup_logic / reconnect_attempt STATUS notes

Note the STATUS numbers in parentheses for 0x80108154..0x8010C02C ("frame 152", "224", "200", "40", "168") are
**frame sizes, not word counts**; the real sizes are in the table.

| suggested name | address | words | bytes | frame | cloud group / notes referencing it | evidence (head / end) | status |
|---|---|---|---|---|---|---|---|
| func_80107EDC | 0x80107EDC | 158 | 632 | 72 | dynamic_difficulty (STATUS, group.json target; label 0x80108098 is mid-function) | head after jr ra+delay at 0x80107ED4; addiu sp,sp,-72 at head; single jr ra at 0x8010814C (delay slot 0x80108150); word 0x80107EDC in descriptor/pointer table at 0x80115184 (end excl. 0x80108154) | unregistered (no symbol at this address in symbols.json) |
| func_80108154 | 0x80108154 | 224 | 896 | 152 | dynamic_difficulty STATUS head list; catchup_logic STATUS (no label inside) | head after jr ra+delay at 0x8010814C; addiu sp,sp,-152 at head; single jr ra at 0x801084CC (delay slot 0x801084D0); word 0x80108154 in descriptor/pointer table at 0x801151A8 (end excl. 0x801084D4) | unregistered (no symbol at this address in symbols.json) |
| func_801084D4 | 0x801084D4 | 375 | 1500 | 224 | catchup_logic (STATUS, group.json target; label 0x801089CC is mid-function) | head after jr ra+delay at 0x801084CC; addiu sp,sp,-224 at head; single jr ra at 0x80108AA8 (delay slot 0x80108AAC); word 0x801084D4 in descriptor/pointer table at 0x80115238 (end excl. 0x80108AB0) | unregistered (no symbol at this address in symbols.json) |
| func_80108AB0 | 0x80108AB0 | 190 | 760 | 200 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x80108AA8; addiu sp,sp,-200 at head; single jr ra at 0x80108DA0 (delay slot 0x80108DA4); word 0x80108AB0 in descriptor/pointer table at 0x8011525C (end excl. 0x80108DA8) | unregistered (no symbol at this address in symbols.json) |
| func_80108DA8 | 0x80108DA8 | 102 | 408 | 40 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x80108DA0; addiu sp,sp,-40 at head; single jr ra at 0x80108F38 (delay slot 0x80108F3C); word 0x80108DA8 in descriptor/pointer table at 0x80115280 (end excl. 0x80108F40) | unregistered (no symbol at this address in symbols.json) |
| func_80108F40 | 0x80108F40 | 330 | 1320 | 168 | dynamic_difficulty STATUS head list; INDEX-style labels skill_rating_update 0x80108F6C, matchmaking 0x801092D8 are mid-function | head after jr ra+delay at 0x80108F38; addiu sp,sp,-168 at head; single jr ra at 0x80109460 (delay slot 0x80109464); word 0x80108F40 in descriptor/pointer table at 0x801152A4 (end excl. 0x80109468) | unregistered (no symbol at this address in symbols.json) |
| func_80109468 | 0x80109468 | 382 | 1528 | 72 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x80109460; addiu sp,sp,-72 at head; single jr ra at 0x80109A58 (delay slot 0x80109A5C); word 0x80109468 in descriptor/pointer table at 0x80115790 (end excl. 0x80109A60) | unregistered (no symbol at this address in symbols.json) |
| func_80109A60 | 0x80109A60 | 317 | 1268 | 40 | dynamic_difficulty STATUS head list; labels session_host 0x80109A98, session_join 0x80109EFC are mid-function | head after jr ra+delay at 0x80109A58; addiu sp,sp,-40 at head; single jr ra at 0x80109F4C (delay slot 0x80109F50); word 0x80109A60 in descriptor/pointer table at 0x801157D8 (end excl. 0x80109F54) | unregistered (no symbol at this address in symbols.json) |
| func_8010A53C | 0x8010A53C | 154 | 616 | 64 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x8010A534; addiu sp,sp,-64 at head; single jr ra at 0x8010A79C (delay slot 0x8010A7A0); word 0x8010A53C in descriptor/pointer table at 0x80115A60 (end excl. 0x8010A7A4) | unregistered (no symbol at this address in symbols.json) |
| func_8010B7FC | 0x8010B7FC | 115 | 460 | 56 | dynamic_difficulty STATUS head list; label disconnect_handler 0x8010B874 is mid-function | head after jr ra+delay at 0x8010B7F4; addiu sp,sp,-56 at head; single jr ra at 0x8010B9C0 (delay slot 0x8010B9C4); word 0x8010B7FC in descriptor/pointer table at 0x801172E8 (end excl. 0x8010B9C8) | unregistered (no symbol at this address in symbols.json) |
| func_8010B9C8 | 0x8010B9C8 | 175 | 700 | 152 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x8010B9C0; addiu sp,sp,-152 at head; single jr ra at 0x8010BC7C (delay slot 0x8010BC80); word 0x8010B9C8 in descriptor/pointer table at 0x8011730C (end excl. 0x8010BC84) | unregistered (no symbol at this address in symbols.json) |
| func_8010BC84 | 0x8010BC84 | 232 | 928 | 64 | reconnect_attempt (STATUS, group.json target; label 0x8010BE7C is mid-function) | head after jr ra+delay at 0x8010BC7C; addiu sp,sp,-64 at head; single jr ra at 0x8010C01C (delay slot 0x8010C020); word 0x8010BC84 in descriptor/pointer table at 0x80117330 (end excl. 0x8010C024) | unregistered (no symbol at this address in symbols.json) |
| func_8010C02C | 0x8010C02C | 174 | 696 | 168 | dynamic_difficulty STATUS head list (no label inside) | head after jr ra+delay at 0x8010C024; addiu sp,sp,-168 at head; single jr ra at 0x8010C2DC (delay slot 0x8010C2E0); word 0x8010C02C in descriptor/pointer table at 0x80117528 (end excl. 0x8010C2E4) | unregistered (no symbol at this address in symbols.json) |

## Confirmed: additional unregistered heads found while verifying (not in the requested list)

The same scan shows more unregistered heads in the same opaque runs, with the same evidence. They are listed so the
maintainers can register the whole range in one go. The range 0x8010221C..0x80107EDC also contains registered heads
(`func_80107AEC`, `race_finish`, `checkpoint_hit`, `lap_complete`, ...) which are omitted.

| suggested name | address | words | bytes | frame | cloud group / notes referencing it | evidence (head / end) | status |
|---|---|---|---|---|---|---|---|
| func_80109F54 | 0x80109F54 | 376 | 1504 | 136 | NOT in any STATUS list; label session_leave 0x8010A4C0 is mid-function | head after jr ra+delay at 0x80109F4C; 2 pre-prologue insns (lui/load) then addiu sp at 0x80109F5C; single jr ra at 0x8010A52C (delay slot 0x8010A530); word 0x80109F54 in descriptor/pointer table at 0x801157B4 (end excl. 0x8010A534) | unregistered |
| func_8010B5D0 | 0x8010B5D0 | 139 | 556 | 152 | NOT in any STATUS list; start of blob_8010b5d0.s (reconnect_attempt STATUS mentions the file only) | head after jr ra+delay at 0x8010B5C8; addiu sp,sp,-152 at head; single jr ra at 0x8010B7F4 (delay slot 0x8010B7F8); word 0x8010B5D0 in descriptor/pointer table at 0x801172C4 (end excl. 0x8010B7FC) | unregistered |
| func_80102F30 | 0x80102F30 | 890 | 3560 | 640 | bigfish/maintainer note; trophy_animation 0x8010306C and game_over_screen 0x80103A08 are mid-function | head after jr ra+delay at 0x80102F28; addiu sp,sp,-640 at head; single jr ra at 0x80103D10 (delay slot 0x80103D14); word 0x80102F30 in descriptor/pointer table at 0x8011471C (end excl. 0x80103D18) | unregistered |
| func_80103D28 | 0x80103D28 | 629 | 2516 | 264 | INDEX name_entry_screen (head 0x80103D28); label name_entry_screen 0x80104320 is mid-function | head after jr ra+delay at 0x80103D20; addiu sp,sp,-264 at head; single jr ra at 0x801046F4 (delay slot 0x801046F8); jal from 0x801054EC (end excl. 0x801046FC) | unregistered |
| func_80104704 | 0x80104704 | 260 | 1040 | 136 | highscore_entry_anim STATUS (label 0x80104A58 is mid-function; STATUS could not find the head) | head after jr ra+delay at 0x801046FC; addiu sp,sp,-136 at head; single jr ra at 0x80104B0C (delay slot 0x80104B10); jal from 0x80105730 (end excl. 0x80104B14) | unregistered |
| func_8010221C | 0x8010221C | 137 | 548 | 24 | INDEX/blob_8010221c.s start; label race_results_screen 0x80102250 is mid-function | head after jr ra+delay at 0x80102214; addiu sp,sp,-24 at head; single jr ra at 0x80102438 (delay slot 0x8010243C); jal from 0x80102F90 (end excl. 0x80102440) | unregistered |
| func_80102448 | 0x80102448 | 334 | 1336 | 376 | unlabelled head in the same opaque run (blob_8010221c.s) | head after jr ra+delay at 0x80102440; 2 pre-prologue insns (lui/load) then addiu sp at 0x80102450; single jr ra at 0x80102978 (delay slot 0x8010297C); jal from 0x80102FC4 (end excl. 0x80102980) | unregistered |
| func_80102980 | 0x80102980 | 364 | 1456 | 480 | label award_ceremony 0x80102A74 is mid-function | head after jr ra+delay at 0x80102978; 2 pre-prologue insns (lui/load) then addiu sp at 0x80102988; single jr ra at 0x80102F28 (delay slot 0x80102F2C); jal from 0x80102FD4 (end excl. 0x80102F30) | unregistered |
| func_80104B14 | 0x80104B14 | 601 | 2404 | 576 | label statistics_display 0x80104E84 is mid-function (blob_80104b14.s start) | head after jr ra+delay at 0x80104B0C; addiu sp,sp,-576 at head; single jr ra at 0x80105470 (delay slot 0x80105474); jal from 0x80105760 (end excl. 0x80105478) | unregistered |
| func_80105480 | 0x80105480 | 445 | 1780 | 584 | label achievements_check 0x80105858 is mid-function | head after jr ra+delay at 0x80105478; addiu sp,sp,-584 at head; single jr ra at 0x80105B6C (delay slot 0x80105B70); word 0x80105480 in descriptor/pointer table at 0x801148C4 (end excl. 0x80105B74) | unregistered |
| func_80105B74 | 0x80105B74 | 141 | 564 | 88 | unlabelled | head after jr ra+delay at 0x80105B6C; addiu sp,sp,-88 at head; single jr ra at 0x80105DA0 (delay slot 0x80105DA4); word 0x80105B74 in descriptor/pointer table at 0x801158F8 (end excl. 0x80105DA8) | unregistered |
| func_80105DA8 | 0x80105DA8 | 64 | 256 | 24 | unlabelled | head after jr ra+delay at 0x80105DA0; 2 pre-prologue insns (lui/load) then addiu sp at 0x80105DB0; single jr ra at 0x80105EA0 (delay slot 0x80105EA4); word 0x80105DA8 in descriptor/pointer table at 0x80115940 (end excl. 0x80105EA8) | unregistered |
| func_80105EA8 | 0x80105EA8 | 627 | 2508 | 400 | labels achievement_unlock 0x80105EF4 and achievement_display 0x80106260 are mid-function | head after jr ra+delay at 0x80105EA0; addiu sp,sp,-400 at head; single jr ra at 0x8010686C (delay slot 0x80106870); word 0x80105EA8 in descriptor/pointer table at 0x8011591C (end excl. 0x80106874) | unregistered |
| func_80106874 | 0x80106874 | 178 | 712 | 64 | label profile_stats_update 0x801068F4 is mid-function | head after jr ra+delay at 0x8010686C; addiu sp,sp,-64 at head; single jr ra at 0x80106B34 (delay slot 0x80106B38); word 0x80106874 in descriptor/pointer table at 0x801159D0 (end excl. 0x80106B3C) | unregistered |
| func_80106B3C | 0x80106B3C | 150 | 600 | 64 | unlabelled | head after jr ra+delay at 0x80106B34; addiu sp,sp,-64 at head; single jr ra at 0x80106D8C (delay slot 0x80106D90); word 0x80106B3C in descriptor/pointer table at 0x801159F4 (end excl. 0x80106D94) | unregistered |
| func_80106D94 | 0x80106D94 | 401 | 1604 | 64 | label difficulty_scaling 0x80107110 is mid-function | head after jr ra+delay at 0x80106D8C; addiu sp,sp,-64 at head; single jr ra at 0x801073D0 (delay slot 0x801073D4); word 0x80106D94 in descriptor/pointer table at 0x80114E6C (end excl. 0x801073D8) | unregistered |

Cross-checks from the user notes: `name_entry_screen` head 0x80103D28 ends at 0x801046F8 in the notes; that is the
address of the final delay-slot `nop` (inclusive). Exclusive end is 0x801046FC, so 629 words (INDEX says ~628).
`game_over_screen` (0x80103A08) lies inside `func_80102F30` (0x80102F30..0x80103D18, 890 words, frame 640, only one
`jr ra`, at 0x80103D10). `highscore_entry_anim` (0x80104A58) lies inside `func_80104704` (0x80104704..0x80104B14,
260 words, frame 136); its STATUS could not locate the head because `build/game_code.bin` was absent, but the head is
0x80104704 (prologue `addiu sp,sp,-136`, `lui s4,0x8014` loads `0x801461D0`; `jal` caller at 0x80105730). The STATUS note
saying the run before it is "10,300 bytes (0x8010221C-0x80104A58)" is right, and the owner turns out to be the last
head in it.

## Uncertain / needs maintainer check

| suggested name | address | words | bytes | frame | notes referencing it | evidence | status |
|---|---|---|---|---|---|---|---|
| func_8010C2E4 | 0x8010C2E4 | 89 | 356 | leaf | NOT in any STATUS list; follows 0x8010C02C | head after jr ra+delay at 0x8010C2DC; leaf, no sp adjust (saves a2/a3 to home slots); last jr ra at 0x8010C440 (delay slot 0x8010C444); word 0x8010C2E4 in descriptor/pointer table at 0x8011752C (end excl. 0x8010C448) | unregistered |
| (stub) | 0x80102440 | 2 | 8 | 0 | none | `jr ra; nop`, between func_8010221C and func_80102448 | uncertain: padding or a real 2-word function? |
| (stub) | 0x80103D18 | 2 | 8 | 0 | none | `jr ra; nop`, after func_80102F30 | uncertain, as above |
| (stub) | 0x80103D20 | 2 | 8 | 0 | none | `jr ra; nop`, before func_80103D28 | uncertain, as above |
| (stub) | 0x801046FC | 2 | 8 | 0 | none | `jr ra; nop`, between func_80103D28 and func_80104704 | uncertain, as above |
| (stub) | 0x80105478 | 2 | 8 | 0 | none | `jr ra; nop`, between func_80104B14 and func_80105480 | uncertain, as above |
| (stub) | 0x8010A534 | 2 | 8 | 0 | none | `jr ra; nop`, between func_80109F54 and func_8010A53C | uncertain, as above |
| (stub) | 0x8010C024 | 2 | 8 | 0 | reconnect_attempt STATUS ("a separate 2-word `jr ra; nop` follows") | `jr ra; nop`, between func_8010BC84 and func_8010C02C | uncertain, as above |

Notes on the uncertain rows:

- 0x8010C2E4 has no `addiu sp` prologue (leaf, stores `a2`/`a3` to their home slots first) and contains an
  early-return `jr ra` at 0x8010C324 (delay slot `move v0,zero`, branch target of a `bnez` at 0x8010C30C), so a
  naive "first jr ra" scan would give 18 words; the `jr ra` at 0x8010C440 is the last one and 0x8010C448 is the next head (its
  `lui/lh` precede an `addiu sp,sp,-32` at 0x8010C450). The head is supported by the pointer word at 0x8011752C
  directly after the one for 0x8010C02C (0x80117528). More heads exist after 0x8010C448 (the scan shows prologues at 0x8010C450, 0x8010C590, 0x8010C6D0, 0x8010C7F4, some
  possibly registered, e.g. `steering_apply` 0x8010C6C8); not checked here, beyond the requested list.
- The 2-word stubs: the same shape exists as registered symbols elsewhere (`func_80107AEC`, `func_8010B520`,
  `func_80100D24`), so the maintainers probably want to register them as 2-word functions. Whether they are real
  functions or alignment filler is the maintainers' decision.
- Heads 0x80102448, 0x80102980, 0x80105DA8 and 0x80109F54 begin with `lui` and a load that the scheduler hoisted above the
  `addiu sp`. The address given is the first of those instructions, which is what follows the previous `jr ra` delay slot
  (this is how `func_8010A7A4` is registered: its symbol is at the `lui`, the prologue is 8 bytes later).

## Already registered (verified; nothing to add)

Group notes for these say "real head"; `symbols.json` already has a symbol at the exact head, and the
extent matches the STATUS figure. Each has exactly one `jr ra` and starts right after a `jr ra` delay slot.

| group | address | words | bytes | frame | registered as | note |
|---|---|---|---|---|---|---|
| audio_doppler_calc | 0x800B6788 | 281 | 1124 | 112 | `audio_doppler_calc` | head OK; name is wrong (text blitter) |
| func_8008705C | 0x8008705C | 45 | 180 | 24 | `func_8008705C` | `render_init_setup` at 0x80087068 is a mid-function label inside it |
| audio_pitch_adjust | 0x800959DC | 18 | 72 | 24 | `audio_pitch_adjust` | |
| draw_number | 0x800C760C | 110 | 440 | 24 | `draw_number` | |
| best_times_display | 0x800D5BB0 | 56 | 224 | 24 | `best_times_display` | |
| func_800F0F44 | 0x800F0F44 | 116 | 464 | 72 | `func_800F0F44` | |
| billboard_render | 0x800F64D4 | 244 | 976 | 144 | `billboard_render` (also `finish_state_normal`) | `sign_render` at 0x800F6894 is a mid-function label (suspicious) |
| menu_back | 0x800CBE08 | 33 | 132 | 24 | `menu_back` | head fine. STATUS "suspicious" is about missed callers, not the head. Its caller `func_800CBF2C` is registered (69 words, 0x800CBF2C..0x800CC040); `menu_animation_update` at 0x800CC000 is a mid-function label inside it (suspicious) |
| func_8010A7A4 | 0x8010A7A4 | 75 | 300 | 56 | `func_8010A7A4` | `network_sync_full` at 0x8010A83C is a mid-function label inside it (suspicious). Related: `func_8010A8D0` (0x8010A8D0) and `func_8010AEAC` (0x8010AEAC, contains `ping_measurement` 0x8010B284) are already registered |

## Suspicious labels summary

- `highscore_entry_anim` 0x80104A58: tail of `func_80104704` (resolved above; STATUS said head unknown).
- `menu_back`: fine, head registered. The only "suspicious" part is the group generator missing callers.
- `network_sync_full` 0x8010A83C, `ping_measurement` 0x8010B284, `sign_render` 0x800F6894, `render_init_setup` 0x80087068,
  `menu_animation_update` 0x800CC000: labels inside registered functions (no new head).
- `game_over_screen` 0x80103A08, `trophy_animation` 0x8010306C, `award_ceremony` 0x80102A74, `race_results_screen`
  0x80102250, `statistics_display` 0x80104E84, `achievements_check` 0x80105858 and friends: all tails, see tables.

## Method (reproducible)

1. Inflate the image as `extscore.py` does and index it at `addr - 0x80086A50`.
2. Candidate heads = address after every `jr ra` (0x03E00008) + delay slot, where an `addiu sp,sp,-N` (0x27BDxxxx)
   follows within 4 words or the word is a `jr ra; nop` stub.
3. End = last `jr ra` before the next head, plus its delay slot. Every head above has exactly one `jr ra`, except
   0x8010C2E4 (two).
4. Confirm with callers: `jal` encodings (0x0C......) for 0x8010221C, 0x80102448, 0x80102980, 0x80103D28, 0x80104704,
   0x80104B14; raw pointer words in the tables around 0x80114E6C, 0x80115184..0x80115A60 and 0x801172C4..0x8011752C for
   the rest. Check `symbols.json` for an existing symbol at the address (none for any row in the tables above).
