/* Research-only inferred views of witnessed native record layouts.
 * Real accepted setter interfaces; no global storage ownership is claimed.
 * The complete caller remains NONMATCH (22/25 words). See README.md.
 */
typedef short s16;
typedef int s32;
typedef unsigned char u8;
typedef struct CarPart64 { u8 prefix[20]; s32 model; u8 rest[40]; } CarPart64;
typedef struct Slot68 { u8 rest[60]; s32 first, second; } Slot68;
extern CarPart64 D_80139320[];
extern Slot68 D_8012E700[];
void func_8008E06C(s16 slot, s32 *value)
{
    D_8012E700[slot].first = *value;
}
void func_80092BC8(s16 slot, s32 *value)
{
    D_8012E700[slot].second = *value;
}
void func_80092BF4(s16 key, s32 *first, s32 *second)
{
    s32 slot = D_80139320[key].model;
    func_8008E06C(slot, first);
    func_80092BC8(slot, second);
}
