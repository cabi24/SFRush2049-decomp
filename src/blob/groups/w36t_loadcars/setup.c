/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800BB02C(index, kind, data) -- initialise the 24-byte per-node slot
 * D_8013FEE0[index] (clear flag/active, marker 255, value 0x8000) and give it a
 * 512-byte buffer: the caller's `data` if supplied; otherwise, when the shared
 * heap has less than 512 bytes free (audio_output_setup(NULL) = free total), share
 * the buffer of the first earlier node whose gLink entry (D_80153E88, 8 bytes,
 * as in func_800EC914) has the same kind byte and copy that node's three payload
 * bytes; else allocate with audio_dma_sync(0, 512).  Caller: audio_interrupt_handler.
 * No arcade ancestor found.
 *
 * Shaping (prior research cloud/work/ipa-groups/codex_slots_a93 had the logic):
 *  - no `slot` pointer local: every access is D_8013FEE0[index].field, so the
 *    slot address is a spilled temporary (frame 32, home 24), not a named local (frame 40);
 *  - the shared buffer pointer is copied before the payload bytes;
 *  - `if (i == index) {}` after the loop is compiled-out code: it is retail's
 *    `bnel v1,a3` / `b` pair at the loop exit.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct Owner Owner;
typedef struct Slot24 { u8 flag; u8 other1[15]; u8 active, marker; u16 value; void *data; } Slot24;
typedef struct Link { u8 state; u8 kind; u8 payload[3]; u8 other5[3]; } Link;

extern Slot24 D_8013FEE0[];
extern Link D_80153E88[];
extern u32 audio_output_setup(Owner *);
extern void *audio_dma_sync(int, int);

void func_800BB02C(int index, int kind, void *data)
{
    int i;

    D_8013FEE0[index].flag = 0;
    D_8013FEE0[index].active = 0;
    D_8013FEE0[index].marker = 255;
    D_8013FEE0[index].value = 0x8000;
    if (data != 0) {
        D_8013FEE0[index].data = data;
    } else if (audio_output_setup(0) < 512) {
        for (i = 0; i < index; i++) {
            if (kind == D_80153E88[i].kind) {
                D_8013FEE0[index].data = D_8013FEE0[i].data;
                D_80153E88[index].payload[0] = D_80153E88[i].payload[0];
                D_80153E88[index].payload[1] = D_80153E88[i].payload[1];
                D_80153E88[index].payload[2] = D_80153E88[i].payload[2];
                break;
            }
        }
        if (i == index) {
        }
    } else {
        D_8013FEE0[index].data = audio_dma_sync(0, 512);
    }
}
