/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native reconstruction of the model-record halfword setter.
 * This is a complete-record representation, not an original N64 typedef.
 * Five 16-byte spans start at +8. Span 0 covers header storage; its tail
 * is the signed count at +22. Actual four payload slots start at +24.
 * Selector -1 marks the header halfword at +10 while setting all payload
 * values. It does NOT mark each payload flag. The observed direct caller
 * passes selector 0; use of negative selectors in gameplay is unproven.
 * Defined domain: mapped bank/record, count <= 4, selector >= -1; larger
 * positive selectors return 0 without forming a payload pointer.
 */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

typedef struct RecordSpan16 {
    u16 value;
    u16 flags;
    u8 unobserved04[10];
    s16 tail;
} RecordSpan16;
typedef struct ModelRecord {
    u8 unobserved00[8];
    RecordSpan16 spans[5];
} ModelRecord;
typedef struct ModelBank { ModelRecord *records; s32 count; } ModelBank;
extern ModelBank D_801161F4[];

s32 func_8008B000(u16 handle, s16 selector, u16 value)
{
    ModelRecord *model;
    s32 i;
    model = &D_801161F4[handle >> 10].records[handle & 1023];
    if (selector >= 0) {
        if (selector >= model->spans[0].tail) return 0;
        model->spans[selector + 1].value = value;
        model->spans[selector + 1].flags |= 0x8000;
    } else {
        for (i = 0; i < model->spans[0].tail; i++) {
            model->spans[i + 1].value = value;
            model->spans[selector + 1].flags |= 0x8000;
        }
    }
    return 1;
}
