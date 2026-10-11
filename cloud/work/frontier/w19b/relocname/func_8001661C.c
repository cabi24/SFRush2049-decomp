/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001661C (IDO 7.1): code identical; relocation-name split only (w19b). Retail reaches a label that splat
 * placed at a compiler-made address (base-8/-12 loop end pointer or base+4 field). With the reloc_addrs.us.txt
 * entries in cloud/work/frontier/w19b/RESULTS.md (re-split target) this source scores
 * LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical. Prior: D10/BT03 (and w18c relocname/ for 1661C/158D8/15F28). */
/* Eight-byte table records have a word-aligned copy representation and a
 * packed field view. Payload bits are opaque here. Only active records may
 * be searched or shifted; the native tables supply that bounded domain. */
typedef unsigned short u16;
typedef struct { unsigned char unknown_00[9]; unsigned char state; unsigned char unknown_0A[2]; } Descriptor;
#pragma pack(1)
typedef struct { u16 id; u16 count; Descriptor *descriptors; } RecordFields;
#pragma pack()
typedef union { RecordFields fields; unsigned int words[2]; } RecordStorage;
extern int D_80042228;
extern RecordStorage D_80042230[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_8001661C(u16 id)
{
    int i;
    int j;
    for (i = 0; i < D_80042228 && ((RecordFields *)&D_80042230[i])->id != id; i++) {
    }
    if (i != D_80042228) {
        func_80014594();
        for (j = i + 1; j < D_80042228; j++) {
            D_80042230[j - 1] = D_80042230[j];
        }
        D_80042228--;
        func_800145DC();
        return 1;
    }
    return 0;
}
