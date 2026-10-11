/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: synth volume-fader setup; MusyX synthVolume source-family lead.
 * AxioDL/musyx 78d2e16e4905fc675952162d331c24d5198b2687, CC0-1.0.
 * Native N64 fixed-point layout and semantics, not the later float fader. */
/* w20c: D_8004F300 declared as lib_1a660 declares it (ChannelState, verbatim TU lines; the TU
 * must hold them above the func_8001B9F8 slot, tu_patch/lib_1a660.c.diff) and read through the
 * MasterFader view (one typedef shared verbatim by func_8001B9F8/func_8001BE14). Type views only. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern ChannelState D_8004F300[32];
typedef struct MasterFader { u32 volume; u32 target; s32 delta; u32 time; u32 seqId; u8 type; u8 seqMode; u8 unknown16[2]; u32 pauseVolume; u32 pauseTarget; s32 pauseDelta; u32 pauseTime; } MasterFader;
extern void func_8001E930(u32 *);

void func_8001B9F8(u8 volume, u16 time, u8 vGroup, u8 seqMode, u32 seqId)
{
    u32 ltime;
    u32 i;
    u8 type;

    if (time == 0) {
        ++time;
    }
    ltime = time;
    func_8001E930(&ltime);
    switch (vGroup) {
    case 255:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 0 || ((MasterFader *)D_8004F300)[i].type == 1) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 252:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == 2 || ((MasterFader *)D_8004F300)[i].type == 3) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    case 250: type = 2; goto setup_type;
    case 251: type = 3; goto setup_type;
    case 253: type = 0; goto setup_type;
    case 254: type = 1; goto setup_type;
    setup_type:
        for (i = 0; i < 32; ++i) {
            if (((MasterFader *)D_8004F300)[i].type == type) {
                ((MasterFader *)D_8004F300)[i].target = volume << 16;
                ((MasterFader *)D_8004F300)[i].time = ltime;
                ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
                ((MasterFader *)D_8004F300)[i].seqId = 0xFFFFFFFFU;
            }
        }
        return;
    default:
        i = vGroup;
        ((MasterFader *)D_8004F300)[i].target = volume << 16;
        ((MasterFader *)D_8004F300)[i].time = ltime;
        ((MasterFader *)D_8004F300)[i].delta = (s32)(((MasterFader *)D_8004F300)[i].target - ((MasterFader *)D_8004F300)[i].volume) / time;
        ((MasterFader *)D_8004F300)[i].seqMode = seqMode;
        ((MasterFader *)D_8004F300)[i].seqId = seqId;
        return;
    }
}
