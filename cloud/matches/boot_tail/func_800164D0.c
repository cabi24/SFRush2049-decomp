/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Append a previously unseen descriptor table; initialize each state byte.
 * Guard acquisition can change the live table count, so it is reloaded. */
typedef unsigned short u16;
typedef struct {
    unsigned char unknown_00[9];
    unsigned char state;
    unsigned char unknown_0A[2];
} Descriptor;
#pragma pack(1)
typedef struct {
    u16 id;
    u16 count;
    Descriptor *descriptors;
} RecordFields;
#pragma pack()
typedef union {
    RecordFields fields;
    unsigned int words[2];
} RecordStorage;
extern int D_80042228;
extern RecordStorage D_80042230[];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_800164D0(u16 id, Descriptor *descriptors, u16 count)
{
    int i;
    for (i = 0; i < D_80042228 && ((RecordFields *)&D_80042230[i])->id != id; i++) {
    }
    if (i == D_80042228 && D_80042228 < 128) {
        func_80014594();
        ((RecordFields *)&D_80042230[D_80042228])->id = id;
        ((RecordFields *)&D_80042230[D_80042228])->count = count;
        ((RecordFields *)&D_80042230[D_80042228])->descriptors = descriptors;
        for (i = 0; i < count; i++) {
            descriptors->state = 31;
            descriptors++;
        }
        D_80042228++;
        func_800145DC();
        return 1;
    }
    return 0;
}
