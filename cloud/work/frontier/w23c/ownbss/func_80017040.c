/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80017040 - lib_16320 (IDO 7.1). MusyX synthdata.c dataGetFX: search every registered FX group
 * (D_80042230[0..D_80042228), packed {id, count, table}) for FX ID with func_8001E864 (sndBSearch, comparator
 * func_80017018 = fxcmp, 12-byte entries) and return the entry, else 0.
 * w23c: MusyX's function-local static key IS retail's D_800426A0 (code identical with it; D10 BT03-low-lookups
 * true=950). Needs lib_16320's owned .bss block grown to 0x80042630..0x800426AF (RESULTS.md "ownbss").
 * No devices, no unused locals. Types: the TU's Descriptor/RecordFields/RecordStorage/PackedLeadingKey (verbatim)
 * and a private 12-byte key record FxKey_80017040 (FX_TAB). */
typedef unsigned short u16;
typedef struct { unsigned char unknown_00[9]; unsigned char state; unsigned char unknown_0A[2]; } Descriptor;
#pragma pack(1)
typedef struct { u16 id; u16 count; Descriptor *descriptors; } RecordFields;
#pragma pack()
typedef union { RecordFields fields; unsigned int words[2]; } RecordStorage;
typedef struct FxKey_80017040 { u16 id; unsigned char unknown02[10]; } FxKey_80017040;
#pragma pack(1)
typedef struct PackedLeadingKey { unsigned short key; } PackedLeadingKey;
#pragma pack()
extern int D_80042228;
extern RecordStorage D_80042230[];
int func_80017018(const PackedLeadingKey *left, const PackedLeadingKey *right);
void *func_8001E864(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
Descriptor *func_80017040(u16 id)
{
    static FxKey_80017040 key_80017040;
    Descriptor *result;
    long i;

    key_80017040.id = id;
    for (i = 0; i < D_80042228; ++i) {
        if ((result = func_8001E864(&key_80017040, ((RecordFields *)&D_80042230[i])->descriptors,
                                    ((RecordFields *)&D_80042230[i])->count, sizeof(Descriptor),
                                    (int (*)(const void *, const void *))func_80017018)) != 0) {
            return result;
        }
    }
    return 0;
}
