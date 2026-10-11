/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80016EE0 - lib_16320 (IDO 7.1). Looks up id in the 8-byte record table D_8003C618[D_8003C610] (bsearch func_8001E864, comparator func_80016E40) and returns the record's payload or 0. Twin of func_80016E68 (needed with it for the contiguous .bss block).
 * w20b: the search key and the result are FUNCTION-LOCAL STATICS (MusyX snd_data.c dataGet* shape:
 * `static KEY key; static KEY *result; key.id = id; if ((result = sndBSearch(&key, ...)) != NULL) ...`;
 * the dataGet* identification is a hypothesis from the code shape, not proven). uopt handles the
 * address-not-taken static `result` unlike an extern global: retail's store/re-read (E68/EE0) and the
 * result kept in v1 and stored on both exits (F80) follow from it.
 * As extern globals (w18c/w19b) the store/reload pattern cannot be reproduced (best true=225..245).
 * Code: true=0 strict=0 reloc_blind=0; the only objdump-dr rows that differ are the static relocations
 * (.bss+N vs the splat labels D_800426xx). The three statics of func_80016E68 / func_80016EE0 /
 * func_80016F80 form the TU's whole .bss (0x30 B) and land at retail's 0x80042670..0x8004269F only as
 * one owned storage block (rom_owned_data.json storage_blocks, existing-TU mode); see w20b RESULTS.md.
 * Types: the TU's own record typedefs and PackedKey comparator prototypes, verbatim. Devices: the
 * comparator cast to the bsearch parameter type (no code); the statics carry the table address in
 * their names (key_<table>, result_<table>) because the ROM-TU build's asm-processor (--convert-statics
 * local) rejects two function statics with one name in a TU ("symbol key defined twice"); names do not
 * change code. No unused locals. */
typedef unsigned short u16;
#pragma pack(1)
typedef struct { void *payload; u16 id; u16 references; } RecordFields_8003C618;
#pragma pack()
typedef union { RecordFields_8003C618 fields; unsigned int words[2]; } RecordStorage_8003C618;
extern int D_8003C610;
extern RecordStorage_8003C618 D_8003C618[];
#pragma pack(1)
typedef struct PackedKey { unsigned char unknown00[4]; unsigned short key; } PackedKey;
#pragma pack()
int func_80016E40(const PackedKey *left, const PackedKey *right);
void *func_8001E864(const void *key, const void *base, int count, int size,
                  int (*compare)(const void *, const void *));

void *func_80016EE0(u16 id)
{
    static RecordFields_8003C618 key_8003C618;
    static RecordFields_8003C618 *result_8003C618;

    key_8003C618.id = id;
    if ((result_8003C618 = func_8001E864(&key_8003C618, D_8003C618, D_8003C610, sizeof(RecordFields_8003C618),
                                (int (*)(const void *, const void *))func_80016E40)) != 0) {
        return result_8003C618->payload;
    }
    return 0;
}
