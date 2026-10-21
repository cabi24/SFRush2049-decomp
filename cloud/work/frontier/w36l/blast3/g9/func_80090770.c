/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Complete 44-byte indexed halfword setter at 0x80090770.
 * Native record stride is 0x44; its unsigned halfword is at offset 0x14.
 * The index is signed 16-bit and the value is unsigned 16-bit.
 * No direct arcade equivalent is known. Preserve statement line boundaries.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct IndexedRecord44 {
    u8 unknown00[0x14];
    u16 value;
    u8 unknown16[0x2E];
} IndexedRecord44;

extern IndexedRecord44 D_8012E700[];

void func_80090770(s16 index, u16 value) {
    D_8012E700[index].value = value;
}
