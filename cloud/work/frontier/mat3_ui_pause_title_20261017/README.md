# Pause title pane: exact target, incomplete group

Research only, base `58425072`, 2026-10-17. `voice_stop` is the historical
mislabel for the pause-screen title pane at `0x800B438C`, 552 bytes. No audio
semantics are inferred from that label. No accepted-image or ROM credit.

## Result

The complete target is a strict **MATCH, 138/138 words**, with its native
152-byte frame and text buffer at sp+72. Stock and instrumented compiler
pipelines produced byte-identical complete objects. No padding array,
unused local, dummy call, fake prototype, forced register, or target edit
was introduced.

This is not an installation packet. Every known emitted body, including
the partial caller and inherited font bodies, is listed under `members`;
`context` and `claims` are empty. Fresh canonical full-group results:

- voice_stop: MATCH
- func_800B4FB0: 314/368 words differ
- slot_state_setup: 2/58 words differ, 3 extra nonzero words
- object_create: 28/28 words differ (deleted/internalized body)
- func_800B669C: MATCH, inherited, no new credit

The real B4FB0 caller keeps the private title helper out of line naturally.
The other real caller, RaceStateMachine_Update, has no complete body in this
packet. The current genuine selector copy is verbatim
`src/blob/groups/credits_scroll_grp/gr3_b.c`; it is not presented as a locked
replacement. The B4FB0 caller is preserved from the complete historical
`dot_menu_state_b4fb0_20261006/candidate.c`, changing only the obsolete
osJamMesg label to the current osSendMesg symbol. Its body is still partial
in the matching sense. All protected files and production groups are unchanged.

## Source evidence and transformations

GC `pausescr::_TitlePane`, address `0x80078CF8`, size 528, DWARF DIE
`0x1689C2`, records the ordered locals `int x, y, width; unsigned char *str;
unsigned char str1[64]`. The current MAT3 source supplied by the owner has
`str = str1` immediately after HAL_SetFont(13), used by both formatted-text
width calls. Its source SHA-256 is
`69cefdd0ced15c6bdd8fdcf9880a8387f50c2a08dacaff906b0f5429f8d1296f`.
The donor was read, not freshly compiled for this packet.

The N64 reconstruction preserves those genuine locals and buffer, replacing
the earlier inferred text[80], extra half-width local, and s16 coordinates.
GC's indices single[242] and tab[58] map to N64 single[233] and tab[56]. N64
has no GC null-save fallback at this site. It calculates width/2 in the final
panel call; directly transplanting GC's earlier width/=2 produces 14/138
differences. Coordinates are loaded after width selection. The panel is
created before the zero-width test and hidden by the (-20,0,-10,10) rectangle.

HAL_SetFont's authentic int old return value and nested semaphore helpers
restore the apparently unused font-return copy. Each real helper is written
on one physical source line together, disclosing the as1 source-location tie
that retains the native instruction order. Separate definition lines give
4/138 differences. These helpers use the genuine N64 queue operations;
the GC empty semaphore bodies are not transplanted.

Removing the genuine str variable gives two buffer-offset differences;
placing it after the array gives the same two-word failure. The adopted
source has a live str assignment and uses, not an unused frame filler.

## Replay and limits

`python3 cloud/work/frontier/mat3_ui_pause_title_20261017/verify.py`

The replay pins only packet source and canonical target-word digests,
freshly compiles the complete group, reports every member, explicitly keeps
the incomplete closure unclaimed, and rejects font 12, the wrong text-table
index, and separate helper definition locations. It works unchanged under
`python3 -O`. No broad test suite, behavior harness, full integration, CI
watcher, image splice, or full-ROM gate is claimed.
