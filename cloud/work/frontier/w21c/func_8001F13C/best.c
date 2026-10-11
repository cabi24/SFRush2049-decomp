/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "types.h"
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
#pragma pack(0)
typedef struct ChannelLink_8001EE9C { u8 previous, next; u16 active; } ChannelLink_8001EE9C;
#pragma pack(1)
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[29]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
typedef struct VoiceAlloc_8001F13C { u8 unknown00[0xBA]; u16 allocId; u8 unknownBC; u8 block; u8 unknownBE[226]; } VoiceAlloc_8001F13C;
#pragma pack(0)
typedef struct GroupLink_8001EE9C { u16 next, previous; } GroupLink_8001EE9C;
typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;
extern ChannelLink D_80050440[32];
extern VoiceState D_8004BEB8[];
extern volatile unsigned char D_8004BE94;
extern u8 D_8004FA18, D_8004FA19, D_8004FA1A;
extern u8 D_800504C0, D_800504C1;
extern unsigned char D_800504C2, D_800504C3;
extern ChannelLink_8001EE9C D_800504C8[32];
extern u8 D_80050548[256];
extern GroupLink_8001EE9C D_80050648[256];
extern u16 D_80050A48;
#define ALLOC(i) (((VoiceAlloc_8001F13C *)D_8004BEB8)[i])
int func_8001F13C(u8 priority, u8 maxVoices, u16 allocId, u8 fxFlag)
{
    s32 i;
    s32 num;
    s32 voice;
    u16 p;
    u32 type_alloc;
    ChannelLink *sfv;

    if (!D_8004BE94) {
        if (fxFlag) {
            type_alloc = (D_800504C2 >= D_8004FA1A && D_8004FA18 > D_8004FA1A) ? 1 : 0;
            if (D_8004FA1A <= maxVoices) {
                goto _skip_alloc;
            }
            goto _do_alloc;
        } else {
            type_alloc = (D_800504C3 >= D_8004FA19 && D_8004FA18 > D_8004FA19) ? 1 : 0;
            if (D_8004FA19 <= maxVoices) {
                goto _skip_alloc;
            }
        _do_alloc:
            num = 0;
            voice = -1;
            p = D_80050A48;
            while (p != 0xFFFF && priority >= p && voice == -1) {
                for (i = D_80050548[p]; i != 0xFF; i = D_800504C8[i].next) {
                    if (ALLOC(i).block)
                        continue;
                    if (allocId != ALLOC(i).allocId)
                        continue;
                    ++num;
                    if (!type_alloc || fxFlag == D_8004BEB8[i].external4C) {
                        if ((D_8004BEB8[i].flags24 & 2))
                            continue;
                        if (voice != -1) {
                            if (D_8004BEB8[i].value28 < D_8004BEB8[voice].value28)
                                voice = i;
                        } else
                            voice = i;
                    }
                }
                p = D_80050648[p].next;
            }
        }
        if (num < maxVoices) {
            while (p != 0xFFFF && num < maxVoices) {
                i = D_80050548[p];
                while (i != 0xFF) {
                    if (!ALLOC(i).block) {
                        if (allocId == ALLOC(i).allocId) {
                            num++;
                        }
                    }
                    i = D_800504C8[i].next;
                }
                p = D_80050648[p].next;
            }
            if (num < maxVoices) {
            _skip_alloc:
                voice = -1;
                if (D_800504C0 != 0xFF && type_alloc == 0) {
                    voice = D_800504C0;
                    goto _update;
                }
                if (priority < D_80050A48) {
                    return -1;
                }
                p = D_80050A48;
                while (p != 0xFFFF && priority >= p && voice == -1) {
                    for (i = D_80050548[p]; i != 0xFF; i = D_800504C8[i].next) {
                        if (ALLOC(i).block != 0)
                            continue;
                        if (!type_alloc || fxFlag == D_8004BEB8[i].external4C) {
                            if ((D_8004BEB8[i].flags24 & 2))
                                continue;
                            if (voice != -1) {
                                if (D_8004BEB8[voice].value28 > D_8004BEB8[i].value28)
                                    voice = i;
                            } else
                                voice = i;
                        }
                    }
                    p = D_80050648[p].next;
                }
                if (voice == -1) {
                    return -1;
                }
            _update:
                if (D_8004BEB8[voice].channel2E > priority) {
                    goto _fail;
                }
            }
        }
        if (voice == -1) {
            goto _fail;
        }
        if (D_80050440[voice].flags == 1) {
            sfv = D_80050440 + voice;
            i = sfv->previous;
            if (i != 0xFF) {
                D_80050440[i].next = sfv->next;
            } else {
                D_800504C0 = sfv->next;
            }
            i = sfv->next;
            if (i != 0xFF) {
                D_80050440[i].previous = sfv->previous;
            }
            if (voice == D_800504C1) {
                D_800504C1 = sfv->previous;
            }
            sfv->flags = 0;
        } else if (D_8004BEB8[voice].external4C) {
            D_800504C2--;
        } else {
            D_800504C3--;
        }
        if (fxFlag != 0) {
            ++D_800504C2;
        } else {
            ++D_800504C3;
        }
        return voice;
    }
_fail:
    return -1;
}
