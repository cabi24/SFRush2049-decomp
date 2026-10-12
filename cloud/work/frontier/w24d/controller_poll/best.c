/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * controller_poll (0x800C95DC, 928 B) -- w24d. EQUAL in the whole-program unit (body 232/232, frame 88/88); the two
 * remaining display rows are the own-.rodata 0.2f relocation (0x80123F94 = 0x3E4CCCCD). Body only: build with
 * `mk.sh best.c OUT.c` (prelude = src/blob/groups/controller_poll/group.c up to its draft, D_80149B64/D_80149B74 extern).
 * Read the pads once per frame: stick axes through func_800C9590, held/pressed masks, per-button auto-repeat.
 * Levers (each traced, see w24d/RESULTS.md):
 *   - The pad state written by the controller thread is read through volatile views (gStickRaw, gStickCal,
 *     gPadHeld = D_80156978). Volatile loads keep their emission order in as1 (stick raw before calibration, the
 *     `sw/lw` re-read of the held mask); `raw` is read before each call so the raw load is emitted first.
 *   - `if (b == 0) continue;` in the button loop (was `if (b != 0) { ... }`): the continue makes 32 (the loop bound)
 *     win the tied priority over &D_8002AFB4 (t2/t3 as retail) and gives `connected` retail's t4 without w23e's
 *     compiled-out `if (connected) {}` holder.
 * No shaping devices, no unused locals.
 */
typedef struct { u8 pad[636]; volatile s32 tick; } SysBlk;
#define gPadHeld ((volatile u32 *) D_80156978)
#undef gStickRaw
#undef gStickCal
#define gStickRaw ((volatile s8 (*)[2]) &D_80149B50)
#define gStickCal ((volatile s8 (*)[2]) &D_80149AF8)
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
        osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
        return;
    }
    for (i = 0; i < 4; i++) {
        raw = gStickRaw[i][0];
        x = func_800C9590(1.0f, gStickCal[i][0], raw);
        raw = gStickRaw[i][1];
        y = func_800C9590(1.0f, gStickCal[i][1], raw);
        gStick[i].unk0 = x;
        gPadHeld[i] = gHeld[i];
        held = gPadHeld[i];
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
            if (b == 0) {
                continue;
            }
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
        if (connected) {
            D_80149784 |= D_80143A00[i];
        }
    }
    osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
}

