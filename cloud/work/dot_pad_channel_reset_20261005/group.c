/* flags: -g0 -O3 -mips2 -G 0 -non_shared (IPA group: Input_InitPadHandlers is inlined into Input_ApplyPadConfig) */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {u32 word0,word4;u16 half8,halfA,halfC,padE,half10,half12;u8 tail[12];} Record;
extern Record D_80140BF0[];

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ s8 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 pad24[0x10];
    /* 0x34 */ u16 index;
} Sprite;

void Input_SetAnalogBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void input_status_update(s32 arg0, s32 arg1, s32 arg2, unsigned short arg3);
void Input_SetPadSecondaryFlag(s32 arg0, s32 arg1);
void Input_SetPadEnabledFlag(s32 arg0, s32 arg1);

void Input_InitPadHandlers(u32 index,u32 h8,u32 w4,u32 w0,u32 hA,u32 hC,u32 h10,u32 h12) {Record *r=&D_80140BF0[index];r->word4=w4;r->word0=w0;r->half8=h8;r->halfA=hA;r->halfC=hC;r->half10=h10;r->half12=h12;}

void Input_ApplyPadConfig(Sprite *spr) {
    Input_InitPadHandlers(spr->index, spr->unkC, spr->unk8, spr->unk4, spr->unkE, spr->unk10, spr->unk14, spr->unk16);
    Input_SetAnalogBounds(spr->index, spr->unk1C, spr->unk1E, spr->unk20, spr->unk22);
    input_status_update(spr->index, spr->unk1A, spr->unk18, spr->unk12);
    Input_SetPadSecondaryFlag(spr->index, spr->unk19);
    Input_SetPadEnabledFlag(spr->index, spr->unk0 == -1);
}

/* Complete nonmatch, 2026-10-05, 0x80094FE8..0x8009508C.
 * Historical name: audio_channel_reset. This is a player/car flag scan that
 * updates Sprite.unk1A and reapplies the pad configuration only on a change.
 * N64-specific callback; no arcade donor is established (reference checkout
 * unavailable). The two real definitions above are unchanged accepted context.
 * Named car-index and signed-flag locals recover the native loop structure;
 * the remaining mismatch is allocation, not a matching or coverage claim.
 */

typedef struct Player76 {u8 p0,car;u8 pad2[74];} Player76;
typedef struct Car772 {u8 pad0[6];s8 flag;u8 pad7[765];} Car772;
extern s16 D_8014A108;
extern Player76 D_8014A118[];
extern Car772 D_80144030[];
s32 audio_channel_reset(Sprite *config) {
s32 all=1;
 s32 i,car,active;
 for(i=0;i<D_8014A108;i++) {
  car=D_8014A118[i].car;
  active=D_80144030[car].flag;
  if(active==0)all=0;
 }
 if(all!=config->unk1A) {
  config->unk1A=all;
  Input_ApplyPadConfig(config);
 }
 return 1;
}
