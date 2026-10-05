# particle_system unit (component 13) — structure (w5b, 2026-10-05)

Names are historical labels. Nothing here is a match; the one complete body (`render_display_list`) is a
draft proven only in a stand-in group (`group_standin/`).

## Why none of these can close alone

All four assigned functions sit in one interprocedural knot, "component 13" of the frontier tool:

```
func_8009F058 (5,228 B, unassigned, saves no s-regs)      ──┬──> render_display_list (IPA params s0,s1,s3,s4,s5)
   └──> track_collision_wall (2,712 B, recursive)            │
          └──> particle_system (2,900 B, IPA params s7,t0) ──┘──> func_80099B30 (provisional)
                   └──> func_8009C3F8 (452 B, IPA param f16) <── camera_follow_path, func_800BFD8C,
                                                                 camera_update_c, select_screen_update (wrappers)
```

* `render_display_list` has two callers (`particle_system` ×2, `func_8009F058` ×1). Its parameter registers and
  the registers it may clobber without saving (it saves only `ra`, writes `s2`) are decided by IPA over both
  callers. A real proof needs `particle_system` **and** `func_8009F058`.
* `particle_system` receives `s7` (object) and `t0` (view index) from `track_collision_wall`, which is itself
  called by `func_8009F058`.
* `func_8009C3F8` receives its float in `f16`; the 9-word wrappers `camera_update_c`/`select_screen_update` copy
  `f12`→`f16` (A21 got 92/113 without the real callers). Its other direct callers are `particle_system`,
  `camera_follow_path` (unit, s0 param) and `func_800BFD8C`, all unmatched.

So the smallest real group is {func_8009F058, track_collision_wall, particle_system, render_display_list,
func_80099B30} plus, for `func_8009C3F8`, the camera wrappers and `camera_follow_path`/`func_800BFD8C`.

## render_display_list (0x80099BFC, 2,559 words) — texture/display-list loader, complete draft

Sibling of `object_render` (w2i): SDK GBI macros on a local `Gfx *gfx` whose address is taken (passed to
`func_80099B30`), so every macro does `lw/addiu/sw 596(sp)`; each macro's block-scoped `_g` owns a frame slot
(frame 608, only `ra` saved).

```c
void render_display_list(Texture *tex /*s1*/, TexRect *rect /*s0*/, TexRect *clip /*s3*/,
                         Gfx **gp /*s4*/, Palette *palettes /*s5*/);
```
(parameter order is a guess; the stand-in group reproduces the retail register assignment s0,s1,s3,s4,s5.)

| Retail offset | What |
|---|---|
| +0x000 | `gfx = *gp; if (tex == NULL) return;` (no store back) |
| +0x014 | `if (!(tex->flags & 0x08000000)) gSPDisplayList(gfx++, tex->image);` → tail |
| +0x040 | masks/maskt: rect ? `func_80087804(rect->x0 - rect->x1) - 5` (u16) : `func_80087804(tex->width/height)` |
| +0x0A0 | `if (flags & 0x10000) {cms = CLAMP; masks = 0;} else cms = WRAP;` `cms = (flags & 0x40000) ? cms|MIRROR : cms;` same for T with 0x20000/0x80000. The `else` arm and the `?:` are what give retail's `b` to the next instruction and the `andi 0xffff` in both arms |
| +0x108 | `fmt = tex->fmt (u8 @21)`, `siz = tex->siz (u8 @20)`; TLUT cache `D_8017A638` (0 none, 1 RGBA16) |
| +0x11C | RGBA: TLUT none; 16b tile +0x154 / block +0x36C; 32b tile +0x564 / block +0x778 |
| +0x96C | CI: TLUT RGBA16 (`E3001001 / 0x8000`); 8b tile +0x9B8 / block +0xBC8; 4b tile +0xDC0 / block +0xFE8 |
| +0x11E4 | I (fmt 4): TLUT none; 8b tile +0x1228 / block +0x1438; 4b tile +0x1630 / block +0x1858 |
| +0x1A54 | IA (fmt 3): TLUT none; 16b +0x1A98/+0x1CB0; 8b +0x1EB0/+0x20C0; 4b +0x22B8/+0x24E0 |
| +0x26D8 | `if (clip) {gfx--; gDPSetTileSize(gfx++, 0, clip->x0>>3, …);} else if (rect) {same with rect}` |
| +0x27A8 | `if (tex->palette >= 0 && !(flags & 0x20000000) && palettes) func_80099B30(&gfx, &palettes[tex->palette]);` then `*gp = gfx` |

Tile loads are SDK `gDPLoadTextureTile`/`_4b` with `uls = rect->x0 >> 5` etc. (rects are u16 10.5 fixed point),
`pal 0`, shifts 0. Block loads are the **standard** SDK `gDPLoadTextureBlock`/`_4b` (SetTileSize from 0,0 —
unlike `object_render`'s "At" variant).

Types: `Texture` {u16 width @16, u16 height @18, u8 siz @20, u8 fmt @21, s16 palette @22, void *image @24,
u32 flags @28}; `TexRect` {u16 x0,y0,x1,y1}; `Palette` (24 bytes) {u8 first @16, u8 last @17, s32 table @20}.
Flags are tested with masks (`& 0x10000` → retail `sll; bgez`); a bitfield struct loads halfwords and is wrong.

State of the draft (`group_standin/`, stand-in callers zz_a/zz_b): mnemonics identical except one effect —
uopt promotes the `0xF2000000` (G_SETTILESIZE) constant into `s2` after `masks` dies and reuses it in the tail;
retail materialises `lui at,0xf200` at each use. That costs one word (2,558 vs 2,559), moves `li t3,2047` /
`lui ra,0xf5xx` by one slot in each block section, and renumbers the temps. Likely an IPA context effect (the
real callers constrain which callee-saved registers the callee may use); re-test inside the real group before
changing source.

## particle_system (0x8009C8F0, 725 words) — object/billboard draw with LOD (scouted, not written)

Inputs: `s7` = object (u32 flags @0, mode/LOD table index u16 @20 (`>>10` table, `&0x3FF` entry ×44 bytes),
other object ptr @8, f32 scale @12, rect ptrs @48/@52, palette @56, colours @60..67), `t0` = view index,
`a1`,`a2`,`a3` = camera object / count / flag, 5th arg (stack) = `Gfx **` (local copy at 228(sp), passed to
`render_display_list` as `s4 = sp+228`).

| Retail | What |
|---|---|
| 0x8009C8F0 | LOD table entry `a0`; `lod = entry->count(@22) - 1`; distance cached in `D_80124EE8` (f32): from camera position delta (`sqrt(x²+y²+z²)`) unless the object's per-view bit (`0x100 << t0`) is set |
| 0x8009CB10 | LOD pick: forced (`flags & 4`: `flags & 3`), else walk down while `entry[lod].dist * scale > distance` |
| 0x8009CC78 | `camera_update_d`, geometry-mode / texture / other-mode commands (`D9`, `D7`, `E3`, `E7`) |
| 0x8009CDD4 | billboard orientation: `func_8008E0B8` (normalise), `func_8008C768`, `func_8009C3F8` (asin/acos) |
| 0x8009CF88 | palette `func_80099B30`, then `render_display_list` twice (with/without clip rect @52) |
| 0x8009D060 | tile-size rewrite from rects (`F2`), prim/env colour (`FA`/`FB` from bytes @60..67), `gSPDisplayList` (`DE`) |
| 0x8009D34C | second `func_80099B30` (clears `D_8017A4B0` first), state restore (`E7`, `E3`, `D9`, `D7`), `sw zero,52(s7)` |

## track_collision_wall (0x8009DD88, 678 words) — recursive scene-graph walk (scouted)

Saves s0–s8, f20; `a2` = node (`s7`), `a3` = view (`s2`, passed to particle_system in `t0`). Per node:
visibility via `track_collision_edge`, three `func_8009D99C` clip tests, `func_8009D708`/`func_8009D45C`,
`particle_system` for drawable nodes (s7/t0 set up at 0x8009E5DC..0x8009E60C), recursion into children
(index @22) at 0x8009E6CC. Constants kept in s-regs: `s0 = &D_8012E700`, `s3 = 68`, `s4 = 0x40000`,
`s6 = 0x800000`, `s5 = &D_80124FE0`, `s8 = &D_80124FC4`.

## func_8009C3F8 (0x8009C3F8, 113 words) — Cody–Waite asin/acos kernel (scouted; A21 source exists)

`f16` = x, `a0` = 0 asin / 1 acos. |x| < 2.3e-10 → x; |x| ≥ 1 → π/2; |x| > 0.5 → g = ((0.5−|x|)+0.5)·0.5,
y = −2√g, i = 1−a0; else g = x². R = g·P(g)/Q(g) (coefficients at 0x80123AC4..0x80123AE8), result
y + y·R, then the `a[i]`/`b[i]` tables at 0x8011F010/0x8011F018 (0, π/4 / π/2, π/4). The A21 source
(`cloud/work/ipa-groups/codex_trig_roots_a21/group.c`) is semantically right; it needs the real callers for
the `f16` parameter.

## Next steps

1. Write `particle_system` and `track_collision_wall` bodies from the tables above; build the group
   {func_8009F058 stand-in or real, track_collision_wall, particle_system, render_display_list, func_80099B30}.
2. Re-score `render_display_list` there; only then chase the `0xF2000000` promotion.
3. `func_8009C3F8`: add `particle_system` and `camera_follow_path` to A21's group.
