/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * controller_poll -- NEAR-MISS (w23e, from w22f best.c), not a match. Unit: words 11 ops 2 norm 2, frame 88/88, body
 * 232/232 (w22f: 92/30, 230 words). Body only: build with `mk.sh best.c OUT.c` (prelude = lines 1..3720 of
 * src/blob/groups/controller_poll/group.c, D_80149B64/D_80149B74 made extern).
 * Changes vs w22f (each traced, see w23e/RESULTS.md):
 *   - `0.2f` literal instead of `repeat = D_80123F94` (0x80123F94 = 0x3E4CCCCD is this function's own .rodata):
 *     1.0f/$f24 then 0.2f/$f20 hoisted in retail's order.
 *   - `raw = gStickRaw[i][0];` before the first call: the raw-array IV is numbered before the cal-array IV, so
 *     raw/cal get s3/s4 as retail.
 *   - `gRepeatTime[i][b] = gTick;` before `D_80143A00[i] |= mask;` (ugen temp ring in that block).
 * Shaping devices (disclosed; still being replaced):
 *   - `if (connected) {}` after the button loop: raises `connected` (tot 30 -> 40, save 6.0 -> 8.0) above the
 *     loop-pointer webs (5.86-6.83), so it takes t4 and the gStick pointer an s-register (92 -> 33 words alone).
 *   - `held = *(volatile u32 *) &D_80156978[i];`: retail stores gHeld[i] to D_80156978[i] and re-reads it
 *     (`sw t9,0(s1); lw t0,0(s1)`); a plain read is forwarded by uopt.
 * Left (11 words): t2/t3 swap of the 32 loop bound and &D_8002AFB4 (tied priority, lower web number wins), second
 * call's raw/cal load order (as1 aftercycles; retail has raw first), 2 own-rodata relocation display rows.
 */
typedef struct { u8 pad[636]; volatile s32 tick; } SysBlk;
#define gTick (((SysBlk *) &D_8002E8E8)->tick)

/* read the pads: sticks, held/pressed masks and auto-repeat per button */
void controller_poll(void)
{
    s32 i;
    s32 b;
    s32 raw;
    u32 held;
    u32 mask;
    s32 connected;
    f32 x;
    f32 y;

    osRecvMesg((OSMesgQueue *) &D_801497A8, NULL, 1);
    if (gResetTick != 0) {
        gResetTick = 0;
        D_80111958 = gTick;
    }
    D_8015694C = D_80149784 = D_80156944 = 0;
    if (gSkipFrames != 0) {
        gSkipFrames--;
        for (i = 0; i < 4; i++) {
            gPrevPressed[i] = 0;
            gStick[i].unk4 = 0.0f;
            gStick[i].unk0 = 0.0f;
            D_80143A00[i] = 0;
            D_80156978[i] = 0;
            D_80156998[i] = 0;
        }
        osJamMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
        return;
    }
    for (i = 0; i < 4; i++) {
        raw = gStickRaw[i][0];
        x = func_800C9590(1.0f, gStickCal[i][0], raw);
        y = func_800C9590(1.0f, gStickCal[i][1], gStickRaw[i][1]);
        gStick[i].unk0 = x;
        D_80156978[i] = gHeld[i];
        held = *(volatile u32 *) &D_80156978[i];
        gStick[i].unk4 = y;
        D_80156998[i] = gPrevPressed[i];
        gPrevPressed[i] = 0;
        D_80143A00[i] = 0;
        if (held != 0) {
            D_80111958 = gTick;
        }
        connected = gConnected[i];
        if (connected) {
            D_8015694C |= D_80156998[i];
            D_80156944 |= held;
        }
        for (b = 0; b < 32; b++) {
            if (b != 0) {
                mask = 1 << b;
                if (held & mask) {
                    if (gRepeatTime[i][b] == 0 ||
                        (u32) (s32) (0.2f * D_8002AFB4) < gTick - gRepeatTime[i][b]) {
                        gRepeatTime[i][b] = gTick;
                        D_80143A00[i] |= mask;
                    }
                } else {
                    gRepeatTime[i][b] = 0;
                }
            }
        }
        if (connected) {}
        if (connected) {
            D_80149784 |= D_80143A00[i];
        }
    }
    osJamMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
}

