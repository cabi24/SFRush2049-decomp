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
    s32 pauseVolume;
    s32 pauseTarget;
    s32 pauseDelta;
    u32 pauseTime;
} MasterFader;
extern MasterFader D_8004F300[32];
extern void func_8001E930(u32 *);

void func_8001BE14(u8 volume, u16 time, u8 group)
{
    u32 ltime;
    u8 type;
    MasterFader *fader;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (group) {
    case 255:
        for (fader = D_8004F300; fader != D_8004F300 + 32; ++fader) {
            if (fader->type == 0 || fader->type == 1) {
                fader->pauseTarget = volume << 16;
                fader->pauseDelta = (fader->pauseTarget - fader->pauseVolume) / time;
                fader->pauseTime = ltime;
            }
        }
        return;
    case 252:
        for (fader = D_8004F300; fader != D_8004F300 + 32; ++fader) {
            if (fader->type == 2 || fader->type == 3) {
                fader->pauseTarget = volume << 16;
                fader->pauseDelta = (fader->pauseTarget - fader->pauseVolume) / time;
                fader->pauseTime = ltime;
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
        for (fader = D_8004F300; fader != D_8004F300 + 32; ++fader) {
            if (fader->type == type) {
                fader->pauseTarget = volume << 16;
                fader->pauseDelta = (fader->pauseTarget - fader->pauseVolume) / time;
                fader->pauseTime = ltime;
            }
        }
        return;
    default:
        fader = &D_8004F300[group];
        fader->pauseTarget = volume << 16;
        fader->pauseDelta = (fader->pauseTarget - fader->pauseVolume) / time;
        fader->pauseTime = ltime;
        return;
    }
}
