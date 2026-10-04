/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SamplePayload {
    u32 frequency, lengthFormat, loopStart, loopLength;
} SamplePayload;
typedef struct SampleInfo {
    u32 frequency;
    void *data;
    u32 offset, length, loopStart, loopLength;
    u8 format;
} SampleInfo;
#pragma pack()
typedef struct SampleRecord {
    u16 identifier, references;
    u32 offset;
    void *data;
    SamplePayload descriptor;
} SampleRecord;
typedef struct RegisteredSamples {
    SampleRecord *records;
    void *base;
    u16 count, unknown0A;
} RegisteredSamples;
extern int D_800385A0;
extern RegisteredSamples D_800385A8[8];
extern u16 D_80042648;
extern SampleRecord *D_80042664;
extern SamplePayload *D_80042668;
extern void *func_8001E864(const void *, const void *, int, int,
                         int (*)(const void *, const void *));
extern int func_80016CE0(const void *, const void *);
int func_80016CF0(u16 identifier, SampleInfo *sample)
{
    int i;
    SampleRecord *record;
    SamplePayload *payload;
    D_80042648 = identifier;
    for (i = 0; i < D_800385A0; i++) {
        record = func_8001E864(&D_80042648, D_800385A8[i].records,
            D_800385A8[i].count, sizeof(SampleRecord), func_80016CE0);
        if (record != 0) {
            payload = (SamplePayload *)((u8 *)record + sizeof(*record) - sizeof(*payload));
            sample->frequency = payload->frequency;
            sample->data = record->data;
            sample->offset = 0;
            sample->loopStart = payload->loopStart;
            sample->length = payload->lengthFormat & 0xFFFFFF;
            sample->loopLength = payload->loopLength;
            sample->format = payload->lengthFormat >> 24;
            D_80042668 = payload;
            D_80042664 = record;
            return 0;
        }
        D_80042664 = record;
    }
    return -1;
}
