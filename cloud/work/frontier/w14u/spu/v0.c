/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct SndNode { u8 pad0[0x10]; s32 type; s32 unk14; u8 pad1; s8 unk19; u8 unk1A; u8 pad2[5]; f32 unk20; f32 unk24; } SndNode;
typedef struct SndEmitter { u8 pad[8]; SndNode *node; } SndEmitter;
typedef struct SndRec { u8 pad[2]; u8 kind; u8 pad2; SndNode *node; } SndRec;
extern s32 D_80146200;
extern f32 D_80152748;
extern s32 D_80110250;
extern s32 D_8011024C;
extern void *D_80143CF0[];
extern void *D_80143AE8[];
void *func_80091B00(void);

void sound_position_update(SndEmitter *arg0, s32 arg1)
{
    SndNode *node;
    SndRec *rec;
    s32 type;
    s32 n;
    f32 f2;
    f32 f12;
    f32 f14;

    node = arg0->node;
    type = node->type;
    if (type == 2) {
    }
    if (type == 1) {
        return;
    }
    if (0.75f * (f32) D_80146200 <= (f32) arg1) {
        goto block_11;
    }
    if (node->unk19 != 0 && type == 2 && !(node->unk24 > 0.0f)) {
        f14 = D_80152748;
        f12 = node->unk20 + 0.2f;
        f2 = f12;
        if (f14 < node->unk20) {
            f2 = f12 - 14400.0f;
        }
        if (f2 < f14) {
            goto block_11;
        }
    }
    if (type != 2) {
        if (0.1f < node->unk24 || (node->unk19 == 0 && node->unk14 == 1)) {
            rec = (SndRec *) func_80091B00();
            n = D_8011024C;
            D_80143AE8[n] = rec;
            D_8011024C = n + 1;
            rec->kind = 3;
            rec->node = arg0->node;
            node->unk1A++;
        }
    }
    return;
block_11:
    if (type == 2) {
        rec = (SndRec *) func_80091B00();
        n = D_80110250;
        D_80143CF0[n] = rec;
        D_80110250 = n + 1;
        rec->kind = 5;
        rec->node = arg0->node;
        node->unk1A++;
    }
}
