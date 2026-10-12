/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80016C20 - lib_16320 (IDO 7.1). MusyX synthdata.c dataGetMacro (doldecomp/ttyd libs/musyx; Rush's revision
 * passes the count straight to the search): main = id >> 6; if macro main table D_8003DA28[main] has entries,
 * binary-search (func_8001E864 = sndBSearch, comparator func_80016BF8 = maccmp) the sub table D_8003E228 from its
 * first index for ID and return the entry's data pointer, else 0.
 * w23c: MusyX's function-local statics base, main, key, result ARE retail's D_80042630/34/38(+4 = D_8004263C)/40
 * (code identical with them; as extern globals D10 BT03-low-lookups true=805). Needs lib_16320's owned .bss block
 * grown to 0x80042630 (see RESULTS.md "ownbss"). No devices, no unused locals. Types: the TU's ResourceRange,
 * RecordFields_8003E228/RecordStorage_8003E228 and PackedKey (verbatim). */
typedef unsigned short u16;
#pragma pack(1)
typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;
#pragma pack(0)
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003E228;
#pragma pack()
typedef union { RecordFields_8003E228 fields; unsigned int words[2]; } RecordStorage_8003E228;
#pragma pack(1)
typedef struct PackedKey { unsigned char unknown00[4]; unsigned short key; } PackedKey;
#pragma pack()
extern ResourceRange D_8003DA28[512];
extern RecordStorage_8003E228 D_8003E228[];
int func_80016BF8(const PackedKey *left, const PackedKey *right);
void *func_8001E864(const void *key, const void *base, int count, int size, int (*compare)(const void *, const void *));
void *func_80016C20(u16 id)
{
    static long base_80016C20;
    static long main_80016C20;
    static RecordFields_8003E228 key_80016C20;
    static RecordFields_8003E228 *result_80016C20;

    main_80016C20 = id >> 6;
    if (D_8003DA28[main_80016C20].count != 0) {
        base_80016C20 = D_8003DA28[main_80016C20].first;
        key_80016C20.id = id;
        if ((result_80016C20 = func_8001E864(&key_80016C20, &D_8003E228[base_80016C20], D_8003DA28[main_80016C20].count, 8,
                                    (int (*)(const void *, const void *))func_80016BF8)) != 0) {
            return result_80016C20->payload;
        }
    }
    return 0;
}
