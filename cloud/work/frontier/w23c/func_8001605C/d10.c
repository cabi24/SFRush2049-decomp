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

int func_8001605C(SampleRecord *records, void *base)
{
    int index;
    u16 count;
    u16 i;
    SampleRecord *entry;
    for (index = 0; index < D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    if (index == D_800385A0) {
        if (D_800385A0 < 8) {
            count = 0;
            entry = records;
            while (entry->identifier != 0xFFFF) {
                ++count;
                ++entry;
            }
            entry = records;
            func_80014594();
            D_800385A8[D_800385A0].records = records;
            D_800385A8[D_800385A0].base = base;
            D_800385A8[D_800385A0].count = count;
            for (i = 0; i < count; ++i) {
                entry->references = 0;
                ++entry;
            }
            ++D_800385A0;
            func_800145DC();
            return 1;
        }
        return 0;
    }
    return 1;
}
