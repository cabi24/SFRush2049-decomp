/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 scene-parent search. Record layout is corroborated by the accepted
 * child/sibling accessors; no complete arcade ancestor is established.
 * Search each record's first-child/sibling chain and return the first owning
 * record. The comparison precedes the -1 terminator check, so querying -1
 * can return an owner as well. Input and return are signed halfwords; the
 * record counter itself is full-width. Requires valid terminating chains. */
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef struct SceneRecord {
    u8 prefix[22];
    s16 child;
    s16 sibling;
    u8 tail[42];
} SceneRecord;
extern SceneRecord D_8012E700[];
extern s32 D_80156990;

s16 func_800A7BF8(s16 child)
{
    s32 i;
    s16 search_key;
    s32 next;
    s16 first_child;
    s16 sibling;
    SceneRecord *record;

    i = 0;
    if (D_80156990 > 0) {
        search_key = child;
        do {
            record = &D_8012E700[i];
            first_child = record->child;
            if (search_key == first_child)
                return i;
            next = first_child;
            if (first_child != -1) {
                do {
                    sibling = D_8012E700[next].sibling;
                    if (search_key == sibling)
                        return i;
                    next = sibling;
                } while (sibling != -1);
            }
            i++;
        } while (i < D_80156990);
    }
    return -1;
}
