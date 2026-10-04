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
extern RegisteredSamples D_800385A8[];
extern void func_80014CFC(void **, void **);

int func_800162AC(u16 id, SampleRecord *records)
{
    u32 index;
    void *descriptor;
    for (index = 0; index < (u32)D_800385A0 &&
         D_800385A8[index].records != records; ++index) {
    }
    while (records->identifier != 0xFFFF) {
        if (records->identifier == id) {
            if (records->references == 0) {
                records->data = D_800385A8[index].base + records->offset;
                descriptor = records->descriptor;
                func_80014CFC(&descriptor, &records->data);
            }
            ++records->references;
            break;
        }
        ++records;
    }
    return 1;
}
