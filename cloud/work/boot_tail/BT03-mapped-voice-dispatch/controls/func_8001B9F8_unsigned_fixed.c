/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Synth volume-fader setup; MusyX synthVolume source-family lead.
 * AxioDL/musyx 78d2e16e4905fc675952162d331c24d5198b2687, CC0-1.0.
 * Native N64 fixed-point layout and semantics, not the later float fader. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MasterFader {
    u32 volume;
    u32 target;
    s32 delta;
    u32 time;
    u32 seqId;
    u8 type;
    u8 seqMode;
    u8 unknown16[18];
} MasterFader;
extern MasterFader D_8004F300[32];
extern void func_8001E930(u32 *);

void func_8001B9F8(u8 volume, u16 time, u8 vGroup, u8 seqMode, u32 seqId)
{
    u32 ltime;
    u32 i;
    u8 type;
    MasterFader *smf;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (vGroup) {
    case 255:
        for (smf = D_8004F300, i = 0; i < 32; ++i, ++smf) {
            if (smf->type == 0 || smf->type == 1) {
                smf->target = volume << 16;
                smf->delta = (s32)(smf->target - smf->volume) / time;
                smf->time = ltime;
                smf->seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 252:
        for (smf = D_8004F300, i = 0; i < 32; ++i, ++smf) {
            if (smf->type == 2 || smf->type == 3) {
                smf->target = volume << 16;
                smf->delta = (s32)(smf->target - smf->volume) / time;
                smf->time = ltime;
                smf->seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 250: type = 2; goto setup_type;
    case 251: type = 3; goto setup_type;
    case 253: type = 0; goto setup_type;
    case 254: type = 1; goto setup_type;
    setup_type:
        for (smf = D_8004F300, i = 0; i < 32; ++i, ++smf) {
            if (smf->type == type) {
                smf->target = volume << 16;
                smf->delta = (s32)(smf->target - smf->volume) / time;
                smf->time = ltime;
                smf->seqId = 0xFFFFFFFFU;
            }
        }
        return;
    default:
        smf = &D_8004F300[vGroup];
        smf->target = volume << 16;
        smf->delta = (s32)(smf->target - smf->volume) / time;
        smf->time = ltime;
        smf->seqMode = seqMode;
        smf->seqId = seqId;
        return;
    }
}
