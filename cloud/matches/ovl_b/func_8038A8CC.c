/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Standalone match: no callers, inlined helpers, or deleted-static stubs required in this unit. */
/* Runtime image B: 0x8038A8CC..0x8038A95C, 144 bytes.
 * Initialize the 25-slot surface-object ring and resolve its texture index.
 * N64-specific body, no whole-function arcade donor or original type claim.
 * Ring widths and size are witnessed by B:8038A408. The texture lookup uses
 * the unchanged accepted 800B24EC MBOX_FindTexture_Sub boundary.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef struct RingState {
    s8 wrapped;
    u8 unknown01;
    u16 next;
    void *objects[25];
} RingState;
typedef struct NameEntry { char name[36]; } NameEntry;
extern RingState D_80399A70;
extern s16 D_80399AD8;
extern char D_80394D14[];
extern volatile u8 D_80140BDC;
extern NameEntry *func_800B24EC(char *, s16 *, s8, s8, s32);
void func_8038A8CC(void)
{
    s32 i;
    D_80399A70.wrapped = 0;
    D_80399A70.next = 0;
    func_800B24EC(D_80394D14, &D_80399AD8, 0, D_80140BDC - 1, 1);
    for (i = 0; i < 25; i++) {
        D_80399A70.objects[i] = 0;
    }
}
