/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Sequence start, [0x800979A0,0x80097AC4), 292 bytes.
 * The second word argument is genuinely homed but unused in retail.
 * The accepted slot_value_get is inlined; func_80096288 preserves a3.
 * External services remain ordinary O32 call boundaries.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef struct ResourceGroup ResourceGroup;
typedef struct SampleRecord SampleRecord;
typedef struct ResourceOffsets ResourceOffsets;
typedef struct SequenceOptions SequenceOptions;
typedef struct SeqEntry { u16 h0; u16 h2; } SeqEntry;
extern u8 D_80151960;
extern s32 D_8011EAA0, D_80151A6C, D_80151AD4;
extern void *D_80151ADC;
extern SeqEntry D_8011F070[];
extern ResourceGroup *D_80152464;
extern void *D_801525FC;
extern SampleRecord *D_80152690;
extern ResourceOffsets *D_801526D8;
s32 audio_frame_sync(s32, s32, s32, s32, void *);
void display_list_alloc(s32);
void *slot_value_get(s32);
s32 func_8001536C(ResourceGroup *, u16, void *, SampleRecord *, ResourceOffsets *);
s32 func_800156E8(u16, u16, void *, SequenceOptions *);

void func_800979A0(s32 arg0, s32 arg1) {
    SeqEntry *e;

    if (D_80151960 == 0) {
        if (D_8011EAA0 == -1 || arg0 != D_80151A6C) {
            D_80151AD4 = audio_frame_sync(arg0 + 10, 0, 0, 0, 0);
            display_list_alloc(D_80151AD4);
            D_80151ADC = slot_value_get(D_80151AD4);
            e = &D_8011F070[arg0];
            func_8001536C(D_80152464, e->h2, D_801525FC, D_80152690, D_801526D8);
            D_8011EAA0 = func_800156E8(e->h2, arg0, D_80151ADC, 0);
            D_80151A6C = arg0;
        }
    }
}
