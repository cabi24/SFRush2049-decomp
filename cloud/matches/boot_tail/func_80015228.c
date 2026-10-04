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
extern void *func_80014CAC(void *);
extern int func_8001605C(SampleRecord *, void *);
extern int func_800162AC(u16, SampleRecord *);

void func_80015228(u16 *ids, void *address, SampleRecord *records)
{
    u16 id;
    if (func_8001605C(records, func_80014CAC(address))) {
        while ((id = *ids) != 0xFFFF) {
            func_800162AC(*ids++, records);
        }
    }
}
