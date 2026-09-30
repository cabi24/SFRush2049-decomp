# track_render_process (0x800A377C), scout report

Feasibility: **LOW-MEDIUM** (needs a small IPA group plus a lot of struct work). Effort: 1-2 days.

## Facts
- 800 words, `asm/us/blob/blob_8009dd18.s`. Not spliced. **The name is wrong**: it is a Controller Pak / rumble channel manager (`osPfsInitPak`, `osPfsFreeBlocks`, `osPfsDeleteFile`, `osMotorStart/Stop`, `osCreateMesgQueue` x6, `osRecvMesg` x6, `osJamMesg` x10, `memcpy` x6, four `jalr t9` calls through a function pointer at `D_80144008`). It loops over channels (per-channel struct at `D_80144030`, stride 0x304 = 772 bytes; `memset(ch, 0, 772)` is in `func_800A3724`).
- Frame 432, saves `ra`+`s0..s8`, `a0`,`a1` stored to home slots. Prologue and the caller (`func_800C813C`) show it is called with no argument setup at all (a0/a1 are junk): effectively `void f(void)`.
- **IPA-dependent, mildly.** Callees `func_800A3640` (51 words) and `func_800A3724` (22 words) write `s0,s2..s7` and `s0,s1` without saving them, and the caller re-materialises `s2,s3,s5,s6,s7` (`lui`/`addiu`) after every such call. `func_800A3724` also has a second caller (`car_lod_select`). Other callees (`func_800A1E94`, `func_800A1910`, `func_800A1644` (179 words), `track_collision_setup`, `no_catchup`, `func_8008A704`, `func_8009211C`, libultra) show no IPA signals. Minimal group: `track_render_process`, `func_800A3640`, `func_800A3724`, with stand-in callers (`car_lod_select`, `func_800C813C`), everything else `context`.
- 19 distinct callees, 17 distinct globals, 11 backward branches (many small loops), zero FP, zero multiplies. Repetitive state-machine shape (per-channel status bytes at `ch+1..+11`), which is friendly to IDO once the struct is right.

## First pass
`seeds/track_render_process.c` (m2c plus repairs: `sp130..sp138` merged into a `s8 sp130[16]` buffer): compiles at `-O2`, 772 words vs 800, **aligned shape 77%, aligned exact 10%** with no hand work. That is a much better raw seed than the other big ones. It ignores IPA (score at `-O2` not `-O3`), so the exact figure will change under the group build.

## Approach
1. Define `PakChannel` (0x304 bytes) from the m2c offsets; name the per-channel flags. 2. Build a group dir (`group.json` files/keep) and use `zbuild.py`. 3. Hand-write the 3 IPA members first (they are small), then the body.

## Risks
`sp130` buffer layout (`OSPfs` at 0x138 in the frame) and the repeated `lui/addiu` s-reg rematerialisation is IPA-generated and cannot be forced from C. Unknown `D_80144008` handler table types. Arcade source gives no help (N64-specific).
