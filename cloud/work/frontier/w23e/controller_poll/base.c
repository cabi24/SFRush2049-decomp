/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * controller_poll -- NEAR-MISS (w22f, from w1g best.c), not a match. Unit: words 92 ops 30, frame 88/88 (w1g: 120/30).
 * Change vs w1g: the chained zero store `D_8015694C = D_80149784 = D_80156944 = 0;` (audio_occlusion's idiom, w8b):
 * retail keeps &D_80156944/&D_80149784 in s7/s8 and stores D_8015694C through `lui at`.
 * Body only: build the scored file with `mk.sh best.c OUT.c` (prelude = lines 1..3720 of
 * src/blob/groups/controller_poll/group.c with the tentative D_80149B64/D_80149B74 made extern; the locked
 * group.c defines them in the unit). See w22f/RESULTS.md (q1: 81 words with a laundered loop access).
 */
typedef struct { u8 pad[636]; volatile s32 tick; } SysBlk;
#define gTick (((SysBlk *) &D_8002E8E8)->tick)

/* read the pads: sticks, held/pressed masks and auto-repeat per button */
void controller_poll(void)
{
    s32 i;
    s32 b;
    u32 held;
    u32 mask;
    s32 connected;
    f32 x;
    f32 y;
    f32 repeat;

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
    repeat = D_80123F94;
    for (i = 0; i < 4; i++) {
        x = func_800C9590(1.0f, gStickCal[i][0], gStickRaw[i][0]);
        y = func_800C9590(1.0f, gStickCal[i][1], gStickRaw[i][1]);
        gStick[i].unk0 = x;
        D_80156978[i] = gHeld[i];
        held = ((u32 *) D_80156978)[i];
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
                        (u32) (s32) (repeat * D_8002AFB4) < gTick - gRepeatTime[i][b]) {
                        D_80143A00[i] |= mask;
                        gRepeatTime[i][b] = gTick;
                    }
                } else {
                    gRepeatTime[i][b] = 0;
                }
            }
        }
        if (connected) {
            D_80149784 |= D_80143A00[i];
        }
    }
    osSendMesg((OSMesgQueue *) &D_801497A8, NULL, 0);
}

