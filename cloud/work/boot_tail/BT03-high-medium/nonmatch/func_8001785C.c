/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: see README.md and verification.json. */
typedef unsigned char u8;
typedef struct MapRecord { u8 unknown00[5]; u8 key; u8 unknown06[2]; } MapRecord;
void func_8001785C(u8 *map, MapRecord *record)
{
    u8 index;
    for (index = 0; index < 128; index++) map[index] = 255;
    index = 0;
    while (record->key != 255) {
        map[record->key] = index;
        index++;
        record++;
    }
}
