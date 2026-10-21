# w36j results (wave 36, runtime image B: hudextra.c / worlddam.c)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36j (from base = master e68aea6e1).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_80391B00 | UpdateHUDExtraPostCamera@hudextra.c with animBattleCoins + animStuntCoins inlined (confirmed) | 2380 | NONMATCH 407/595 (MAT3 shape, no shaping) or 83/595 with a disclosed compiled-out holder read; residual = live-range split decisions (retail has one more code-free interfering web) | func_80391B00/best/, func_80391B00/best_holder/ |
| func_80392894 | animStuntPoints@hudextra.c (confirmed) | 1872 | **MATCH** (standalone; own .rodata 2.69f unverified on builder) | cloud/matches/ovl_b/func_80392894.c |
| func_80392FE4 | animHealth@hudextra.c (confirmed) | 1332 | **MATCH** (standalone; own .rodata 4 floats unverified on builder, checked by hand) | cloud/matches/ovl_b/func_80392FE4.c |
| func_8038A408 | newWorldDamage@worlddam.c + inlined findDamageSlot (confirmed, w35e) | 1220 | **MATCH** (standalone; 10 disclosed unused words, w35e's frame residual) | cloud/matches/ovl_b/func_8038A408.c |
| func_80390F60 | UpdateBattlePowerUps@powerups.c (proven; inlines UpdateBattleInvs + UpdateInvs) | 988 | **group MATCH** (supersedes dot_runtime_b_resource_callbacks_20261012 in place) | groups/dot_runtime_b_resource_callbacks_20261012/ |
| func_803925D0 | ShowHUDExtra@hudextra.c (confirmed; inlines ClearBattlePoints) | 708 | **MATCH** (standalone) | cloud/matches/ovl_b/func_803925D0.c |
| func_8039244C | RemoveHUDExtra@hudextra.c (confirmed) | 380 | **MATCH** (standalone, first typed port) | cloud/matches/ovl_b/func_8039244C.c |

## Summary
Six of seven assigned functions closed: 5 standalone strict matches in `cloud/matches/ovl_b/` (animStuntPoints
1872, animHealth 1332, newWorldDamage 1220, ShowHUDExtra 708, RemoveHUDExtra 380) and one group match
(UpdateBattlePowerUps 988, group `groups/dot_runtime_b_resource_callbacks_20261012/`, supersedes in place) =
6,500 new image-B bytes. func_80391B00 (2380) is NONMATCH with the residual traced to one live-range split.

## What generalises
- **as1 breaks equal-priority ties by source line.** Three closures came from line placement alone: a one-line
  `{ a; b; }` or `for (...) x = 0;` changes the schedule (animStuntPoints), an inlined static's statements carry the
  callee's lines, so defining it before/after the caller matters (ShowHUDExtra's ClearBattlePoints after), and a
  one-line macro gives a hoisted address the same line as the stores it must follow (newWorldDamage).
- **`.noalias` from islda roots**: loading the value through `G[i].f` instead of `p->f` lets as1 reorder a store pair
  (animStuntPoints); conversely MAT3's block-local `inst = &G[idx]; inst->f |= 1` avoids a noalias region whose
  `.alias` after the arm's `b` blocks as1's cross-block hoist (animHealth). Prove with an edited ugen listing first.
- **MAT3 text beats "simplified" text**: `gNumWindows==2?8:8` (a compiled-out test) is what gives constant 2 its
  register; MAT3's `if (...) return;` / `else if ((x -= dt) < 0.0f)` / `fadeAlpha + 8 > 255` forms; `0.f` vs `0.0f`
  still separates zero webs; MAT3's function-level vs block-level declarations set homes (`car` before invsColor).
- Direct MAT3 typed ports of hudextra.c functions matched on the first or second compile (RemoveHUDExtra first try).

## Identities proven
- func_80390F60 = UpdateBattlePowerUps@powerups.c (GC 0x80033c94): calls func_80390D38, func_80390B10, then the
  inlined UpdateBattleInvs (which inlines UpdateInvs); strict group MATCH.
- func_80390D38 = UpdateBattleHealth@powerups.c (GC 0x80033618): Random(10)+5, last==4, gObjList entry D_80142A82.
- func_80390B10 = UpdateBattleShield@powerups.c (GC 0x80033840): Random(60)+90, last==3, D_80142A8E.
- func_80390B08 = UpdateBattleInvs@powerups.c deleted stub; func_80390B00 = UpdateInvs@powerups.c deleted stub.
- D_80399AE0 = gBattlePowerUps (tBattlePowerUps, 0x14); D_803942B8 = UpdateInvs' invsColor initializer.
- func_80391B00 = UpdateHUDExtraPostCamera@hudextra.c (inlines animBattleCoins, animStuntCoins).
- func_80391864 = animWeaponIcons@hudextra.c; func_80391650 = animPowerupIcons@hudextra.c (both called by 91B00
  with the same switch/kind logic as MAT3).
- func_8039244C = RemoveHUDExtra; func_803925D0 = ShowHUDExtra (inlines ClearBattlePoints; func_803925C8 is its
  deleted stub); func_80392894 = animStuntPoints; func_80392FE4 = animHealth (all hudextra.c, strict MATCH).
- func_80393518 = animAmmoCounts@hudextra.c, func_803936A8 = animBattlePoints@hudextra.c, func_803914B4 =
  _AddBattlePoint@hudextra.c (already matched; bodies agree with MAT3 line for line).
- hudextra data: D_80395ED4 = blit, D_80395C00 = weaponIcon[4], D_80395CD0 = powerupIcon[4], D_80395DA0 =
  battleCoin[4], D_803950C0 = stuntCoin[4][10] (0x48 records), D_80395E70 = battlePoint[4], D_80395ED0 =
  battlePointCnt, D_80395ED8 = bonuscnt[4], D_80395060 = healthPal[4], D_80394280 = healthColor[4][2][4],
  D_80394150 = healthPos, D_80393F50 = stuntPos, D_80393FD0 = stuntCoinPos, D_803940D0 = fragPos,
  D_80394250 = playerPaneColors, D_80394270 = playerColor, D_80393B74 = whichCam, D_80393B40 =
  stuntCoinTexNames[10], D_80393B68 = powerupIconNames[3], D_803942A0/A4 = stunt/weapon coinColor initializers,
  D_803942A8 = N64 battle coin model names {"BCOIN_BLUEG1",...}, D_80393DE8 = BattleMultiBlit, D_80393B84 =
  StuntMultiBlit, D_80115F28 = wrongWayPos.
- Game-side callees (historical name = MAT3 name): render_helper = HAL_SetTextDepth, object_create = HAL_SetFont,
  func_800B669C = TB_SetJust, func_800ED66C = HAL_SetTextTranslucency, fcvt_wrapper = TB_Sprintf,
  dispatch_handler = TB_SetTextColor, sound_pitch_diff_halved = TB_CenterText, state_utility = TB_DrawText,
  object_bytes_sum_global = TB_GetFontLineHeight, object_manager_update = HAL_GetTextWidth, sound_control =
  NewMultiBlit, sound_stop = RemoveBlit, sound_call_minimal = ZOID_RemoveObject, ambient_sound_set = InitPane,
  unlock_notification = ChangePaneColor, input_new_data_wrapper = Hidden, Input_ApplyPadConfig = UpdateBlit,
  pad_config = gBlitInstanceList, string_copy_format = MBOX_FindObject_WORK, func_8008E398 = MBOX_NewObject,
  model_data_load = MBOX_HideObject, model_transform_setup = MBOX_ShowObject, func_8008B32C = ScaleUV,
  func_80090E9C = YawUV, func_80090F44 = PitchUV, car_lights_render = ZOID_WindowTo3D, func_8008E06C =
  ZOID_SetColor, func_80090770 = ZOID_SetObjectDef, func_8008B2E4 = Random, countdown_state = gText,
  D_8014A108 (active_player_count) = gNumPlayers, D_8014A110 (gameplay_mode) = gRace, D_801543CC = gModelTime,
  D_80152038 = gStuntPlayer, D_8012E700 = ObjInstanceList, D_8012E67C = gCarTeam, D_801170FC = gPauseState.

## Notes

### func_80392894 = animStuntPoints (MATCH)
`score.py fn cloud/matches/ovl_b/func_80392894.c func_80392894 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
```
func_80392894:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xf0, .rodata+0x0 at +0xf4)
    not verified: .rodata+0x0 at +0xf4: no retail data available
```
(own literal 2.69f = 0x402C28F6, retail word at 0x80394F50 checked by hand.)
Built on Astra's #339 packet (2/468). Closing levers, in order found:
1. as1 trace (`as1t.sh`): the last 2 words were an as1 tie (aftercycles equal) between `sw t8,112(s4)` (line N)
   and `li v0,-2` (line N+1). Adding `.noalias $5,$20` (bonuscnt pointer vs p) to the ugen listing and
   reassembling gave strict 0 -> the store pair needs to be known non-aliasing.
2. Source lever (w13b rule: noalias depends on the access's ucode root): load the showBonus copy through
   `gStuntPlayer[i].showBonus` (islda root) instead of `p->showBonus`. Astra's 2/468 -> MATCH.
3. Moving to MAT3 names/shape broke two schedules again until the clear loop body and the
   decrement/sentinel pair were put on separate source lines: **as1 ties break by source line**, so
   one-line `{ a; b; }` and `for (...) x = 0;` change the schedule.
Disclosed: unused0/unused1 (frame 184, 176 without; exact residual).

### func_8039244C = RemoveHUDExtra (MATCH)
Direct MAT3 typed port (MAT3 names/order, `int player, j`), first compile:
```
func_8039244C:
  MATCH
```
Supersedes Astra's #367 research (4/95; its hand-built pointer walk was the wrong shape: the plain
indexed `for (player...)` loop gives retail's pointer-compare loop end by itself).

### func_803925D0 = ShowHUDExtra (MATCH)
```
func_803925D0:
  MATCH
```
MAT3 typed port with ClearBattlePoints inlined. Two levers: (1) `w/2`, `h/2` in the InitPane arguments instead
of MAT3's `w /= 2; h /= 2;` (retail halves after the player-count guard: loop-invariant hoist); (2) define the
static ClearBattlePoints AFTER ShowHUDExtra (original file order). Its inlined stores keep the callee's source
lines and as1 breaks ties by line. The object begins with the deleted ClearBattlePoints stub (no symbol), which is
retail func_803925C8 (8 bytes, unlocked; not claimed).

### func_80392FE4 = animHealth (MATCH)
```
func_80392FE4:
  MATCH (8 section-relative relocations unverified: .rodata+0x0 at +0x11c, .rodata+0x0 at +0x120, .rodata+0x4 at +0x190, .rodata+0x4 at +0x194, .rodata+0x8 at +0x308, .rodata+0x8 at +0x30c, .rodata+0xc at +0x34c, .rodata+0xc at +0x350)
```
MAT3 port reached Astra's 14/333 (the > 0.8 arm's healthColor/224/112 setup stayed after `bc1f`; retail hoists
it into the compare block). Listing oracle (`as1t.sh` + `asmo.sh`): deleting the `.alias $2,$sp` that ugen put
between the frame arm's `b` and the next label gave strict 0 (w12b's xbb-weight mechanism). Source fix = MAT3's
own `{ anon_palette_texture_20 *inst = &gBlitInstanceList[blt->blidx]; inst->flags |= 1; }` (pointer-variable root,
no noalias region). Also `(animID & 0xf0) >> 4` (N64 decode order).

### func_80391B00 = UpdateHUDExtraPostCamera (in progress)
Identity: the game calls it; it calls func_80391864 (= animWeaponIcons) and func_80391650 (= animPowerupIcons),
then inlines animBattleCoins (gRace == 6) and animStuntCoins (gRace == 4). Not animStuntCoins alone (the
correlation lead): the coin code is the inlined static. N64 coin creation differs from GC: battle coins pick
"BCOIN_<team>G1" from a local name table (D_803942A8), stunt coins sprintf "%sG1" and either re-model an existing
object (func_80090770) or create it; no *_Coin positions, no texture calls; the battle coinColor is set but unused.
Group g6 (supersedes runtime_b_hud_scenes_20261006 in place: members 80391650/80391864 unchanged and MATCH).
Frame 336 matched only with: battle `tex, texIdx, char name[32]` and stunt `e, tex, texIdx` unused (MAT3 names
except the battle name buffer) -- frame residual, to be disclosed if it lands.
Residual: register colouring in the stunt loop: retail gives constant 2 -> s8, 11 -> t0 (split), p in memory;
ours 11 -> s8, 2 split, p in a0.

### func_8038A408 = newWorldDamage (MATCH)
```
func_8038A408:
  MATCH
```
w35e's 4-word residual was an as1 equal-priority tie between the three last-edge delta stores (line N) and the
cross-block-hoisted ring address `la D_80399A70` (also line N with a one-line EDGE_CHECK macro). as1 breaks ties by
source line, so writing the four checks out with a multi-line `if (len > 200.0f) {\n return;\n }` gives the `la`
a later line than the stores and retail's order. The 10 unused words (frame 248) remain as w35e disclosed them;
I found no natural owner (one deleted stub only, so no second inlined static; the top of the frame is fixed by
newpos/uvs/xyz/delta at 236/200/152/140, so any 10 words declared after delta fit).

### func_80390F60 = UpdateBattlePowerUps (group MATCH)
Group `groups/dot_runtime_b_resource_callbacks_20261012/` supersedes the installed group of that name in place
(members B00, B08, B10, D38 + F60; claims [F60]; keep [F60]; context []). `score.py group` without --claims: all
five MATCH; with --claims: `func_80390F60: MATCH (2 section-relative relocations unverified: .data+0x0 at +0x20,
.data+0x0 at +0x24)` (the invsColor initializer, retail D_803942B8 = 10 20 40 60 70 90 B0 B0, checked).
B10/D38 source untouched; B00/B08 (8-byte stubs) source rewritten as the MAT3 bodies that F60 inlines.

#### 80391B00 progress (wave 36, continued)
- `d -= gNumWindows == 1 ? 18 : gNumWindows == 2 ? 8 : 8;` (MAT3's literal text, `?8:8` included) is load-bearing:
  the compiled-out `== 2` test inside the prev loop raises constant 2's priority (tot 200 -> 1200), so 2 takes s8 and
  11 is split into t0 pieces exactly as retail (`li t0,11` after each call). Without it 11 takes s8.
- reset loop through `coin = &stuntCoin[i][j]` (retail's `move s3,s0` copy and j*72 IV).
- Battle part (first 158 words) and all frame offsets match; the stunt loop differs only by allocation.
- Remaining cause (traced with the w16d split-instrumented uopt, bin/uopt_spl, tools/spl.sh): the stunt player
  pointer `p` (PRE'd ixa(gStuntPlayer, i*120), inserted at bb25) is split; its first piece grows from bb25 over the
  alpha computation into the `p->show` test and the else branch and is coloured a0 (save 3.5). Retail never colours
  p (every use reloads the 144(sp) temp). Oracles: forcing p's web split (`force.sh p1:w104=s`) or rejecting bb31
  or bb87/93 from that piece (`SPL_REJ=31 SPL_SYM=104`) gives 12 non-equal words; the rest is then the (u8)alpha
  temp home 136 vs 140, one extra `sw v1,124(sp)` before the counts test, and `sb t9,211(sp)` vs `sb t9,3(a1)`.
  addadjacents adds a block when shared < old && 2*regsleft >= numintf + shared (uopt.c L46e2e0).
- Tried without effect: islda roots for p's fields, alpha/if shapes, `p` block-local, K&R statics, early-exit forms,
  colour-pointer spellings. `for (..; i++, p++)` (p a real variable) gives the retail reload pattern but p then
  lives in its own home (224) instead of retail's temp 144 (36 words).

#### 80391B00: ghost-interference finding (late wave 36)
- Retail's split decisions need ONE more interfering int web across the stunt loop than the MAT3 port has (a web
  with no code: it only raises numintf in addadjacents). Oracle: a compiled-out read of an uninitialised local
  (`if (e) {}`, e = MAT3's unused `e`) placed in the coin update, the reset loop or the i-loop top (not the j-loop
  top, not after the loop) drops strict 407 -> **83/595** (`func_80391B00/best_holder/`, frame 336): p becomes a pure
  memory web exactly as retail. A second ghost changes nothing.
- With that, forcing the counts pointer web (`ixa(p, j*2)`, w297) to v1 and its reset-loop piece (w377) to split
  leaves 6 non-equal words: the (u8)alpha spill temp (136 vs 140; retail has one more temp-holding expression web
  before it in itable order, conflicting with the alpha region) and one spill-store placement of the counts pointer.
  So retail's ghost is probably an EXPRESSION web (it takes a spill temp), not a variable.
- MAT3's dead `if (gNumWindows==1) e=10; ... ==4` lines in the coin update reproduce the interference too
  (strict 139) but add two spill temps (frame 344): not retail's form. `e -= ...` in the prev loop adds code.
- Not delivered: still NONMATCH, and the holder is an artificial read; next step is finding the natural expression
  that is live (with no code) across the coin update / reset loop / loop top.
