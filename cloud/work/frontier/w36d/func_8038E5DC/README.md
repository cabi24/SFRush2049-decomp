# func_8038E5DC = updateTrackScreen (MAT3 trackscr.c), kept IPA root — one instruction off

best_unit.c = the whole trackscr unit. Every word equals retail except case 14's `gOptions[29] = 3` store, which retail
emits absolutely (`lui at,0x8014; sb t8,0x6125(at)`) and IDO emits `sb t8,29(v0)` from the case's gOptions base.
Writing that one store through a second symbol for the same address (`D_8014611D[8] = 3`, diagnostic)
gives a strict MATCH, so the retail source stored through a different base symbol there; genuine spelling unknown.

Diagnostic edit (not a deliverable): in best_unit.c case 14, replace
`if (D_80146108[29] < 0) D_80146108[29] = 3;` by `if (D_80146108[29] < 0) D_8014611D[8] = 3;` and add
`extern s8 D_8014611D[];` -> `score.py group` reports func_8038E5DC `MATCH (4 section-relative relocations unverified:
.rodata+0xa8 at +0x37c, .rodata+0xa8 at +0x384, .rodata+0xfc at +0xd28, .rodata+0xfc at +0xd30)` (the jump tables).
