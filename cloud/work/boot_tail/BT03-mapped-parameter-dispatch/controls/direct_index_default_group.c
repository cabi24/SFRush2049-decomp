/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native fixed-point pause-volume ramp. Family lead: AxioDL/musyx synth.c,
 * synthPauseVolume, revision 78d2e16e4905fc675952162d331c24d5198b2687,
 * CC0-1.0. The newer floating-point representation is not imported. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef struct MasterFader {
    u8 normalFader[20];
    u8 type;
    u8 reserved[3];
    u32 pauseVolume;
    u32 pauseTarget;
    u32 pauseDelta;
    u32 pauseTime;
} MasterFader;
extern MasterFader D_8004F300[32];
extern void func_8001E930(u32 *);

void func_8001BE14(u8 volume, u16 time, u8 group)
{
    s32 i;
    u32 ltime;
    u8 type;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (group) {
    case 255:
        for (i = 0; i < 32; ++i) {
            if (D_8004F300[i].type == 0 || D_8004F300[i].type == 1) {
                D_8004F300[i].pauseTarget = volume << 16;
                D_8004F300[i].pauseDelta = (s32)(D_8004F300[i].pauseTarget - D_8004F300[i].pauseVolume) / time;
                D_8004F300[i].pauseTime = ltime;
            }
        }
        return;
    case 252:
        for (i = 0; i < 32; ++i) {
            if (D_8004F300[i].type == 2 || D_8004F300[i].type == 3) {
                D_8004F300[i].pauseTarget = volume << 16;
                D_8004F300[i].pauseDelta = (s32)(D_8004F300[i].pauseTarget - D_8004F300[i].pauseVolume) / time;
                D_8004F300[i].pauseTime = ltime;
            }
        }
        return;
    case 250:
        type = 2;
        goto setup_type;
    case 251:
        type = 3;
        goto setup_type;
    case 253:
        type = 0;
        goto setup_type;
    case 254:
        type = 1;
        goto setup_type;
    setup_type:
        for (i = 0; i < 32; ++i) {
            if (D_8004F300[i].type == type) {
                D_8004F300[i].pauseTarget = volume << 16;
                D_8004F300[i].pauseDelta = (s32)(D_8004F300[i].pauseTarget - D_8004F300[i].pauseVolume) / time;
                D_8004F300[i].pauseTime = ltime;
            }
        }
        return;
    default:
        D_8004F300[group].pauseTarget = volume << 16;
        D_8004F300[group].pauseDelta = (s32)(D_8004F300[group].pauseTarget - D_8004F300[group].pauseVolume) / time;
        D_8004F300[group].pauseTime = ltime;
        return;
    }
}
