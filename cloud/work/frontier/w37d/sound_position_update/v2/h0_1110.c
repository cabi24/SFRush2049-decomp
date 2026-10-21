/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct FxTag {      /* tFxSequence, N64 offsets */
    u8 pad0[0x10];
    s32 state;              /* 0x10 */
    s32 type;               /* 0x14 */
    u8 pad18;
    s8 loop;                /* 0x19 */
    u8 numCmds;             /* 0x1A */
    u8 pad1B[5];
    f32 startTime;          /* 0x20 */
    f32 vol;                /* 0x24 */
} FxTag;
typedef struct SortedTag {  /* anon_te_tag_c */
    u8 pad0[8];
    FxTag *tag;             /* 0x8 */
} SortedTag;
typedef struct AudioMsg {   /* tAudioMsg */
    u8 pad0[2];
    u8 cmd;                 /* 0x2 hdr.cmd */
    u8 pad3;
    FxTag *tag;             /* 0x4 */
} AudioMsg;

extern s32 D_80146200;          /* gMaxSfx */
extern AudioMsg *D_80143CF0[];  /* gStopCmds */
extern s32 D_80110250;          /* gNumStopCmds */
extern AudioMsg *D_80143AE8[];  /* gStartCmds */
extern s32 D_8011024C;          /* gNumStartCmds */
AudioMsg *func_80091B00(void);  /* _GetCmd */
s32 func_80095198(f32 x, f32 delta);   /* AudioTimeout */

void sound_position_update(void *element, s32 elNum)
{
    SortedTag *stag = element;
    AudioMsg *cmd;
    FxTag *tag = stag->tag;

    if (tag->state == 2 || tag->state != 1) {
        if (elNum >= D_80146200 * 0.75f ||
            (tag->loop && tag->state == 2 && tag->vol <= 0.0f && func_80095198(tag->startTime, 0.2f))) {
            if (tag->state == 2) {
                cmd = func_80091B00();
                D_80143CF0[D_80110250++] = cmd;
                cmd->cmd = 5;
                cmd->tag = stag->tag;
                cmd->tag->numCmds++;
            }
        } else if (stag->tag->state != 2 &&
                   (stag->tag->vol > 0.1f || (!stag->tag->loop && stag->tag->type == 1))) {
            cmd = func_80091B00();
            D_80143AE8[D_8011024C++] = cmd;
            cmd->cmd = 3;
            cmd->tag = stag->tag;
            cmd->tag->numCmds++;
        }
    }
}
