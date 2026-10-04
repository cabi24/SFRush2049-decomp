/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SampleRecord {
    u16 identifier;
    u16 references;
    u32 offset;
    void *data;
    u8 descriptor[16];
} SampleRecord;
typedef struct RegisteredSamples {
    SampleRecord *records;
    unsigned char *base;
    u16 count;
    u16 unknown0A;
} RegisteredSamples;
extern int D_800385A0;
extern RegisteredSamples D_800385A8[8];
extern void func_80014594(void);
extern void func_800145DC(void);

int func_800161A0(SampleRecord *records)
{
    int index;
    for (index = 0; index < D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    if (index != D_800385A0) {
        func_80014594();
        for (index = index + 1; index < D_800385A0; ++index) {
            D_800385A8[index - 1] = D_800385A8[index];
        }
        --D_800385A0;
        func_800145DC();
        return 1;
    }
    return 0;
}
